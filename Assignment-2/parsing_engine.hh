#pragma once

#include "Node.h"
#include "stdlib.h"
#include <unordered_map>
#include "error_codes.hh"
#include <stdexcept> // Include for std::runtime_error

namespace symbol_table {

    class SymbolRecord {
    protected:
        string name;
        string type; // For variable: int, int[], boolean, custom class types; For method: return type; For class: class name itself
        string sym_record; // "global" for class, class name for method, method name for variable
    public:
        SymbolRecord(string name, string type)
            : name(name), type(type) {}
        virtual ~SymbolRecord() {}

        void setName(string name) { this->name = name; }
        string getName() { return this->name; }

        void setType(string type) { this->type = type; }
        string getType() { return this->type; }

        string getRecord() { return "\tRecord: " + this->sym_record + "\tName: " + this->name + "\tType: " + this->type; }
    };


    class Variable : public SymbolRecord {
    public:
        // Additional properties specific to variables can be added here
        Variable(string name, string type)
            : SymbolRecord(name, type) { this->sym_record = "Variable"; }
    };


    class Method : public SymbolRecord {
    private:     // Method-specific properties, e.g., parameters
        // unordered_map<string, Variable*> params;
        std::vector<Variable*> params;
        unordered_map<string, Variable*> vars;
    public:
        Method(string name, string returnType)
            : SymbolRecord(name, returnType) { this->sym_record = "Method"; }

        void addVariable(Variable* var) { this->vars[var->getName()] = var; }
        void addParameter(Variable* var) { this->params.push_back(var);}

        size_t getParamSize() { return this->params.size(); }
        std::vector<Variable*> getParams() { return this->params; }
    };


    class Class : public SymbolRecord {
    private:    // Class-specific properties, e.g., member variables, methods
        unordered_map<string, Variable*> vars;
        unordered_map<string, Method*> methods;
    public:
        Class(string name, string type)
            : SymbolRecord(name, type) { this->sym_record = "Class";}

        void addVariable(Variable* var) { this->vars[var->getName()] = var;}
        void addMethod(Method* method) { this->methods[method->getName()] = method;}

        Variable* lookupVariable(string name) {
            if(this->vars.find(name) != this->vars.end()) return this->vars[name];

            return NULL;
        }
        Method* lookupMethod(string name) {
            if(this->methods.find(name) != this->methods.end()) return this->methods[name];

            return NULL;
        }
    };


    class Scope {
    private:
        int next = 0;
        Scope* parentScope;
        unordered_map<string, SymbolRecord*> records;
        SymbolRecord* scopeContext = nullptr;
    public:
        vector<Scope*> childrenScopes;
        string scopeName;

        // Default constructor
        Scope() : parentScope(nullptr), next(0) {};
        // Needed input constructor
        Scope(Scope *parent, string name, SymbolRecord* scopeContext = nullptr)
            : parentScope(parent), scopeName(name), scopeContext(scopeContext) {};
        
        Scope* getParent() { return this->parentScope; };

        SymbolRecord* getScopeContext() { return this->scopeContext; }

        Scope* nextChild(string name, SymbolRecord* classContext) {
            Scope* nextChild;
            if (next == childrenScopes.size()) {
                nextChild = new Scope(this, name, classContext);
                childrenScopes.push_back(nextChild);
            } else {
                nextChild = childrenScopes.at(next);
            }

            this->next++;
            return nextChild;
        }

        SymbolRecord* lookup(string key) {
            if (this->records.find(key) != this->records.end()) {
                std::cout << key << " exists in records" << std::endl;
                return this->records[key];
            }
            if(parentScope == NULL) {
                std::cout << "parentScope is null" << std::endl;
                return NULL;
            }
            std::cout << key << " exists in parentScope" << std::endl;
            return parentScope->lookup(key);
            // return this->records.find(key) != this->records.end() ? this->records[key] : (this->parentScope ? this->parentScope->lookup(key) : NULL);
        }

        void put(string key, SymbolRecord* record) {
            this->records[key] = record;
        }

        void printScope(int level = 0) {
            for (auto i = records.begin(); i != records.end(); i++) {
                std::cout << std::string(level, '\t') << records[i->first]->getRecord() << std::endl;
            }
        }

        void resetScope() {
            this->next = 0;
            for (int i = 0; i < childrenScopes.size(); i++) {
                childrenScopes.at(i)->resetScope();
            }
        }
    };


    class SymbolTable {
    private:
        Scope* root;
        Scope* current;

    public:
        SymbolTable();
        ~SymbolTable();

        void enterScope(string name, SymbolRecord* scopeContext);
        void exitScope();

        Scope* getCurrentScope() { return this->current; };

        void put(string key, SymbolRecord* item);

        SymbolRecord* findSymbol(string name);

        void printTable();
        void printAllTable(Scope* node, int level = 1);

        void resetTable();
    };


    void traverseTree(Node* root, SymbolTable* sym_table);
}

namespace semantic_analysis {
    std::string traverseTreeSemantically(Node* node, symbol_table::SymbolTable* sym_table);
    errCodes semantic_analysis(Node* node, symbol_table::SymbolTable* smy_table);
}