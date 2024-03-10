#pragma once

#include <iostream>
#include <string>

namespace byte_code {
    enum InstructionType {
        GOTO,
        ILOAD,
        ICONST,
        ISTORE,
        IADD,
        ISUB,
        IMUL,
        IDIV,
        ILT,
        IGT,
        IEQ,
        IAND,
        IOR,
        INOT,
        IF_FALSE_GOTO,
        INVOKEVIRTUAL,
        IRETURN,
        PRINT,
        STOP
    };

    bool is_number(const std::string& str);

    class Instruction {
    public:
        InstructionType type;
        std::string argument;

std::string generateLine() {
        switch (type) {
            case GOTO:
                return "goto " + argument;
            case ILOAD:
                return "iload " + argument;
            case ICONST:
                return "iconst " + argument;
            case ISTORE:
                return "istore " + argument;
            case IADD:
                return "iadd";
            case ISUB:
                return "isub";
            case IMUL:
                return "imul";
            case IDIV:
                return "idiv";
            case ILT:
                return "ilt";
            case IGT:
                return "igt";
            case IEQ:
                return "ieq";
            case IAND:
                return "iand";
            case IOR:
                return "ior";
            case INOT:
                return "inot";
            case IF_FALSE_GOTO:
                return "iffalse goto " + argument;
            case INVOKEVIRTUAL:
                return "invokevirtual " + argument;
            case IRETURN:
                return "ireturn";
            case PRINT:
                return "print";
            case STOP:
                return "stop";
            default:
                return "Unknown Instruction";
        }
    }
    };
}