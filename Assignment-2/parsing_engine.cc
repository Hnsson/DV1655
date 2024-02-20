#include "parsing_engine.hh"

namespace symbol_table {
    SymbolTable::SymbolTable() {
        root = new Scope();
        current = root;
        // Initialize with global scope if necessary
    }

    SymbolTable::~SymbolTable() {};

    void SymbolTable::enterScope(string name)  {
        current = current->nextChild(name);
    }

    void SymbolTable::exitScope()  {
        current = current->getParent();
    }

    void SymbolTable::put(string key, SymbolRecord* item) {
        current->put(key, item);
    }

    SymbolRecord* SymbolTable::findSymbol(string name) {
        return current->lookup(name);
    }

    void SymbolTable::printTable() {
        cout << std::endl << "-----" << root->scopeName << "-----" << std::endl;
        root->printScope();

        for(auto i = root->childrenScopes.begin(); i != root->childrenScopes.end(); i++){
            printAllTable(*i);
        }
        cout << "----- end" << root->scopeName << "-----" << std::endl;
    }

    void SymbolTable::printAllTable(Scope* node, int level) {
        std::cout << std::endl << std::string(level, '\t') << "-----" << node->scopeName << "-----" << std::endl;
        node->printScope(level);

        // Correctly increment level for nested scopes, without affecting siblings
        for(auto i = node->childrenScopes.begin(); i != node->childrenScopes.end(); i++){
            printAllTable(*i, level + 1); // Use level + 1 here
        }

        std::cout << std::string(level, '\t') << "----- end " << node->scopeName << "-----" << std::endl;
    }

    void SymbolTable::resetTable() {
        root->resetScope();
    }





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
}

namespace semantic_analysis {


    errCodes traverseTreeSemantically(Node* node, symbol_table::SymbolTable* sym_table) {
        try {
            // - Verify that all identifiers are declared
            // - Type checking
            // - Expressions: e.g., In a + b; both a and b should be integers
            // - Statemetns: e.g., In a = b; type of a and b should be the same
            // - Methods: In int a(int c) ... return b; the type of return
            //            expression (b) should be the same as type of the method declaration (int).
            // - Method calls: In a(x); the type of x and c should be the same
            // - Array access: In b[a]; b.length the type of a must be integer; the type of b must
            //                 be int[];
            // - Out of scope: Inheritance, polymorphic calls, and other OO features
            
            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEMANTIC_ERROR; // Return a semantic error code if an exception was caught
        }
    }
}