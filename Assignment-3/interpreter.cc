#include "interpreter.hh"

namespace stack_machine
{
    void StackMachine::setVariable(const std::string& name, int value) {
        localStack.top()[name] = value; // Set the variable in the local stack
    }
    int StackMachine::getVariable(const std::string& name) {
        if (localStack.top().find(name) != localStack.top().end()) return localStack.top()[name]; // Get the variable from the local stack

        std::cerr << "Variable (" << name << ") not found." << std::endl;
        return 0;
    }

    void StackMachine::executeInstruction(const byte_code::Instruction& instruction, int& line_no, std::unordered_map<std::string, int>& method_line) {
        int op_1, op_2, result; // Variables for operations

        switch (instruction.type) { // Execute the instruction based on the type
            case byte_code::ILOAD: { // Load the variable to the stack
                // Assuming you have a way to access local variables, e.g., an array or map
                op_1 = getVariable(instruction.argument);
                dataStack.push(op_1);
            } break;
            case byte_code::ICONST: // Push the constant to the stack
                dataStack.push(std::stoi(instruction.argument));
                break;
            case byte_code::ISTORE: { // Store the variable from the stack
                if (!dataStack.empty()) {
                    setVariable(instruction.argument, dataStack.top());
                    dataStack.pop();
                }
            } break;
            case byte_code::IADD: { // Add the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    result = op_1 + op_2;
                    dataStack.push(result);
                }
            } break;
            case byte_code::ISUB: { // Subtract the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    result = op_1 - op_2;
                    dataStack.push(result);
                }
            } break;
            case byte_code::IMUL: { // Multiply the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    result = op_1 * op_2;
                    dataStack.push(result);
                }
            } break;
            case byte_code::IDIV: { // Divide the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    if (op_2 == 0) {
                        std::cerr << "Division by zero error" << std::endl;
                        break;
                    }
                    result = op_1 / op_2;
                    dataStack.push(result);
                }
            } break;
            case byte_code::ILT: { // Compare the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(op_1 < op_2 ? 1 : 0);
                }
            } break;
            case byte_code::IGT: { // Compare the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(op_1 > op_2 ? 1 : 0);
                }
            } break;
            case byte_code::IEQ: { // Compare the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(op_1 == op_2 ? 1 : 0);
                }
            } break;
            case byte_code::IAND: { // Logical AND the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push((op_1 && op_2) ? 1 : 0);
                }
            } break;
            case byte_code::IOR: { // Logical OR the top two elements from the stack
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push((op_1 || op_2 ) ? 1 : 0);
                }
            } break;
            case byte_code::INOT: { // Logical NOT the top element from the stack
                if (!dataStack.empty()) {
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(!op_1);
                }
            } break;
            case byte_code::PRINT: { // Print the top element from the stack
                int result = dataStack.top();
                std::cout << "Program print: " << result << std::endl;
                dataStack.pop();
            } break;
            case byte_code::IF_FALSE_GOTO: { // Goto the line number if the top element from the stack is false
                int condition = dataStack.top();
                dataStack.pop();
                if (!condition) {
                    line_no = method_line[instruction.argument + ":"]; // Goto the line number of the method
                }
            } break;
            case byte_code::INVOKEVIRTUAL: { // Method invocation
                activationStack.push(line_no);
                localStack.push(std::unordered_map<std::string, int>());
                line_no = method_line[instruction.argument + ":"]; // Goto the method line number of the method
            } break;
            case byte_code::GOTO: // Goto the line number
                line_no = method_line[instruction.argument + ":"]; // Goto the line number of the method
                break;
            case byte_code::IRETURN: { // Return from the method
                line_no = activationStack.top();
                localStack.pop();
                activationStack.pop();
            } break;
        }
    }
} // namespace stack_machine


namespace interpreter {
    Interpreter::Interpreter(std::string file_path)
        : file_path(file_path) {
        init(file_path);
    }
    Interpreter::~Interpreter() {
        // Close the file stream
        if (file_stream.is_open()) {
            file_stream.close();
        }
    }

    void Interpreter::init(std::string file_path) {
        std::cout << "Initializing interpreter..." << std::endl;
        // Open the file for reading
        file_stream.open(file_path);
        if(!file_stream.is_open()) { // Check if the file is opened successfully
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return;
        }
        // Print success message
        std::cout << "File: (" << file_path << ") opened for reading..." << std::endl;
    }

    byte_code::Instruction Interpreter::parseInstruction(std::string line, std::unordered_map<std::string, int>& method_line, int line_no) {
        // Initialize the instruction
        byte_code::Instruction instruction;

        std::istringstream iss(line);
        // Tokenizes the string into a vector of tokens using whitespace as delimiter
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        instruction.type = byte_code::METHOD;
        if (tokens.empty()) return instruction; // Return default instruction for whitespace lines
        // Get the operation from the first token
        const std::string& op = tokens[0];


        // Map string operation to InstructionType enum
        if (op == "iload") instruction.type = byte_code::ILOAD;
        else if (op == "istore") instruction.type = byte_code::ISTORE;
        else if (op == "iconst") instruction.type = byte_code::ICONST;
        else if (op == "iadd") instruction.type = byte_code::IADD;
        else if (op == "isub") instruction.type = byte_code::ISUB;
        else if (op == "imul") instruction.type = byte_code::IMUL;
        else if (op == "idiv") instruction.type = byte_code::IDIV;
        else if (op == "ilt") instruction.type = byte_code::ILT;
        else if (op == "igt") instruction.type = byte_code::IGT;
        else if (op == "ieq") instruction.type = byte_code::IEQ;
        else if (op == "iand") instruction.type = byte_code::IAND;
        else if (op == "ior") instruction.type = byte_code::IOR;
        else if (op == "inot") instruction.type = byte_code::INOT;
        else if (op == "goto") instruction.type = byte_code::GOTO;
        else if (op == "iffalse") instruction.type = byte_code::IF_FALSE_GOTO;
        else if (op == "invokevirtual") instruction.type = byte_code::INVOKEVIRTUAL;
        else if (op == "ireturn") instruction.type = byte_code::IRETURN;
        else if (op == "print") instruction.type = byte_code::PRINT;
        else if (op == "stop") instruction.type = byte_code::STOP;
        else {
            // Handle label/block identifiers
            method_line[op] = line_no;
            instruction.type = byte_code::METHOD;
            instruction.argument = op;
            return instruction; // Return default instruction but with method name as argument
       }

        if (tokens.size() > 1 && op != "iffalse") { // If the operation has an argument (e.g., iload, istore, iconst) then set the argument
            instruction.argument = tokens[1];
        } else if (tokens.size() > 1 && op == "iffalse") { // If the operation is iffale then set the argument
            instruction.argument = tokens[2];
        }

        return instruction;
    }

    errCodes Interpreter::interpret() {
        if(!file_stream.is_open()) { // Check if the file is opened successfully
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return errCodes::INTERPRETER_ERROR;
        }
        // Initialize the stack machine
        stack_machine::StackMachine machine;
        // Push an empty local stack for the first method
        machine.localStack.push(std::unordered_map<std::string, int>());
        // Read the file line by line
        std::string line;
        // Initialize the program as a vector of instructions
        std::vector<byte_code::Instruction> program;
        // Map method names to line numbers for method invocation
        std::unordered_map<std::string, int> method_line;

        int line_no = 0;
        while(std::getline(file_stream, line)) {
            byte_code::Instruction instruction = parseInstruction(line, method_line, line_no); // Parse the line to an instruction (the function also handles method names and line numbers for method invocation)
            program.push_back(instruction); // Add the instruction to the program

            line_no++;
        }

        // Printing methods with corresponding line number
        // for (const auto& pair : method_line) {
        //     std::cout << pair.first << " : " << pair.second << std::endl;
        // }

        std::cout << "- Program start -" << std::endl;
        line_no = 0;
        while(program[line_no].type != byte_code::STOP) { // Execute the program until the stop instruction is reached
            if(program[line_no].type != byte_code::METHOD) machine.executeInstruction(program[line_no], line_no, method_line); // Execute the instruction

            line_no++;
        }
        std::cout << "-----------------" << std::endl;


        return errCodes::SUCCESS; // Return success
    }
} // namespace interpreter