#pragma once

#include <stdio.h>
#include <string>

class Tac {
    protected:
        std::string op, lhs, rhs, result;
    public:
        Tac(std::string _op = "", std::string _y = "", std::string _z = "", std::string _result = "")
            : op(_op), lhs(_y), rhs(_z), result(_result) {}
        virtual ~Tac() {}

        virtual void dump() = 0; 
        virtual std::string getDump() = 0;
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
};