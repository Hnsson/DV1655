#pragma once

#include "tac.hh"
#include "Node.h"
#include "parsing_engine.hh"
#include <unordered_set>
#include <vector>

namespace {
    class BBlock { // Basic Block
        public:
            std::string name; // Name of the block
            std::vector<Tac*> tacInstructions; // Instructions in the block
            Tac* condition; // Condition for the block
            BBlock *trueExit, *falseExit; // True and False exits from the block
            BBlock(std::string name = "")
                : name(name), trueExit(nullptr), falseExit(nullptr) {}
            ~BBlock() {
                for(auto* instr : tacInstructions) {
                    delete instr;
                }

                delete condition;
            }
    };
} // namespace BBlock

namespace intermediate_representation {
    BBlock* current_block = nullptr; // Current block
    std::vector<BBlock*> method_blocks; // Blocks for the method
    std::string generateTempId(); // Generate a temporary id
    std::string generateBlockId(); // Generate a block id

    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table, BBlock* prev_block); // Traverse the tree for IR generation

    void generateCFGContent(BBlock* block, ofstream& outStream, unordered_set<BBlock*>& visited); // Generate the CFG content
    errCodes generateCFG(); // Generate the CFG

    void generateCodeContent(BBlock* block, ofstream& outStream, unordered_set<BBlock*>& visited, bool print); // Generate the code content
    errCodes generateCode(); // Generate the code

    errCodes generateIR(Node* root, symbol_table::SymbolTable* sym_table); // Generate the IR
} // namespace intermediate_representation
