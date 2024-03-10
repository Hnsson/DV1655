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

    std::string generateCode() override { return "";

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
        if (lhs == "true") _lhs->argument = "1";
        else if (lhs == "false") _lhs->argument = "0";
        else {
            if (byte_code::is_number(lhs)) {
                // Load constant
                _lhs->type = byte_code::ICONST;
            } else {
                // Load identifer
                _lhs->type = byte_code::ILOAD;
            }
        }
        _lhs->argument = lhs;

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
            // Paramters, need to push to stack `rhs` amount.
        }

        // Create final instruction
        _result->type = byte_code::ISTORE;
        _result->argument = result;

        return _invoke->generateLine() + "\n\t" + _result->generateLine();
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

    std::string generateCode() override { return "";

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
        return "_";
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

        if (lhs == "true") _lhs->argument = "1";
        else if (lhs == "false") _lhs->argument = "0";
        else {
            if (byte_code::is_number(lhs)) {
                // Load constant
                _lhs->type = byte_code::ICONST;
            } else {
                // Load identifer
                _lhs->type = byte_code::ILOAD;
            }
        }
        _lhs->argument = lhs;

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

    std::string generateCode() override { return "";

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

    std::string generateCode() override { return "";

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