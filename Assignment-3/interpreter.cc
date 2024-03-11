#include "interpreter.hh"

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

    errCodes  Interpreter::interpret() {
        if(!file_stream.is_open()) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return errCodes::INTERPRETER_ERROR;
        }

        std::string line;
        while(std::getline(file_stream, line)) {
            std::cout << line << std::endl;
        }

        return errCodes::SUCCESS;
    }
}