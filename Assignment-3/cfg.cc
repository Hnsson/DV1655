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




    std::string traverseTreeIR(Node* node, symbol_table::SymbolTable* sym_table) {
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

            // Tac* main_param = new Parameter(node->children.front()->value);
            Tac* main_param = new Copy("Param", node->children.front()->value);
            current_block->tacInstructions.push_back(main_param);
            for(auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeIR(*i, sym_table);
            }
            sym_table->exitScope();

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
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    std::string _method_name = sym_table->getCurrentScope()->getScopeContext()->getName() + "." + (*i)->value;
                    BBlock* _method_block = new BBlock(_method_name);

                    sym_table->enterScope("", NULL);
                    current_block = _method_block;
                    traverseTreeIR(*i, sym_table);
                    sym_table->exitScope();

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
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                std::string param_name = traverseTreeIR(*i, sym_table);
                // Tac* paramInstruction = new Parameter(param_name);
                Tac* paramInstruction = new Copy("Param", param_name);
                current_block->tacInstructions.push_back(paramInstruction);
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
            std::string trueBlockName = generateBlockId();
            BBlock* true_block = new BBlock(trueBlockName);
            std::string continueBlockName = generateBlockId();
            BBlock* continue_block = new BBlock(continueBlockName);

            Tac* uncondJump = new Jump(continueBlockName);

            current_block->trueExit = true_block;
            current_block->falseExit = continue_block;

            auto it = node->children.begin();
            std::string cond = traverseTreeIR(*it, sym_table);
            Tac* conditionalInstruction = new CondJump(continueBlockName, cond);
            current_block->condition = conditionalInstruction;

            std::advance(it, 1);
            current_block = true_block;
            traverseTreeIR(*it, sym_table);
            // true_block->trueExit = continue_block;
            true_block->tacInstructions.push_back(uncondJump);

            current_block = continue_block;
        }
        if(node->type == "If/Else-Expression-Statement") {
            std::string trueBlockName = generateBlockId();
            BBlock* true_block = new BBlock(trueBlockName);
            std::string falseBlockName = generateBlockId();
            BBlock* false_block = new BBlock(falseBlockName);
            std::string continueBlockName = generateBlockId();
            BBlock* continue_block = new BBlock(continueBlockName);

            Tac* uncondJump = new Jump(continueBlockName);

            current_block->trueExit = true_block;
            current_block->falseExit = false_block;

            auto it = node->children.begin();
            std::string cond = traverseTreeIR(*it, sym_table);
            Tac* conditionalInstruction = new CondJump(falseBlockName, cond);
            current_block->condition = conditionalInstruction;
            
            std::advance(it, 1);
            current_block = true_block;
            traverseTreeIR(*it, sym_table);
            true_block->trueExit = continue_block;
            true_block->tacInstructions.push_back(uncondJump);
            

            std::advance(it, 1);
            current_block = false_block;
            traverseTreeIR(*it, sym_table);
            false_block->trueExit = continue_block;
            false_block->tacInstructions.push_back(uncondJump);

            current_block = continue_block;
        }
        if(node->type == "While-Statement") {
            std::string trueBlockName = generateBlockId();
            BBlock* true_block = new BBlock(trueBlockName);
            std::string continueBlockName = generateBlockId();
            BBlock* continue_block = new BBlock(continueBlockName);


            current_block->trueExit = true_block;
            current_block->falseExit = continue_block;

            auto it = node->children.begin();
            std::string cond = traverseTreeIR(*it, sym_table);
            Tac* conditionalInstruction = new CondJump(continueBlockName, cond);
            current_block->condition = conditionalInstruction;

            std::advance(it, 1);
            current_block = true_block;
            traverseTreeIR(*it, sym_table);

            std::string temp_cond = generateTempId();
            Tac* negate_cond = new UnaryExpression("!", cond, temp_cond);
            sym_table->put(temp_cond, new symbol_table::Variable(temp_cond, "boolean"));
            true_block->tacInstructions.push_back(negate_cond);

            Tac* WhileConditionalInstruction = new CondJump(trueBlockName, temp_cond);
            true_block->tacInstructions.push_back(WhileConditionalInstruction);

            current_block = continue_block;
        }
        if(node->type == "SysPrintLn") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);

            Tac* printInstruction = new Print(lhsType);
            current_block->tacInstructions.push_back(printInstruction);
        }
        if(node->type == "Identifier assign") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);

            std::cout << lhsType << " := " << rhsType << std::endl;
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
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);

            return "boolean";
        }
        if(node->type == "LesserThanExpression" || node->type == "GreaterThanExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);

            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "boolean"));
            Tac* lesserThanInstruction = new Expression("<", lhsType, rhsType, name);
            current_block->tacInstructions.push_back(lesserThanInstruction);

            return name;
        }
        // EqualExpression
        if(node->type == "EqualExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);

            return "boolean";
        }


        // Mathematical Operators
        if(node->type == "AddExpression" || node->type == "SubExpression" || 
        node->type == "MultExpression" || node->type == "DivExpression") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);

            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "int"));
            Tac* ExpressionInstruction;
            if (node->type == "AddExpression") ExpressionInstruction = new Expression("+", lhsType, rhsType, name);
            if (node->type == "SubExpression") ExpressionInstruction = new Expression("-", lhsType, rhsType, name);
            if (node->type == "MultExpression") ExpressionInstruction = new Expression("*", lhsType, rhsType, name);
            if (node->type == "DivExpression") ExpressionInstruction = new Expression("/", lhsType, rhsType, name);
            // ExpressionInstruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(ExpressionInstruction);

            return name; // The result of these operations is also an 'int'
        }


        // Programmer Operators
        if (node->type == "Array access") {
            std::string lhsType = traverseTreeIR(node->children.front(), sym_table);
            std::string rhsType = traverseTreeIR(node->children.back(), sym_table);

            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "int"));
            // ArrayAccess(std::string _array, std::string _index, std::string _result)
            Tac* arrayAccessInstruction = new ArrayAccess(lhsType, rhsType, name);
            current_block->tacInstructions.push_back(arrayAccessInstruction);

            return name;
        }
        if (node->type == "Expression" && node->value == "length") {
            // .length property for arrays
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table);
            std::string name = generateTempId();

            sym_table->put(name, new symbol_table::Variable(name, "int"));
            Tac* lengthInstruction = new LengthAccess(lhsName, name);
            // lengthInstruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(lengthInstruction);

            return name;
        }
        if(node->type == "ExpressionCallMethod") {
            // [X] - Check if using this as expression then you need to check if this is called from a class (lhs)
            // [X] - Check if the identifier function exists in the expression above and has correct return type (mhs)
            // [X] - (IF EXISTS) Check if the number of arguments and argument types are correct to the function from mhs (rhs)
            // First step
            std::string lhsName = traverseTreeIR(node->children.front(), sym_table);
            // std::string lhsName = node->children.front()->value;
            // std::string className = sym_table->findSymbol(lhsName)->getType();
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
                    // symbol_table::Variable* _method_param = _method->lookupParameter(param->value);
                    std::string lhsName = traverseTreeIR(*i, sym_table);
                    Tac* input_param = new Parameter(lhsName);
                    current_block->tacInstructions.push_back(input_param);
                }
            } else {
                // Dont add params
            }

            std::string name = generateTempId();
            sym_table->put(name, new symbol_table::Variable(name, "method"));
            // Add the className somehow?
            Tac* callMethodInstruction = new MethodCall(lhsName + "." + mhsName, std::to_string(_paramSize), name);
            // callMethodInstruction->dump(); std::cout << std::endl;
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
            // instruction->dump(); std::cout << std::endl;
            current_block->tacInstructions.push_back(instruction);
            // Return name to find it again, spliting it up to this:
            // x := new int[Y]          (1)
            // _t0 = new int[Y]         (2)
            // x := _t0                 (3)
            // Want to use copy so need the `new int[Y]` in a singular instruction.
            return name;
        }
        if(node->type == "NewIstance") {
            // auto instance = sym_table->findSymbol(node->value);
            std::string instance = node->value;

            // ------ USE THESE IF YOU WANT TO CREATE TEMPORY VARIABLE WITH NEW INSTANCE ------
            // std::string name = generateTempId();

            // sym_table->put(name, new symbol_table::Variable(name, instance));
            // Tac* newObjectInstruction = new NewObject(instance, name);
            // current_block->tacInstructions.push_back(newObjectInstruction);

            // return name;
            // ---------------------------------------------------------------------------------
            return instance;
        }
        if(node->type == "NotOperation") {
            std::string type = traverseTreeIR(node->children.front(), sym_table);
            std::string name = generateTempId();

            // Maybe add this `sym_table->findSymbol(type)->getType()` which results in std::string = "boolean"
            sym_table->put(name, new symbol_table::Variable(name, "boolean"));
            Tac* newUnaryExpressionInstruction = new UnaryExpression("!", type, name);
            current_block->tacInstructions.push_back(newUnaryExpressionInstruction);

            return name;
            // return "!" + type;
        }

        return node->type;
    }



    void printCFG() {
        ofstream outStream("cfg.dot");
        if (!outStream.is_open()) {
            cerr << "Error opening file cfg.dot" << endl;
            return;
        }

        outStream << "digraph CFG {\n";
        unordered_set<BBlock*> visited; // To keep track of visited blocks
        for(BBlock* methodEntry : method_blocks){
            generateCFGContent(methodEntry, outStream, visited);
        }
        outStream << "}\n";
        outStream.close();

        cout << "Built CFG at cfg.dot. Use 'dot -Tpdf cfg.dot -o cfg.pdf' to generate the PDF version." << endl;
    }


    void generateCFGContent(BBlock* block, ofstream& outStream, unordered_set<BBlock*>& visited) {
        if (block == nullptr || visited.find(block) != visited.end()) {
            return; // Avoid printing the same block twice or null blocks
        }

        visited.insert(block); // Mark the current block as visited
        outStream << "    \"" << block->name << "\" [shape=box label=\"" << block->name << "\\n\n";

        for(Tac* instruction : block->tacInstructions) {
            outStream << instruction->getDump();
            outStream << "\\n";
        }

        if(block->condition) outStream << block->condition->getDump();

        outStream << "\"];\n";

        if (block->trueExit != nullptr) {
            outStream << "    \"" << block->name << "\" -> \"" << block->trueExit->name << "\" [label=\"true\"];\n";
            generateCFGContent(block->trueExit, outStream, visited);
        }
        if (block->falseExit != nullptr) {
            outStream << "    \"" << block->name << "\" -> \"" << block->falseExit->name << "\" [label=\"false\"];\n";
            generateCFGContent(block->falseExit, outStream, visited);
        }
    }



    void printBlocks(BBlock* block, const std::string& prefix = "") {
        if (!block) return; // Base case: if block is null, do nothing

        // Print block name and its TAC instructions
        std::cout << prefix << block->name << ":" << std::endl;
        for (Tac* instruction : block->tacInstructions) {
            std::cout << prefix + "    ";
            instruction->dump();
            std::cout << std::endl;
        }

        if (block->condition) {
            std::cout << prefix + "    ";
            block->condition->dump();
            std::cout << std::endl;
        }
        // Recursively print trueExit and falseExit if they exist
        if (block->trueExit) {
            // std::cout << prefix + "    " << block->name << " ---> true:" << std::endl;
            printBlocks(block->trueExit, prefix + "    ");
        }
        if (block->falseExit) {
            // std::cout << prefix + "    " << block->name << " ---> false:" << std::endl;
            printBlocks(block->falseExit, prefix + "    ");
        }
    }

    errCodes generateCFG() {
        std::cout << "- CFG CREATION PENDING" << "\t\t..." << std::endl;

        // Print structure to console
        for (BBlock* method_block : method_blocks) {
            printBlocks(method_block);
            std::cout << std::endl;
        }

        std::cout << "- CFG CREATION SUCCESS" << "\t\t✅" << std::endl;
        return errCodes::SUCCESS;
    }


    errCodes generateIR(Node* root, symbol_table::SymbolTable* sym_table) {
        try {
            errCodes cfg_error = errCodes::SUCCESS;

            sym_table->resetTable();
            BBlock* _top_block = new BBlock("Program");
            current_block = _top_block;
            traverseTreeIR(root, sym_table);

            cfg_error = generateCFG();
            printCFG();

            if (cfg_error != errCodes::SUCCESS) throw std::runtime_error("");

            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEGMENTATION_FAULT; // Return a semantic error code if an exception was caught
        }
    }
}