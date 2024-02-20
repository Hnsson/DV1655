#include "symbol_table.h"

class SymbolRecord {
protected:
    string name;
    string type; // For variable: int, int[], boolean, custom class types; For method: return type; For class: class name itself
    string sym_record; // "global" for class, class name for method, method name for variable
public:
    SymbolRecord(string name, string type)
        : name(name), type(type) {}

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
    unordered_map<string, Variable*> params;
    unordered_map<string, Variable*> vars;
public:
    Method(string name, string returnType)
        : SymbolRecord(name, returnType) { this->sym_record = "Method"; }

    void addVariable(Variable* var) { this->vars[var->getName()] = var; }
    void addParameter(Variable* var) { this->params[var->getName()] = var;}
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
public:
    vector<Scope*> childrenScopes;
    string scopeName;
    // Default constructor
    Scope() {};
    // "Copy" constructor
    Scope(const Scope& other)
        : parentScope(other.parentScope), scopeName(other.scopeName) {};
    // Needed input constructor
    Scope(Scope *parent, string name)
        : parentScope(parent), scopeName(name) {};
    
    Scope* getParent() { return this->parentScope; };

    Scope* nextChild(string name) {
        Scope* nextChild;
        if (next == childrenScopes.size()) {
            nextChild = new Scope(this, name);
            childrenScopes.push_back(nextChild);
        } else {
            nextChild = childrenScopes.at(next);
        }

        this->next++;
        return nextChild;
    }

    SymbolRecord* lookup(string key) {
        return this->records.find(key) != this->records.end() ? this->records[key] : (this->parentScope ? this->parentScope->lookup(key) : NULL);
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
    SymbolTable() {
        root = new Scope();
        current = root;
        // Initialize with global scope if necessary
    }

    ~SymbolTable() {
        // Cleanup
    }

    void enterScope(string name) {
        current = current->nextChild(name);
    }

    void exitScope() {
        current = current->getParent();
    }

    void put(string key, SymbolRecord* item) {
        current->put(key, item);
    }

    SymbolRecord* findSymbol(string name) {
        return current->lookup(name);
    }

    // void printTable() {
    //     root->printScope();
    // }

    void printTable() {
        cout << std::endl << "-----" << root->scopeName << "-----" << std::endl;
        root->printScope();

        for(auto i = root->childrenScopes.begin(); i != root->childrenScopes.end(); i++){
            printAllTable(*i);
        }
        cout << "----- end" << root->scopeName << "-----" << std::endl;
    }

    void printAllTable(Scope* node, int level = 1) {
        std::cout << std::endl << std::string(level, '\t') << "-----" << node->scopeName << "-----" << std::endl;
        node->printScope(level);

        // Correctly increment level for nested scopes, without affecting siblings
        for(auto i = node->childrenScopes.begin(); i != node->childrenScopes.end(); i++){
            printAllTable(*i, level + 1); // Use level + 1 here
        }

        std::cout << std::string(level, '\t') << "----- end " << node->scopeName << "-----" << std::endl;
    }

    void resetTable() {
        root->resetScope();
    }
};

void traverseTree(Node* root, SymbolTable* sym_table) {
    if(root->type == "Program") {
        if(!root->children.empty()) {
            // Assuming the first child is always the main class and the rest are part of the class list
            for(auto i = root->children.begin(); i != root->children.end(); ++i) {
                if ((*i)->type == "Main Class") {
                    // Handle the main class
                    sym_table->put((*i)->value, new Class((*i)->value, (*i)->type));
                    sym_table->enterScope("Class: " + (*i)->value);
                    sym_table->put("this", new Variable("this", (*i)->value));
                    traverseTree(*i, sym_table);
                    sym_table->exitScope();
                } else if ((*i)->type == "ClassList") {
                    // Traverse each class within the class list
                    for (auto classNode = (*i)->children.begin(); classNode != (*i)->children.end(); ++classNode) {
                        sym_table->put((*classNode)->value, new Class((*classNode)->value, "Class"));
                        sym_table->enterScope("Class: " + (*classNode)->value);
                        sym_table->put("this", new Variable("this", (*classNode)->value));
                        traverseTree(*classNode, sym_table);
                        sym_table->exitScope();
                    }
                }
            }
        }
    }
    if(root->type == "Main Class") {
        sym_table->put("main", new Method("main", "void"));
        sym_table->enterScope("Method: main");
        // Assuming the first child of the main class is the parameter "String[] a"
        if (!root->children.empty()) {
            sym_table->put(root->children.front()->value, new Variable(root->children.front()->value, "String[]"));
        }
        sym_table->exitScope();
    }
    if(root->type == "Class") {
        if(!root->children.empty()) {
            for(auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table);
            }
        }
    }
    if(root->type == "Var declarations") {
        if(!root->children.empty()) {
            for(auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table);
            }
        }
    }
    if(root->type == "MethodDeclaration") {
        if(!root->children.empty()) {
            for(auto i = root->children.begin(); i != root->children.end(); i++) {
                Node* type = *(*i)->children.begin();
                sym_table->put((*i)->value, new Method((*i)->value, type->value));

                sym_table->enterScope("Method: " + (*i)->value);
                traverseTree(*i, sym_table);
                sym_table->exitScope();
            }
        }
    }
    if(root->type == "Method") {
        for (auto i = root->children.begin(); i != root->children.end(); i++) {
            traverseTree(*i, sym_table);
        }
    }
    if(root->type == "Method parameters") {
        for (auto i = root->children.begin(); i != root->children.end(); i++) {
            traverseTree(*i, sym_table);
        }
    }
    if(root->type == "MethodContent") {
        for (auto i = root->children.begin(); i != root->children.end(); i++) {
            traverseTree(*i, sym_table);
        }
    }
    if(root->type == "Content") {
        for (auto i = root->children.begin(); i != root->children.end(); i++) {
            traverseTree(*i, sym_table);
        }
    }
    if(root->type == "Return") {
        // Add logic later
    }
    if(root->type == "Var declaration") {
        sym_table->put(root->children.back()->value, new Variable(root->children.back()->value, root->children.front()->value));
    }
}