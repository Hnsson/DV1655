#pragma once

#include <stdexcept>

enum errCodes
{
    SUCCESS = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    AST_ERROR = 3,
    SEMANTIC_ERROR = 4,
    SEGMENTATION_FAULT = 139
};

class ErrorCodeException : public std::runtime_error {
    public:
        int errorCode;

        ErrorCodeException(int code)
            :   std::runtime_error("An error occurred"), errorCode(code) {}
};
