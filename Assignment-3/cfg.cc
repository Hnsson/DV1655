#include "cfg.hh"

namespace intermediate_representation {
    
    std::string generateTempId() {
        static int _count = 0;
        return "_t" + std::to_string(_count++);
    }

    std::string generateBlockId() {
        static int count = 0;
        return "block_" + std::to_string(count++);
    }




    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table, BBlock* current_block) {
        if(node->type == "Program") {
            if(!node->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = node->children.begin(); i != node->children.end(); ++i) {
                    // Handle the classes
                    sym_table->enterScope("", NULL);
                    traverseTreeIR(*i, sym_table, current_block);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Main Class") {
            sym_table->enterScope("", NULL);
            for(auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }
            sym_table->exitScope();
        }
        if(node->type == "Class") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    traverseTreeIR(*i, sym_table, current_block);
                }
            }
        }
        // ------------- MAYBE HAVE THIS IN THE SYMBOL TABLE TRAVERSAL ------------- //
        if(node->type == "Var declarations") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    std::string varDec = traverseTreeIR((*i), sym_table, current_block);
                }
            }
        }
        if(node->type == "Var declaration") {
            return node->children.front()->value;
        }
        // -------------------------------------------------------------------------- //
        if(node->type == "MethodDeclaration") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    Node* type = *(*i)->children.begin();

                    sym_table->enterScope("", NULL);
                    traverseTreeIR(*i, sym_table, current_block);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Method") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }

            // Maybe add return logic here if he doesn't like it being on the return statement (which is more reasonable ;) )
        }
        if(node->type == "MethodContent") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }
        }
        if(node->type == "Content") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }
        }
        if(node->type == "Return") {
            std::string returnType = traverseTreeIR(node->children.back(), sym_table, current_block);
            std::string methodType = ((symbol_table::Method*)sym_table->getCurrentScope()->getScopeContext())->getType();
        }
        if(node->type == "Statements") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }
        }
        if(node->type == "Statement") {
            traverseTreeIR(node->children.front(), sym_table, current_block);
        }

        // Now at level of statement-/expression components
        // Will first perform on statements

        if(node->type == "If-Expression-Statement") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }

            // std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
        }
        if(node->type == "If/Else-Expression-Statement") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }

            // std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
        }
        if(node->type == "While-Statement") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table, current_block);
            }

            // std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
        }
        if(node->type == "SysPrintLn") {
            // std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
        }
        if(node->type == "Identifier assign") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);

            std::cout << lhsType << " := " << rhsType << std::endl;
        }
        if(node->type == "ArrayPositionAssignOp") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string posType = traverseTreeIR(*(++node->children.begin()), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);
        }



        // Complex Operators
        if(node->type == "LogicalAndExpression" || node->type == "LogicalOrExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);

            return "boolean";
        }
        if(node->type == "LesserThanExpression" || node->type == "GreaterThanExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);

            return "boolean";
        }
        // EqualExpression
        if(node->type == "EqualExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);

            return "boolean";
        }


        // Mathematical Operators
        if(node->type == "AddExpression" || node->type == "SubExpression" || 
        node->type == "MultExpression" || node->type == "DivExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);

            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "int"));
            Tac* ExpressionInstruction = new Expression("*", lhsType, rhsType, name);
            ExpressionInstruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(ExpressionInstruction);

            return name; // The result of these operations is also an 'int'
        }


        // Programmer Operators
        if (node->type == "Array access") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, current_block);

            return "int";
        }
        if (node->type == "Expression" && node->value == "length") {
            // .length property for arrays
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table, current_block);
            std::string name = generateTempId();

            sym_table->put(name, new symbol_table::Variable(name, "int"));
            Tac* lengthInstruction = new LengthAccess(lhsName, name);
            lengthInstruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(lengthInstruction);

            return name;
        }
        if(node->type == "ExpressionCallMethod") {
            // [X] - Check if using this as expression then you need to check if this is called from a class (lhs)
            // [X] - Check if the identifier function exists in the expression above and has correct return type (mhs)
            // [X] - (IF EXISTS) Check if the number of arguments and argument types are correct to the function from mhs (rhs)
            // First step
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table, current_block);
            // Because the left hand side is always a class instance, look at the root for classes with that name
            auto lhs_class = (symbol_table::Class*)sym_table->getRootScope()->lookup(lhsName);

            // Second step
            std::string mhsName = (*(++node->children.begin()))->value;
            // symbol_table::Method* _method = ((symbol_table::Class*)sym_table->findSymbol(lhsName))->lookupMethod(mhsName);
            symbol_table::Method* _method = lhs_class->lookupMethod(mhsName);

            // Third step if exists, probably add a parameter stack and the returned value N is what the function should pop x amount of from param stack
            auto it = node->children.begin();
            std::advance(it, 2);
            if (it != node->children.end()) {
                // Add params
            } else {
                // Dont add params
            }

            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "method"));
            Tac* callMethodInstruction = new MethodCall(mhsName, std::to_string(_method->getParamSize()), name);
            callMethodInstruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(callMethodInstruction);

            return name;
        }


        // Lowest level components / instanciators
        if(node->type == "Identifier") {
            auto identifier = sym_table->findSymbol(node->value);
            return identifier->getName();
        }
        if(node->type == "this") {
            auto identifier = sym_table->findSymbol(node->value);
            return identifier->getType();
        }
        if(node->type == "int") {
            return node->value;
        }
        if(node->type == "boolean") {
            return node->value;
        }
        if(node->type == "NewArray" && node->value == "int") {
            // New array
            std::string lhsType = node->value;
            std::string lhsLen = node->children.front()->value;
            std::string name = generateTempId();
            // Think below is necessary due to the node above need to find the properties of the sub-expression
            sym_table->put(name, new symbol_table::Variable(name, "int[]"));
            Tac* instruction = new NewArray(lhsType, lhsLen, name);
            instruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(instruction);
            // Return name to find it again, spliting it up to this:
            // x := new int[Y]          (1)
            // _t0 = new int[Y]         (2)
            // x := _t0                 (3)
            // Want to use copy so need the `new int[Y]` in a singular instruction.
            return name;
        }
        if(node->type == "NewIstance") {
            auto instance = sym_table->findSymbol(node->value);

            return instance->getName();
        }
        if(node->type == "NotOperation") {
            std::string type = traverseTreeIR(node->children.front(), sym_table, current_block);

            return type;
        }

        return node->type;
    }

    errCodes generateCFG(BBlock* start_block) {
        // for(Tac* instructions : start_block->tacInstructions) {
        //     instructions->dump();
        // }

        std::cout << "START BLOCK INSTRUCTIONS: " << start_block->tacInstructions.size() << std::endl;
        return errCodes::SUCCESS;
    }


    errCodes generateIR(Node* root, symbol_table::SymbolTable* sym_table) {
        try {
            sym_table->resetTable();
            BBlock* current_block = new BBlock();
            traverseTreeIR(root, sym_table, current_block);

            // generateCFG(current_block);
            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEGMENTATION_FAULT; // Return a semantic error code if an exception was caught
        }
    }
}