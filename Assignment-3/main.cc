#include <iostream>
#include "parser.tab.hh"
#include "parsing_engine.hh"
#include "error_codes.hh"
#include "cfg.hh"
#include "interpreter.hh"

extern Node *root;
extern FILE *yyin;
extern int yylineno;
extern int lexical_errors;
extern yy::parser::symbol_type yylex();


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

int main(int argc, char **argv)
{
	// Reads from file if a file name is passed as an argument. Otherwise, reads from stdin.
	if (argc > 1)
	{
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
			std::cout << "- LEXER FAILED\t\t❌" << std::endl;
		}
		else std::cout << "- LEXER SUCCEEDED\t\t✅" << std::endl;

		if (parseSuccess && !lexical_errors)
		{
			std::cout << "- PARSER SUCCEEDED\t\t✅" << std::endl;
			// printf("\nThe compiler successfuly generated a syntax tree for the given input! \n");

			// printf("\nPrint Tree:  \n");
			try
			{
				
				int errCode_1 = errCodes::SUCCESS, errCode_2 = errCodes::SUCCESS, errCode_3 = errCodes::SUCCESS, errCode_4 = errCodes::SUCCESS;
				// root->print_tree();
				root->generate_tree(true);

				// ---------------------------- SYNTAX ANALYSIS ------------------------------ //
				symbol_table::SymbolTable* sym_table = new symbol_table::SymbolTable();
				errCode_1 = symbol_table::syntax_analysis(root, sym_table);
				// sym_table->printTable(); 
				
				// --------------------------- SEMNATIC ANALYSIS ----------------------------- //
				errCode_2 = semantic_analysis::semantic_analysis(root, sym_table);

				if (errCode_1 != errCodes::SUCCESS || errCode_2 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::SEMANTIC_ERROR, "SEMANTIC ANALYZER");
				std::cout << "- SEMANTIC ANALYZER SUCCEEDED\t✅" << std::endl;
				
				// At this point, it is presumed that the provided source code is lexically, syntactically and semantically correct.
				// ---------------------------- IR GENERATION -------------------------------- //
				errCode_3 = intermediate_representation::generateIR(root, sym_table);

				if (errCode_3 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::SEGMENTATION_FAULT, "IR GENERATION");
				std::cout << "- IR GENERATION SUCCEEDED\t✅" << std::endl;

				// ---------------------------- INTERPRETER --------------------------------- //
				// Files that works:
				// [X] - E.java
				// [X] - D1.java
				// [X] - D2.java
				// [X] - D3.java
				// [X] - C1.java
				// [X] - C2.java
				// [ ] - B.java	 - Prints 3628800, 5, 1 --> wrong, should be 120, 55, 1
				// [ ] - A.java	 - Prints 10, 1, 1, 4, 4 --> wrong, should be 10, 1, 2, 3, 4
				interpreter::Interpreter* ip = new interpreter::Interpreter("byteCode.bc");
				errCode_4 = ip->interpret();

				delete ip;

				if (errCode_4 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::INTERPRETER_ERROR, "INTERPRETER");
				std::cout << "- INTERPRETER SUCCEEDED\t\t✅" << std::endl;
			}
			catch (ErrorCodeException& e) {
				errCode = e.errorCode;
				std::cout << "- " << e.name << " FAILED\t\t❌" << std::endl;
			}
			catch (...)
			{
				errCode = errCodes::AST_ERROR;
				std::cout << "- SEMANTIC ANALYZER FAILED\t\t❌" << std::endl;
			}
		} else {
			std::cout << "- PARSER FAILED\t\t❌" << std::endl;
		}
	}

	std::cout << "ERROR CODE: " << errCode << std::endl;
	std::cout << "----------------------------------------------------" << std::endl;
	return errCode;
}