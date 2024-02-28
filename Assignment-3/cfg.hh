#pragma once

#include "tac.hh"
#include <vector>

class BBlock {
    public:
        std::string name;
        std::vector<Tac> tacInstructions;
        Tac condition;
        BBlock *trueExit, *falseExit;
        BBlock()
            : trueExit(nullptr), falseExit(nullptr) {}
};