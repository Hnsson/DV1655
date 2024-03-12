#include "interpreter.hh"

namespace stack_machine
{
    void StackMachine::setVariable(const std::string& name, int value) {
        localStack.top()[name] = value;
    }
    int StackMachine::getVariable(const std::string& name) {
        if (localStack.top().find(name) != localStack.top().end()) return localStack.top()[name];

        std::cerr << "Variable (" << name << ") not found." << std::endl;
        return 0;
    }

    void StackMachine::executeInstruction(const byte_code::Instruction& instruction, int& line_no, std::unordered_map<std::string, int>& method_line) {
        int op_1, op_2, result;

        // std::cout << "\tLINE: " << line_no + 1 << std::endl;
        switch (instruction.type) {
            case byte_code::ILOAD:
                if (instruction.argument != "Param") {
                    // Assuming you have a way to access local variables, e.g., an array or map
                    op_1 = getVariable(instruction.argument);
                    // std::cout << "PUSHING: "<< instruction.argument << " TO STACK: " << op_1 << std::endl;
                    // op_1 = localStack.top()[instruction.argument];
                    dataStack.push(op_1);
                }
                break;
            case byte_code::ICONST:
                // std::cout << "PUSING: "<< std::stoi(instruction.argument) << " TO STACK" << std::endl;
                dataStack.push(std::stoi(instruction.argument));
                break;
            case byte_code::ISTORE:
                if (!dataStack.empty()) {
                    // std::cout << "ISTORE: " << instruction.argument << ", " << dataStack.top() << std::endl;
                    setVariable(instruction.argument, dataStack.top());
                    // localStack.top()[std::stoi(instruction.argument)] = dataStack.top();
                    dataStack.pop();
                }
                break;
            case byte_code::IADD:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    result = op_1 + op_2;
                    dataStack.push(result);
                }
                break;
            case byte_code::ISUB:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    result = op_1 - op_2;
                    dataStack.push(result);
                }
                break;
            case byte_code::IMUL:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    result = op_1 * op_2;
                    dataStack.push(result);
                }
                break;
            case byte_code::IDIV:
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
                break;
            case byte_code::ILT:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(op_1 < op_2 ? 1 : 0);
                }
                break;
            case byte_code::IGT:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(op_1 > op_2 ? 1 : 0);
                }
                break;
            case byte_code::IEQ:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(op_1 == op_2 ? 1 : 0);
                }
                break;
            case byte_code::IAND:
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push((op_1 && op_2) ? 1 : 0);
                }
                break;
            case byte_code::IOR: {
                if (dataStack.size() >= 2) {
                    op_2 = dataStack.top(); dataStack.pop();
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push((op_1 || op_2 ) ? 1 : 0);
                }
            } break;
            case byte_code::INOT: {
                if (!dataStack.empty()) {
                    op_1 = dataStack.top(); dataStack.pop();
                    dataStack.push(!op_1);
                }
            } break;
            case byte_code::PRINT: {
                int result = dataStack.top();
                std::cout << "Program print: " << result << std::endl;
                dataStack.pop();
            } break;
            case byte_code::IF_FALSE_GOTO: {
                int condition = dataStack.top();
                dataStack.pop();
                if (!condition) {
                    line_no = method_line[instruction.argument + ":"];
                }
            } break;
            case byte_code::INVOKEVIRTUAL: {
                activationStack.push(line_no);
                localStack.push(std::unordered_map<std::string, int>());
                line_no = method_line[instruction.argument + ":"];
                // activationStack.push(method_line[instruction.argument + ":"]);
                // Method invocation would require a separate mechanism to handle calling and returning from methods.
            } break;
            case byte_code::GOTO:
                line_no = method_line[instruction.argument + ":"];
                break;
            case byte_code::IRETURN: {
                line_no = activationStack.top();
                localStack.pop();
                activationStack.pop();
            } break;
        }
        // std::cout << std::endl;
    }
} // namespace stack_machine


namespace interpreter {
    Interpreter::Interpreter(std::string file_path)
        : file_path(file_path) {
        init(file_path);
    }
    Interpreter::~Interpreter() {
        if (file_stream.is_open()) {
            file_stream.close();
        }
    }

    void Interpreter::init(std::string file_path) {
        std::cout << "Initializing interpreter..." << std::endl;

        file_stream.open(file_path);
        if(!file_stream.is_open()) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return;
        }

        std::cout << "File: (" << file_path << ") opened for reading..." << std::endl;
    }

    byte_code::Instruction Interpreter::parseInstruction(std::string line, std::unordered_map<std::string, int>& method_line, int line_no) {
        byte_code::Instruction instruction;

        std::istringstream iss(line);
        // Tokenizes the string into a vector of tokens
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        instruction.type = byte_code::METHOD;
        if (tokens.empty()) return instruction;

        // std::cout << line << " : " << tokens.size() << std::endl;
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
            // Handle unknown instruction or label/block identifiers
            // std::cerr << "Unknown instruction or label: " << op << std::endl;
            method_line[op] = line_no;
            instruction.type = byte_code::METHOD;
            instruction.argument = op;
            return instruction; // Return default instruction for now
        }

        if (tokens.size() > 1 && op != "iffalse") {
            instruction.argument = tokens[1];
        } else if (tokens.size() > 1 && op == "iffalse") {
            instruction.argument = tokens[2];
        }

        return instruction;
    }

    errCodes Interpreter::interpret() {
        if(!file_stream.is_open()) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return errCodes::INTERPRETER_ERROR;
        }

        stack_machine::StackMachine machine;
        machine.localStack.push(std::unordered_map<std::string, int>());
        std::string line;

        std::vector<byte_code::Instruction> program;
        std::unordered_map<std::string, int> method_line;

        int line_no = 0;
        while(std::getline(file_stream, line)) {
            byte_code::Instruction instruction = parseInstruction(line, method_line, line_no);
            program.push_back(instruction);

            // std::cout << line_no + 1 << " : " << instruction.type << line << std::endl;
            line_no++;
        }

        // Printing methods with corresponding line number
        // for (const auto& pair : method_line) {
        //     std::cout << pair.first << " : " << pair.second << std::endl;
        // }

        line_no = 0;
        while(program[line_no].type != byte_code::STOP) {
            // std::cout << "EXECUTING LINE: " << line_no + 1 << std::endl;

            if(program[line_no].type != byte_code::METHOD) {

                machine.executeInstruction(program[line_no], line_no, method_line);
            }

            // if (machine.activationStack.size() > 0) std::cout << machine.activationStack.size() << " : " << machine.activationStack.top() << std::endl;

            line_no++;
        }



        return errCodes::SUCCESS;
    }
} // namespace interpreter