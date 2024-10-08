#pragma once

#include <stdexcept>

enum errCodes
{
    SUCCESS = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    AST_ERROR = 3,
    SEMANTIC_ERROR = 4,
    GENERATE_IR = 5,
    GENERATE_BYTE_CODE = 6,
    INTERPRETER_ERROR = 7,
    SEGMENTATION_FAULT = 139
};

class ErrorCodeException : public std::runtime_error {
    public:
        int errorCode;
        std::string name;

        ErrorCodeException(int code, std::string name = "")
            :   std::runtime_error("An error occurred"), errorCode(code), name(name) {}
};
