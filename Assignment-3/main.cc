#include <iostream>
#include "parser.tab.hh"
#include "parsing_engine.hh"
#include "error_codes.hh"
#include "cfg.hh"
#include "interpreter.hh"
#include <iomanip> // For setw

extern Node *root;
extern FILE *yyin;
extern int yylineno;
extern int lexical_errors;
extern yy::parser::symbol_type yylex();

// ANSI escape codes for text color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_GRAY          "\033[90m"
#define ANSI_BOLD 		   "\033[1m"
#define ANSI_RESET         "\033[0m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Moved this to seperate file `error_codes.hh` for the semantic analysis to also include as well as main file
// enum errCodes
// {
// 	SUCCESS = 0,
// 	LEXICAL_ERROR = 1,
// 	SYNTAX_ERROR = 2,
// 	AST_ERROR = 3,
// 	SEMANTIC_ERROR = 4,
// 	SEGMENTATION_FAULT = 139
// };

int errCode = errCodes::SUCCESS;

// Handling Syntax Errors
void yy::parser::error(std::string const &err)
{
	if (!lexical_errors)
	{
		std::cerr << "Syntax errors found! See the logs below:" << std::endl;
		std::cerr << "\t@error at line " << yylineno << ". Cannot generate a syntax for this input:" << err.c_str() << std::endl;
		std::cerr << "End of syntax errors!" << std::endl;
		errCode = errCodes::SYNTAX_ERROR;
	}
}

void printStatus(const string& task, bool success) {
    cout << "- " << ANSI_BOLD << task << ANSI_RESET;

    int padding_width = 30 - static_cast<int>(task.length());
    cout << setw(padding_width) << "";
    if (success)
        cout << "\t\t" << ANSI_COLOR_GREEN << setw(2) << "✓" << ANSI_COLOR_RESET << endl; // Print success symbol
    else
        cout << "\t\t" << ANSI_COLOR_RED << setw(2) << "✗" << ANSI_COLOR_RESET << endl; // Print failure symbol
}

int main(int argc, char **argv)
{
	// Reads from file if a file name is passed as an argument. Otherwise, reads from stdin.
	bool if_interpreter = false;
	if (argc > 1)
	{
		if (argc > 2) if_interpreter = true;

		if (!(yyin = fopen(argv[1], "r")))
		{
			perror(argv[1]);
			return 1;
		}
	}
	//
	if (USE_LEX_ONLY)
		yylex();
	else
	{
		std::cout << "---------------- COMPILER FRONT END ----------------" << std::endl;
		yy::parser parser;

		bool parseSuccess = !parser.parse();

		if (lexical_errors) {
			errCode = errCodes::LEXICAL_ERROR;
			printStatus("LEXER FAILED", false);
		}
		else {
			printStatus("LEXER SUCCEEDED", true);
			if (parseSuccess)
			{
				printStatus("PARSER SUCCEEDED", true);
				try
				{
					
					int errCode_1 = errCodes::SUCCESS, errCode_2 = errCodes::SUCCESS, errCode_3 = errCodes::SUCCESS, errCode_4 = errCodes::SUCCESS;
					// root->print_tree();
					std::cout << ANSI_GRAY;
					root->generate_tree(true);
					std::cout << ANSI_RESET;

					// ---------------------------- SYNTAX ANALYSIS ------------------------------ //
					symbol_table::SymbolTable* sym_table = new symbol_table::SymbolTable();
					std::cout << ANSI_GRAY;
					errCode_1 = symbol_table::syntax_analysis(root, sym_table);
					// sym_table->printTable();
					std::cout << ANSI_RESET;
					
					// --------------------------- SEMNATIC ANALYSIS ----------------------------- //
					std::cout << ANSI_GRAY;
					errCode_2 = semantic_analysis::semantic_analysis(root, sym_table);
					std::cout << ANSI_RESET;

					if (errCode_1 != errCodes::SUCCESS || errCode_2 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::SEMANTIC_ERROR, "SEMANTIC ANALYZER");
					printStatus("SEMANTIC ANALYZER SUCCEEDED", true);
					
					// At this point, it is presumed that the provided source code is lexically, syntactically and semantically correct.
					// ---------------------------- IR GENERATION -------------------------------- //
					std::cout << ANSI_GRAY;
					errCode_3 = intermediate_representation::generateIR(root, sym_table);
					std::cout << ANSI_RESET;

					if (errCode_3 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::SEGMENTATION_FAULT, "IR GENERATION");
					printStatus("IR GENERATION SUCCEEDED", true);

					// ---------------------------- INTERPRETER --------------------------------- //
					// Files that works:
					// [X] - E.java
					// [X] - D1.java
					// [X] - D2.java
					// [X] - D3.java
					// [X] - C1.java
					// [X] - C2.java
					// [X] - B.java
					// [X] - A.java
					if (if_interpreter) {
						std::cout << ANSI_GRAY;
						interpreter::Interpreter* ip = new interpreter::Interpreter("byteCode.bc");
						errCode_4 = ip->interpret();
						std::cout << ANSI_RESET;

						delete ip;

						if (errCode_4 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::INTERPRETER_ERROR, "INTERPRETER");
						printStatus("INTERPRETER SUCCEEDED", true);
					} else {
						std::cout << "To run the interpreter, use the '-it' flag at the end." << std::endl;
					}
				}
				catch (ErrorCodeException& e) {
					errCode = e.errorCode;
					printStatus(e.name + " FAILED", false);
				}
				catch (...)
				{
					errCode = errCodes::AST_ERROR;
					printStatus("SEMANTIC ANALYZER FAILED", false);
				}
			} else {
				printStatus("PARSER FAILED", false);
			}
		}
	}

	std::cout << "ERROR CODE: " << errCode << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;
	return errCode;
}