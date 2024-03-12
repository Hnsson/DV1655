#pragma once

#include "byte_code.hh"
#include "error_codes.hh"
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_map>

namespace stack_machine {
    class StackMachine {
        std::stack<int> dataStack;
        // std::unordered_map<std::string, int> localVariables;
        // std::unordered_map<std::string, int> tempVariables;
    public:
        std::stack<std::unordered_map<std::string, int>> localStack;
        std::stack<int> activationStack;
        void executeInstruction(const byte_code::Instruction& instruction, int& line_no, std::unordered_map<std::string, int>& method_line);
        void setVariable(const std::string& name, int value);
        int getVariable(const std::string& name);
    };
}

namespace interpreter {
    class Interpreter {
    public:
        Interpreter(std::string file_path);
        ~Interpreter();

        errCodes interpret();
    private:
        std::string file_path;
        std::ifstream file_stream;

        void init(std::string file_path);
        byte_code::Instruction parseInstruction(std::string line, std::unordered_map<std::string, int>& method_line, int line_no);
    };
}