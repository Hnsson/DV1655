#pragma once

#include "tac.hh"
#include "Node.h"
#include "parsing_engine.hh"
#include <vector>

namespace intermediate_representation {
    class BBlock {
        public:
            std::string name;
            std::vector<Tac*> tacInstructions;
            Tac* condition;
            BBlock *trueExit, *falseExit;
            BBlock()
                : trueExit(nullptr), falseExit(nullptr) {}
            ~BBlock() {
                for(auto* instr : tacInstructions) {
                    delete instr;
                }

                delete condition;
            }
    };

    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table);
}
