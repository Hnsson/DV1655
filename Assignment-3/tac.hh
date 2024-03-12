#pragma once

#include <stdio.h>
#include <string>
#include "byte_code.hh"


class Tac {
    protected:
        std::string op, lhs, rhs, result;
    public:
        Tac(std::string _op = "", std::string _y = "", std::string _z = "", std::string _result = "")
            : op(_op), lhs(_y), rhs(_z), result(_result) {}
        virtual ~Tac() {}

        virtual void dump() = 0; 
        virtual std::string getDump() = 0;

        virtual std::string generateCode() = 0;
};

class Expression : public Tac {
public:
    Expression(std::string _op, std::string _y, std::string _z, std::string _result)
        : Tac(_op, _y, _z, _result) {}
    
    void dump() override {
        printf("%s := %s %s %s", result.c_str(), lhs.c_str(), op.c_str(), rhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + lhs + " " + op + " " + rhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _lhs = new byte_code::Instruction();
        byte_code::Instruction* _rhs = new byte_code::Instruction();
        byte_code::Instruction* _op = new byte_code::Instruction();
        byte_code::Instruction* _result = new byte_code::Instruction();

        if (lhs == "true") {
            _lhs->argument = "1"; // True represented as integer 1
            _lhs->type = byte_code::ICONST;
        } else if (lhs == "false") {
            _lhs->argument = "0"; // False represented as integer 0
            _lhs->type = byte_code::ICONST;
        } else {
            // Determine if 'lhs' is a number or an identifier
            if (byte_code::is_number(lhs)) {
                _lhs->argument = lhs; // Directly use the number as the argument
                _lhs->type = byte_code::ICONST;
            } else {
                // Treat 'lhs' as an identifier
                _lhs->argument = lhs;
                _lhs->type = byte_code::ILOAD;
            }
        }

        if (rhs == "true") {
            _rhs->argument = "1"; // True represented as integer 1
            _rhs->type = byte_code::ICONST;
        } else if (rhs == "false") {
            _rhs->argument = "0"; // False represented as integer 0
            _rhs->type = byte_code::ICONST;
        } else {
            // Determine if 'lhs' is a number or an identifier
            if (byte_code::is_number(rhs)) {
                _rhs->argument = rhs; // Directly use the number as the argument
                _rhs->type = byte_code::ICONST;
            } else {
                // Treat 'lhs' as an identifier
                _rhs->argument = rhs;
                _rhs->type = byte_code::ILOAD;
            }
        }

        if (op == "+") {
            _op->type = byte_code::IADD;
        } else if (op == "-") {
            _op->type = byte_code::ISUB;
        } else if (op == "*") {
            _op->type = byte_code::IMUL;
        } else if (op == "/") {
            _op->type = byte_code::IDIV;
        } else if (op == "<") {
            _op->type = byte_code::ILT;
        } else if (op == ">") {
            _op->type = byte_code::IGT;
        } else if (op == "==") {
            _op->type = byte_code::IEQ;
        } else if (op == "&&") {
            _op->type = byte_code::IAND;
        } else if (op == "||") {
            _op->type = byte_code::IOR;
        }

        _result->type = byte_code::ISTORE;
        _result->argument = result;

        std::string code = _lhs->generateLine() + "\n\t" + _rhs->generateLine() + "\n\t" + _op->generateLine() + "\n\t" + _result->generateLine();
        delete _lhs; delete _rhs; delete _op; delete _result;
        return code;
    }
};

class UnaryExpression : public Tac {
public:
    UnaryExpression(std::string _op, std::string _y, std::string _result)
        : Tac(_op, _y, "", _result) {}
    
    void dump() override {
        printf("%s := %s %s", result.c_str(), op.c_str(), lhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + op + " " + lhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _result = new byte_code::Instruction();
        byte_code::Instruction* _op = new byte_code::Instruction();
        byte_code::Instruction* _lhs = new byte_code::Instruction();

        // Set lhs instruction, (either constant "int, bool" or idenfitifer)
        // If a bool, true = 1, false = 0
        // If integer just see if type casting works, if not then it is a identifier
        if (lhs == "true") {
            _lhs->argument = "1"; // True represented as integer 1
            _lhs->type = byte_code::ICONST;
        } else if (lhs == "false") {
            _lhs->argument = "0"; // False represented as integer 0
            _lhs->type = byte_code::ICONST;
        } else {
            // Determine if 'lhs' is a number or an identifier
            if (byte_code::is_number(lhs)) {
                _lhs->argument = lhs; // Directly use the number as the argument
                _lhs->type = byte_code::ICONST;
            } else {
                // Treat 'lhs' as an identifier
                _lhs->argument = lhs;
                _lhs->type = byte_code::ILOAD;
            }
        }

        // Set operator instruction, ()
        _op->type = byte_code::INOT;

        // Create final instruction
        _result->type = byte_code::ISTORE;
        _result->argument = result;

        return _lhs->generateLine() + "\n\t" + _op->generateLine() + "\n\t" + _result->generateLine();
    }
};

class MethodCall : public Tac {
public:
    MethodCall(std::string _f, std::string _N, std::string _result)
        : Tac("call", _f, _N, _result) {}

    void dump() override {
        printf("%s := %s %s, %s", result.c_str(), op.c_str(), lhs.c_str(), rhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + op + " " + lhs + ", " + rhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _result = new byte_code::Instruction();
        byte_code::Instruction* _invoke = new byte_code::Instruction();

        if (std::stoi(rhs) == 0) {
            // No parameter, just invoke function
            _invoke->type = byte_code::INVOKEVIRTUAL;
            _invoke->argument = lhs;
        } else {
            // Paramters, need to push to stack `rhs` amount. "no difference???"
            _invoke->type = byte_code::INVOKEVIRTUAL;
            _invoke->argument = lhs;
        }

        // Create final instruction
        _result->type = byte_code::ISTORE;
        _result->argument = result;

        std::string code = _invoke->generateLine() + "\n\t" + _result->generateLine();
        delete _result; delete _invoke;
        return code;
    }
};

class Jump : public Tac {
public:
    Jump(std::string _label)
        : Tac("goto", "", "", _label) {}
    
    void dump() override {
        printf("%s %s", op.c_str(), result.c_str());
    }
    std::string getDump() override {
        return op + " " + result;
    }

    std::string generateCode() override {
        byte_code::Instruction* line = new byte_code::Instruction();
        line->type = byte_code::GOTO;
        line->argument = this->result;

        return line->generateLine();
    }
};

class CondJump : public Tac {
public:
    CondJump(std::string _x, std::string _label)
        : Tac("iffalse", _x, "", _label) {}

    void dump() override {
        printf("%s %s goto %s", op.c_str(), result.c_str(), lhs.c_str());
    }
    std::string getDump() override {
        return op + " " + result + " goto " + lhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _lhs = new byte_code::Instruction();
        byte_code::Instruction* _result = new byte_code::Instruction();

        if (lhs == "true") {
            _lhs->argument = "1"; // True represented as integer 1
            _lhs->type = byte_code::ICONST;
        } else if (lhs == "false") {
            _lhs->argument = "0"; // False represented as integer 0
            _lhs->type = byte_code::ICONST;
        } else {
            // Determine if 'lhs' is a number or an identifier
            if (byte_code::is_number(lhs)) {
                _lhs->argument = result; // Directly use the number as the argument
                _lhs->type = byte_code::ICONST;
            } else {
                // Treat 'lhs' as an identifier
                _lhs->argument = result;
                _lhs->type = byte_code::ILOAD;
            }
        }

        _result->type = byte_code::IF_FALSE_GOTO;
        _result->argument = lhs;

        // Assuming generateLine forms the correct bytecode line based on type and argument
        std::string code = _lhs->generateLine() + "\n\t" + _result->generateLine();
        delete _lhs; // Clean up
        delete _result; // Clean up
        return code;
    }
};

class ArrayAccess : public Tac {
public:
    ArrayAccess(std::string _array, std::string _index, std::string _result)
        : Tac("[]", _array, _index, _result) {}
    
    void dump() override {
        printf("%s := %s[%s]", result.c_str(), lhs.c_str(), rhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + lhs + "[" + rhs + "]";
    }

    std::string generateCode() override { return "";

    }
};

class ArrayAssign : public Tac {
public:
    ArrayAssign(std::string _array, std::string _index, std::string _result)
        : Tac("[]", _array, _index, _result) {}
    
    void dump() override {
        printf("%s[%s] := %s", lhs.c_str(), rhs.c_str(), result.c_str());
    }
    std::string getDump() override {
        return lhs + "[" + rhs + "] := " + result;
    }

    std::string generateCode() override { return "";

    }
};

class LengthAccess : public Tac {
public:
    LengthAccess(std::string _array, std::string _result)
        : Tac("length", _array, "", _result) {}

    void dump() override {
        printf("%s := %s %s", result.c_str(), op.c_str(), lhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + op + " " + lhs;
    }

    std::string generateCode() override { return "";

    }
};

class Copy : public Tac {
public:
    Copy(std::string _y, std::string _result)
        : Tac("=", _y, "", _result) {}

    void dump() override {
        printf("%s := %s", result.c_str(), lhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + lhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _lhs = new byte_code::Instruction();
        byte_code::Instruction* _result = new byte_code::Instruction();

        // Handle boolean and number constants
        if (lhs == "true") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "1"; // True represented as integer 1
        } else if (lhs == "false") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "0"; // False represented as integer 0
        } else if (byte_code::is_number(lhs)) {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = lhs; // Directly use the number as the argument
        } else {
            _lhs->type = byte_code::ILOAD;
            _lhs->argument = lhs; // Treat as identifier
        }

        // Store the value into 'result'
        _result->type = byte_code::ISTORE;
        _result->argument = result;

        // Assuming generateLine forms the correct bytecode line based on type and argument
        std::string code = _lhs->generateLine() + "\n\t" + _result->generateLine();
        delete _lhs; // Clean up
        delete _result; // Clean up
        return code;
    }
};

class Print : public Tac {
public:
    Print(std::string _y)
        : Tac("", _y, "", "") {}

    void dump() override {
        printf("print: %s", lhs.c_str());
    }
    std::string getDump() override {
        return "print: " + lhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _result = new byte_code::Instruction();
        byte_code::Instruction* _lhs = new byte_code::Instruction();

        // Handle boolean and number constants
        if (lhs == "true") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "1"; // True represented as integer 1
        } else if (lhs == "false") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "0"; // False represented as integer 0
        } else if (byte_code::is_number(lhs)) {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = lhs; // Directly use the number as the argument
        } else {
            _lhs->type = byte_code::ILOAD;
            _lhs->argument = lhs; // Treat as identifier
        }

        // Set operator instruction, ()
        _result->type = byte_code::PRINT;


        return _lhs->generateLine() + "\n\t" + _result->generateLine();
    }
};

class Parameter : public Tac {
public:
    Parameter(std::string _y)
        : Tac("param", _y, "", "") {}

    void dump() override {
        printf("param %s", lhs.c_str());
    }
    std::string getDump() override {
        return "param " + lhs;
    }

    std::string generateCode() override {
        // Handle boolean and number constants
        byte_code::Instruction* _lhs = new byte_code::Instruction();

        if (lhs == "true") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "1"; // True represented as integer 1
        } else if (lhs == "false") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "0"; // False represented as integer 0
        } else if (byte_code::is_number(lhs)) {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = lhs; // Directly use the number as the argument
        } else {
            _lhs->type = byte_code::ILOAD;
            _lhs->argument = lhs; // Treat as identifier
        }

        std::string code = _lhs->generateLine();
        delete _lhs;
        return code;
    }
};

class Return : public Tac {
public:
    Return(std::string _y)
        : Tac("return", _y, "", "") {}

    void dump() override {
        printf("return %s", lhs.c_str());
    }
    std::string getDump() override {
        return "return " + lhs;
    }

    std::string generateCode() override {
        byte_code::Instruction* _result = new byte_code::Instruction();
        byte_code::Instruction* _lhs = new byte_code::Instruction();

        // Handle boolean and number constants
        if (lhs == "true") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "1"; // True represented as integer 1
        } else if (lhs == "false") {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = "0"; // False represented as integer 0
        } else if (byte_code::is_number(lhs)) {
            _lhs->type = byte_code::ICONST;
            _lhs->argument = lhs; // Directly use the number as the argument
        } else {
            _lhs->type = byte_code::ILOAD;
            _lhs->argument = lhs; // Treat as identifier
        }

        // Set operator instruction, ()
        _result->type = byte_code::IRETURN;


        return _lhs->generateLine() + "\n\t" + _result->generateLine();
    }
};

class NewObject : public Tac {
public:
    NewObject(std::string _type, std::string _result)
        : Tac("new", _type, "", _result) {}

    void dump() override {
        printf("%s := %s %s", result.c_str(), op.c_str(), lhs.c_str());
    }
    std::string getDump() override {
        return result + " := " + op + " " + lhs;
    }

    std::string generateCode() override { return "";

    }
};

class NewArray : public Tac {
public:
    NewArray(std::string _type, std::string _size, std::string _result)
        : Tac("new array", _type, _size, _result) {}

    void dump() override {
        printf("%s := new %s, %s", result.c_str(), lhs.c_str(), rhs.c_str());
    }
    std::string getDump() override {
        return result + " := new " + lhs + ", " + rhs;
    }

    std::string generateCode() override { return "";

    }
};

class StopExecution : public Tac {
public:
    StopExecution()
        : Tac("stop", "", "", "") {}

    void dump() override {
        printf("%s", op.c_str());
    }
    std::string getDump() override {
        return op;
    }

    std::string generateCode() override {
        byte_code::Instruction* stop = new byte_code::Instruction();

        stop->type = byte_code::STOP;

        std::string code = stop->generateLine();
        delete stop;
        return code;
    }
};