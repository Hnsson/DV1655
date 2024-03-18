#include "cfg.hh"

namespace intermediate_representation {
    
    std::string generateTempId() { // Generate a temporary id
        static int _count = 0;
        return "_t" + std::to_string(_count++);
    }

    std::string generateBlockId() { // Generate a block id
        static int count = 0;
        return "block_" + std::to_string(count++);
    }


    bool isLastBlock(BBlock* block, BBlock* continueBlock) {
        // A block is considered 'last' if its exits lead to a known continuation block or are null,
        // indicating no further control structure processing within the current context.
        return (!block->trueExit || block->trueExit == continueBlock) &&
            (!block->falseExit || block->falseExit == continueBlock);
    }

    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table, BBlock* prev_block = nullptr) {
        if(node->type == "Program") {
            if(!node->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = node->children.begin(); i != node->children.end(); ++i) {
                    // Handle the classes
                    sym_table->enterScope("", NULL);
                    traverseTreeIR(*i, sym_table);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Main Class") {
            std::string _main_method_name = node->value + "." + "Main";
            BBlock* _main_method_block = new BBlock(_main_method_name);

            sym_table->enterScope("", NULL);
            current_block = _main_method_block;

            // Add the main argument
            Tac* main_arg = new Argument(node->children.front()->value);
            current_block->tacInstructions.push_back(main_arg);
            for(auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
            sym_table->exitScope();
            // Add the main method block to the list of method blocks
            method_blocks.push_back(_main_method_block);
        }
        if(node->type == "Class") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    traverseTreeIR(*i, sym_table);
                }
            }
        }
        if(node->type == "Var declarations") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    std::string varDec = traverseTreeIR((*i), sym_table);
                }
            }
        }
        if(node->type == "Var declaration") {
            // Front for type, back for name
            return node->children.back()->value;
        }
        if(node->type == "MethodDeclaration") {
            if(!node->children.empty()) {
                // Traverse all the children of the method declaration (i.e. method blocks)
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    // Create a new block for the method with name as the class name + method name
                    std::string _method_name = sym_table->getCurrentScope()->getScopeContext()->getName() + "." + (*i)->value;
                    BBlock* _method_block = new BBlock(_method_name);

                    sym_table->enterScope("", NULL);
                    // Set current block to the method block and traverse the method content
                    current_block = _method_block;
                    traverseTreeIR(*i, sym_table);
                    sym_table->exitScope();
                    // Add the method block to the list of method blocks
                    method_blocks.push_back(_method_block);
                }
            }
        }
        if(node->type == "Method") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Method parameters") {
            for (auto i = node->children.rbegin(); i != node->children.rend(); i++) {
                std::string param_name = traverseTreeIR(*i, sym_table);
                // Add the parameter to the method block
                Tac* argInstruction = new Argument(param_name);
                current_block->tacInstructions.push_back(argInstruction);
            }
        }
        if(node->type == "MethodContent") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Content") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Return") {
            std::string returnType = traverseTreeIR(node->children.back(), sym_table);
            // Return(std::string _result) -> _result is the result of the return statement
            Tac* returnInstruction = new Return(returnType);
            current_block->tacInstructions.push_back(returnInstruction);
        }
        if(node->type == "Statements") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
        }
        if(node->type == "Statement") {
            traverseTreeIR(node->children.front(), sym_table);
        }

        // Now at level of statement-/expression components
        // Will first perform on statements

        if(node->type == "If-Expression-Statement") {
            // If-Expression-Statement
            // True block path
            std::string trueBlockName = generateBlockId();
            BBlock* true_block = new BBlock(trueBlockName);
            // False block path
            std::string continueBlockName = generateBlockId();
            BBlock* continue_block = new BBlock(continueBlockName);

            // Add the condition to the current block and set the true and false exits
            current_block->trueExit = true_block;
            current_block->falseExit = continue_block;

            auto it = node->children.begin();
            std::string cond = traverseTreeIR(*it, sym_table);
            Tac* conditionalInstruction = new CondJump(continueBlockName, cond);
            current_block->condition = conditionalInstruction;
            
            // Set the current block to the true block and traverse the true block content
            std::advance(it, 1);
            current_block = true_block;
            traverseTreeIR(*it, sym_table);
            // Set the true exit of the true block to the continue block
            current_block->trueExit = continue_block;

            // Set the current block to the continue block
            current_block = continue_block;
        }
        if(node->type == "If/Else-Expression-Statement") {
            // If/Else-Expression-Statement
            // True block path
            std::string trueBlockName = generateBlockId();
            BBlock* true_block = new BBlock(trueBlockName);
            // False block path
            std::string falseBlockName = generateBlockId();
            BBlock* false_block = new BBlock(falseBlockName);
            // Continue block path
            std::string continueBlockName = generateBlockId();
            BBlock* continue_block = new BBlock(continueBlockName);

            // Add the condition to the current block and set the true and false exits
            current_block->trueExit = true_block;
            current_block->falseExit = false_block;

            auto it = node->children.begin();
            std::string cond = traverseTreeIR(*it, sym_table);
            Tac* conditionalInstruction = new CondJump(falseBlockName, cond);
            current_block->condition = conditionalInstruction;
            
            // Set the current block to the true block and traverse the true block content
            std::advance(it, 1);
            current_block = true_block;
            traverseTreeIR(*it, sym_table);
            current_block->trueExit = continue_block;
            
            // Set the current block to the false block and traverse the false block content
            std::advance(it, 1);
            current_block = false_block;
            traverseTreeIR(*it, sym_table);
            current_block->trueExit = continue_block;

            // Set the current block to the continue block
            current_block = continue_block;
        }
        if(node->type == "While-Statement") {
            // While-Statement
            // Header block path containing the condition goto paths
            std::string headerBlockName = generateBlockId();
            BBlock* header_block = new BBlock(headerBlockName);
            // True block path
            std::string trueBlockName = generateBlockId();
            BBlock* true_block = new BBlock(trueBlockName);
            // Continue block path
            std::string continueBlockName = generateBlockId();
            BBlock* continue_block = new BBlock(continueBlockName);

            // Add the condition to the header block and set the true and false exits
            auto it = node->children.begin();
            std::string cond = traverseTreeIR(*it, sym_table, header_block);
            current_block->trueExit = header_block;

            std::string temp_cond = generateTempId();
            Tac* conditionalInstruction = new CondJump(continueBlockName, cond);
            header_block->condition = conditionalInstruction;

            // Set the current block to the true block and traverse the true block content
            std::advance(it, 1);
            current_block = true_block;
            traverseTreeIR(*it, sym_table);
            // Set the true exit of the true block to the continue block
            current_block->trueExit = header_block;
            // Set the false exit of the true block (header block) to the continue block
            header_block->trueExit = true_block;
            header_block->falseExit = continue_block;

            // Set the current block to the continue block
            current_block = continue_block;
        }
        if(node->type == "SysPrintLn") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            // Print(std::string _result)
            Tac* printInstruction = new Print(lhsType);
            current_block->tacInstructions.push_back(printInstruction);
        }
        if(node->type == "Identifier assign") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);
            // Copy(std::string _rhs, std::string _lhs)
            Tac* copyInstruction = new Copy(rhsType, lhsType);
            current_block->tacInstructions.push_back(copyInstruction);
            return lhsType;
        }
        if(node->type == "ArrayPositionAssignOp") {
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table);
            std::string posName = traverseTreeIR(*(++node->children.begin()), sym_table);
            std::string rhsName = traverseTreeIR(node->children.back(), sym_table);
            // ArrayAssign(std::string _array, std::string _index, std::string _result)
            Tac* arrayAssignInstruction = new ArrayAssign(lhsName, posName, rhsName);
            current_block->tacInstructions.push_back(arrayAssignInstruction);
        }



        // Complex Operators
        if(node->type == "LogicalAndExpression" || node->type == "LogicalOrExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, prev_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, prev_block);
            // Generate a new temp variable for the result of the logical expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "boolean"));
            // Set instruction based on the type of logical expression
            Tac* instruction;
            if (node->type == "LogicalAndExpression") {
                instruction = new Expression("&&", lhsType, rhsType, name);
            } else {
                instruction = new Expression("||", lhsType, rhsType, name);
            }

            // If prev_block is not null, add the instruction to the prev_block (its a header block), else add it to the current block
            if(prev_block != nullptr) {
                prev_block->tacInstructions.push_back(instruction);
            } else {
                current_block->tacInstructions.push_back(instruction);
            }

            return name;
        }
        if(node->type == "LesserThanExpression" || node->type == "GreaterThanExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, prev_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, prev_block);
            // Generate a new temp variable for the result of the logical expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "boolean"));
            // Set instruction based on the type of logical expression
            Tac* instruction;
            if (node->type == "LesserThanExpression") {
                instruction = new Expression("<", lhsType, rhsType, name);
            } else {
                instruction = new Expression(">", lhsType, rhsType, name);
            }

            // If prev_block is not null, add the instruction to the prev_block (its a header block), else add it to the current block
            if(prev_block != nullptr) {
                prev_block->tacInstructions.push_back(instruction);
            } else {
                current_block->tacInstructions.push_back(instruction);
            }

            return name;
        }
        // EqualExpression
        if(node->type == "EqualExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table, prev_block);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table, prev_block);
            // Generate a new temp variable for the result of the logical expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "boolean"));
            Tac* instruction = new Expression("==", lhsType, rhsType, name);

            // If prev_block is not null, add the instruction to the prev_block (its a header block), else add it to the current block
            if(prev_block != nullptr) {
                prev_block->tacInstructions.push_back(instruction);
            } else {
                current_block->tacInstructions.push_back(instruction);
            }

            return name;
        }


        // Arithmetic Operators
        if(node->type == "AddExpression" || node->type == "SubExpression" || 
        node->type == "MultExpression" || node->type == "DivExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);
            // Generate a new temp variable for the result of the expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "int"));
            // Set instruction based on the type of expression
            Tac* ExpressionInstruction;
            if (node->type == "AddExpression") ExpressionInstruction = new Expression("+", lhsType, rhsType, name);
            if (node->type == "SubExpression") ExpressionInstruction = new Expression("-", lhsType, rhsType, name);
            if (node->type == "MultExpression") ExpressionInstruction = new Expression("*", lhsType, rhsType, name);
            if (node->type == "DivExpression") ExpressionInstruction = new Expression("/", lhsType, rhsType, name);

            current_block->tacInstructions.push_back(ExpressionInstruction);

            return name; // The result of these operations is also an 'int'
        }


        // Unary Operators
        if (node->type == "Array access") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);
            // Generate a new temp variable for the result of the expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "int"));
            // ArrayAccess(std::string _array, std::string _index, std::string _result)
            Tac* arrayAccessInstruction = new ArrayAccess(lhsType, rhsType, name);
            current_block->tacInstructions.push_back(arrayAccessInstruction);

            return name;
        }
        if (node->type == "Expression" && node->value == "length") {
            // length expression
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table);
            // Generate a new temp variable for the result of the expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "int"));
            // LengthAccess(std::string _array, std::string _result)
            Tac* lengthInstruction = new LengthAccess(lhsName, name);

            current_block->tacInstructions.push_back(lengthInstruction);

            return name;
        }
        if(node->type == "ExpressionCallMethod") {
            // [X] - Check if using this as expression then you need to check if this is called from a class (lhs)
            // [X] - Check if the identifier function exists in the expression above and has correct return type (mhs)
            // [X] - (IF EXISTS) Check if the number of arguments and argument types are correct to the function from mhs (rhs)
            // First step
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table);

            symbol_table::SymbolRecord* lhs = sym_table->getRootScope()->lookup(lhsName);
            if (lhs) {
                lhsName = lhs->getType();
            }
            // Second step
            std::string mhsName = (*(++node->children.begin()))->value;
            // Third step if exists, probably add a parameter stack and the returned value N is what the function should pop x amount of from param stack
            int _paramSize = 0;
            auto it = node->children.begin();
            std::advance(it, 2);
            if (it != node->children.end()) {
                _paramSize = (*it)->children.size();
                // Add params ...
                for (auto i = (*it)->children.begin(); i != (*it)->children.end(); i++) {
                    std::string lhsName = traverseTreeIR(*i, sym_table);
                    Tac* input_param = new Parameter(lhsName);
                    current_block->tacInstructions.push_back(input_param);
                }
            } else {
                // Dont add params
            }
            // Generate a new temp variable for the result of the expression
            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "method"));
            // MethodCall(std::string _lhs, std::string _rhs, std::string _result)
            Tac* callMethodInstruction = new MethodCall(lhsName + "." + mhsName, std::to_string(_paramSize), name);
            current_block->tacInstructions.push_back(callMethodInstruction);

            return name;
        }


        // Base types
        if(node->type == "Identifier") {
            // Identifier
            auto identifier = sym_table->findSymbol(node->value);
            return identifier->getName();
        }
        if(node->type == "this") {
            // this
            auto identifier = sym_table->findSymbol(node->value);
            return identifier->getType();
        }
        if(node->type == "int") {
            // int
            return node->value;
        }
        if(node->type == "boolean") {
            // boolean
            return node->value;
        }
        if(node->type == "NewArray" && node->value == "int") {
            // New array
            std::string lhsType = node->value;
            std::string lhsLen = node->children.front()->value;
            std::string name = generateTempId();
            // Add the new array to the current block
            sym_table->put(name, new symbol_table::Variable(name, "int[]"));
            Tac* instruction = new NewArray(lhsType, lhsLen, name);
            current_block->tacInstructions.push_back(instruction);
            // Return name to find it again, spliting it up to this:
            // x := new int[Y]          (1)
            // _t0 = new int[Y]         (2)
            // x := _t0                 (3)
            // Want to use copy so need the `new int[Y]` in a singular instruction.
            return name;
        }
        if(node->type == "NewIstance") {
            // ------ USE THESE IF YOU WANT TO CREATE TEMPORY VARIABLE WITH NEW INSTANCE ------
            // std::string name = generateTempId();
            // sym_table->put(name, new symbol_table::Variable(instance, "NewInstance"));
            // Tac* newObjectInstruction = new NewObject(instance, name);
            // current_block->tacInstructions.push_back(newObjectInstruction);

            // return name;
            // ---------------------------------------------------------------------------------

            std::string instance = node->value;

            return instance;
        }
        if(node->type == "NotOperation") {
            std::string type = traverseTreeIR(node->children.front(), sym_table);
            std::string name = generateTempId();

            // Add the new variable to the current block
            sym_table->put(name, new symbol_table::Variable(name, "boolean"));
            // UnaryExpression(std::string _op, std::string _type, std::string _result)
            Tac* newUnaryExpressionInstruction = new UnaryExpression("!", type, name);
            current_block->tacInstructions.push_back(newUnaryExpressionInstruction);

            return name;
        }

        return node->type;
    }

    void generateCodeContent(BBlock* block, ofstream& outStream, unordered_set<BBlock*>& visited, bool print) {
        if (block == nullptr || visited.find(block) != visited.end()) {
            return; // Avoid printing the same block twice or null blocks
        }

        visited.insert(block); // Mark the current block as visited
        outStream << block->name << ":" << "\n";

        for(Tac* instruction : block->tacInstructions) {
            // std::cout << "DUMP: " << instruction->getDump() << " : " << instruction->generateCode() << std::endl;
            outStream << "\t" << instruction->generateCode() << "\n";
        }

        // Add condition after instructions
        if(block->condition) outStream << "\n\t" << block->condition->generateCode();
        // This will add a goto to every block if they have a trueExit, but if it has a condition, provide that first before this, which is the statement above
        // if (block->trueExit) {
        if (block->trueExit && print) {
            Tac* jump = new Jump(block->trueExit->name);
            outStream << "\n\t" << jump->generateCode();
        }

        if (block->name.find(".Main") != std::string::npos) {
            Tac* exit = new StopExecution();
            outStream << "\t" << exit->generateCode() << "\n";
        }
        outStream << "\n";

        if (block->trueExit != nullptr) {
            generateCodeContent(block->trueExit, outStream, visited, true);
        }
        if (block->falseExit != nullptr) {
            generateCodeContent(block->falseExit, outStream, visited, true);
        }
    }


    errCodes generateCode() {
        ofstream outStream("byteCode.bc");
        if (!outStream.is_open()) {
            cerr << "Error opening file byteCode.bc" << endl;
            return errCodes::GENERATE_BYTE_CODE;
        }

        unordered_set<BBlock*> visited; // To keep track of visited blocks
        for(BBlock* methodEntry : method_blocks){
            generateCodeContent(methodEntry, outStream, visited, false);
        }

        outStream.close();

        return errCodes::SUCCESS;
    }

    void generateCFGContent(BBlock* block, ofstream& outStream, unordered_set<BBlock*>& visited) {
        if (block == nullptr || visited.find(block) != visited.end()) {
            return; // Avoid printing the same block twice or null blocks
        }

        visited.insert(block); // Mark the current block as visited
        outStream << "    \"" << block->name << "\" [shape=box label=\"" << block->name << "\\n";

        for(Tac* instruction : block->tacInstructions) {
            outStream << instruction->getDump();
            outStream << "\\n";
        }

        // Add condition after instructions
        if(block->condition) outStream << "\\n" << block->condition->getDump();
        // This will add a goto to every block if they have a trueExit, but if it has a condition, provide that first before this, which is the statement above
        if (block->trueExit) {
            Tac* jump = new Jump(block->trueExit->name);
            outStream << "\\n" << jump->getDump();
        }

        outStream << "\"];\n";

        if (block->trueExit != nullptr) {
            outStream << "    \"" << block->name << "\" -> \"" << block->trueExit->name << "\" [label=\"true\" color=\"#7dff90\"];\n";
            generateCFGContent(block->trueExit, outStream, visited);
        }
        if (block->falseExit != nullptr) {
            outStream << "    \"" << block->name << "\" -> \"" << block->falseExit->name << "\" [label=\"false\" color=\"#ff7d7d\"];\n";
            generateCFGContent(block->falseExit, outStream, visited);
        }
    }

    errCodes generateCFG() {
        ofstream outStream("cfg.dot");
        if (!outStream.is_open()) {
            cerr << "Error opening file cfg.dot" << endl;
            return errCodes::GENERATE_IR;
        }

        outStream << "digraph CFG {\n";
        unordered_set<BBlock*> visited; // To keep track of visited blocks
        for(BBlock* methodEntry : method_blocks){
            generateCFGContent(methodEntry, outStream, visited);
        }
        outStream << "}\n";
        outStream.close();

        cout << "Built CFG at cfg.dot. Use 'dot -Tpdf cfg.dot -o cfg.pdf' to generate the PDF version." << endl;
        return errCodes::SUCCESS;
    }

    errCodes generateIR(Node* root, symbol_table::SymbolTable* sym_table) {
        try {
            errCodes cfg_error = errCodes::SUCCESS, cg_error = errCodes::SUCCESS;

            // Reset table for scope tracking
            sym_table->resetTable();
            BBlock* _top_block = new BBlock("Program");
            current_block = _top_block;
            traverseTreeIR(root, sym_table);

            // Generate context free grammar
            cfg_error = generateCFG();
            // Generate byte-code
            cg_error = generateCode();


            if (cfg_error != errCodes::SUCCESS || cg_error != errCodes::SUCCESS) throw std::runtime_error("");

            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEGMENTATION_FAULT; // Return a semantic error code if an exception was caught
        }
    }
}