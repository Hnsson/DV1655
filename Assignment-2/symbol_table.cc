#include "symbol_table.hh"

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

void SymbolTable::printAllTable(Scope* node, int level = 1) {
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