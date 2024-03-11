#pragma once

#include "byte_code.hh"
#include "error_codes.hh"
#include <iostream>
#include <fstream>

namespace interpreter {
    class Interpreter {
    public:
        Interpreter(std::string file_path);
        ~Interpreter();

        errCodes interpret();
    private:
        std::string file_path;
        std::ifstream file_stream;

        void init(std::string file_path);
    };
}