#include <iostream>
#include "parser.tab.hh"
// #include "symbol_table.hh"
#include "parsing_engine.hh"
#include "error_codes.hh"

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
		yy::parser parser;

		bool parseSuccess = !parser.parse();

		if (lexical_errors)
			errCode = errCodes::LEXICAL_ERROR;

		if (parseSuccess && !lexical_errors)
		{
			// printf("\nThe compiler successfuly generated a syntax tree for the given input! \n");

			// printf("\nPrint Tree:  \n");
			try
			{
				int errCode_1 = errCodes::SUCCESS, errCode_2 = errCodes::SUCCESS;
				// root->print_tree();
				root->generate_tree();

				// Syntax Analysis + Duplicate Identifier
				symbol_table::SymbolTable* sym_table = new symbol_table::SymbolTable();
				errCode_1 = symbol_table::syntax_analysis(root, sym_table);

				// sym_table->printTable(); 
				
				// Semantic Analysis
				errCode_2 = semantic_analysis::semantic_analysis(root, sym_table);

				if (errCode_1 != errCodes::SUCCESS || errCode_2 != errCodes::SUCCESS) throw ErrorCodeException(errCodes::SEMANTIC_ERROR);

				// At this point, it is presumed that the provided source code is lexically, syntactically and semantically correct.
				// std::cout << "YOOOOO CUTIE" << std::endl;
			}
			catch (ErrorCodeException& e) {
				errCode = e.errorCode;
			}
			catch (...)
			{
				errCode = errCodes::AST_ERROR;
			}
		}
	}
	std::cout << "ERROR CODE: " << errCode << std::endl;
	return errCode;
}