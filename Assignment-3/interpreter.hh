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

// Stack Machine
namespace stack_machine {
    class StackMachine {
        // Stack for data storage
        std::stack<int> dataStack;
    public:
        // Stack for local variables based on scope
        std::stack<std::unordered_map<std::string, int>> localStack;
        // Stack for method activation
        std::stack<int> activationStack;
        void executeInstruction(const byte_code::Instruction& instruction, int& line_no, std::unordered_map<std::string, int>& method_line);
        void setVariable(const std::string& name, int value);
        int getVariable(const std::string& name);
    };
}
// Interpreter
namespace interpreter {
    class Interpreter {
    public:
        Interpreter(std::string file_path);
        ~Interpreter();
        // Interpret the byte code
        errCodes interpret();
    private:
        std::string file_path;
        std::ifstream file_stream;
        // Initialize the interpreter
        void init(std::string file_path);
        // Parse the byte code from the file to instruction format
        byte_code::Instruction parseInstruction(std::string line, std::unordered_map<std::string, int>& method_line, int line_no);
    };
}