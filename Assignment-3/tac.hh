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
};

class Expression : Tac {
    Expression(std::string _op, std::string _y, std::string _z, std::string _result)
        : Tac(_op, _y, _z, _result) {}
    
    void dump() override {
        printf("%s := %s %s %s", result.c_str(), lhs.c_str(), op.c_str(), rhs.c_str());
    }
};

class MethodCall : Tac {
    MethodCall(std::string _f, std::string _N, std::string _result)
        : Tac("call", _f, _N, _result) {}

    void dump() override {
        printf("%s := %s %s, %s", result.c_str(), lhs.c_str(), op.c_str(), rhs.c_str());
    }
};

class Jump : Tac {
    Jump(std::string _label)
        : Tac("goto", "", "", _label) {}
    
    void dump() override {
        printf("%s %s", op.c_str(), result.c_str());
    }
};

class CondJump : Tac {
    CondJump(std::string _op, std::string _x, std::string _label)
        : Tac(_op, _x, "", _label) {}

    void dump() override {
        printf("%s %s goto %s", op.c_str(), result.c_str(), lhs.c_str());
    }
};

class ArrayAccess : Tac {
    ArrayAccess(std::string _array, std::string _index, std::string _result)
        : Tac("[]", _array, _index, _result) {}
    
    void dump() override {
        printf("%s := %s[%s]", result.c_str(), lhs.c_str(), rhs.c_str());
    }
};

class LengthAccess : Tac {
    LengthAccess(std::string _array, std::string _result)
        : Tac("length", _array, "", _result) {}

    void dump() override {
        printf("%s := %s %s", result.c_str(), op.c_str(), lhs.c_str());
    }
};

class Copy : Tac {
    Copy(std::string _y, std::string _result)
        : Tac("=", _y, "", _result) {}

    void dump() override {
        printf("%s := %s", result.c_str(), lhs.c_str());
    }
};

class Parameter : public Tac {
public:
    Parameter(std::string _y)
        : Tac("param", _y, "", "") {}

    void dump() override {
        printf("param %s", lhs.c_str());
    }
};

class Return : public Tac {
public:
    Return(std::string _y)
        : Tac("return", _y, "", "") {}

    void dump() override {
        printf("return %s", lhs.c_str());
    }
};

class NewObject : public Tac {
public:
    NewObject(std::string _type, std::string _result)
        : Tac("new", _type, "", _result) {}

    void dump() override {
        printf("%s := %s %s", result.c_str(), op.c_str(), lhs.c_str());
    }
};

class NewArray : public Tac {
public:
    NewArray(std::string _type, std::string _size, std::string _result)
        : Tac("new array", _type, _size, _result) {}

    void dump() override {
        printf("%s := new %s, %s", result.c_str(), lhs.c_str(), rhs.c_str());
    }
};