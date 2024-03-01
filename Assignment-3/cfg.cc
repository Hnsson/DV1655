#include "cfg.hh"

namespace intermediate_representation {
    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table) {
        if(node->type == "Program") {
            if(!node->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = node->children.begin(); i != node->children.end(); ++i) {
                    sym_table->enterScope("Class: " + (*i)->value, NULL);
                        traverseTreeIR(*i, sym_table);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Main Class") {
            sym_table->enterScope("Method: main", NULL);
                // Handle the statements as a method here:
            sym_table->exitScope();
        }
        if(node->type == "Class") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    traverseTreeIR(*i, sym_table);
                }
            }
        }
        if(node->type == "Var declarations") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                }
            }
        }
        if(node->type == "MethodDeclaration") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    sym_table->enterScope("Method: " + (*i)->value, NULL);
                        traverseTreeIR(*i, sym_table);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Method") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Method parameters") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "MethodContent") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Content") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Return") {
            // Add logic later
        }
        if(node->type == "Var declaration") {
        }

        return "Intermediate Representation";
    }
}