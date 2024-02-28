#pragma once

#include <stdio.h>
#include <string>

class Tac {
    protected:
        std::string op, lhs, rhs, result;
    public:
        Tac(std::string _op = "", std::string _y = "", std::string _z = "", std::string _result = "")
            : op(_op), lhs(_y), rhs(_z), result(_result) {}

        void dump() { printf("%s := %s %s %s", result.c_str(), lhs.c_str(), op.c_str(), rhs.c_str()); }
};

class Expression : Tac {
    Expression(std::string _op, std::string _y, std::string _z, std::string _result)
        : Tac(_op, _y, _z, _result) {}
};

class MethodCall : Tac {
    MethodCall(std::string _f, std::string _N, std::string _result)
        : Tac("call", _f, _N, _result) {}
};

class Jump : Tac {
    Jump(std::string _label)
        : Tac("goto", "", "", _label) {}
};

class CondJump : Tac {
    CondJump(std::string _op, std::string _x, std::string _label)
        : Tac(_op, _x, "", _label) {}
};