#pragma once

#include "tac.hh"
#include "Node.h"
#include "parsing_engine.hh"
#include <vector>

namespace {
    class BBlock {
        public:
            std::string name;
            std::vector<Tac*> tacInstructions;
            Tac* condition;
            BBlock *trueExit, *falseExit;
            BBlock(std::string name = "")
                : name(name), trueExit(nullptr), falseExit(nullptr) {}
            ~BBlock() {
                for(auto* instr : tacInstructions) {
                    delete instr;
                }

                delete condition;
            }
    };
}

namespace intermediate_representation {
    BBlock* current_block = nullptr;
    std::vector<BBlock*> method_blocks;
    std::string generateTempId();
    std::string generateBlockId();

    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table, BBlock* current_block);

    errCodes generateCFG();
    errCodes generateIR(Node* root, symbol_table::SymbolTable* sym_table);
}
