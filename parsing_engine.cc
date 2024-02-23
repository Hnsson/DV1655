#include "parsing_engine.hh"

namespace symbol_table {
    SymbolTable::SymbolTable() {
        root = new Scope();
        current = root;
        // Initialize with global scope if necessary
    }

    SymbolTable::~SymbolTable() {};

    void SymbolTable::enterScope(string name, SymbolRecord* classContext = nullptr)  {
        current = current->nextChild(name, classContext);
    }

    void SymbolTable::exitScope()  {
        current = current->getParent();
    }

    void SymbolTable::put(string key, SymbolRecord* item) {
        current->put(key, item);
    }

    SymbolRecord* SymbolTable::findSymbol(string name) {
        return current->lookup(name);
    }

    void SymbolTable::printTable() {
        cout << std::endl << "-----" << root->scopeName << "-----" << std::endl;
        root->printScope();

        for(auto i = root->childrenScopes.begin(); i != root->childrenScopes.end(); i++){
            printAllTable(*i);
        }
        cout << "----- end" << root->scopeName << "-----" << std::endl;
    }

    void SymbolTable::printAllTable(Scope* node, int level) {
        std::cout << std::endl << std::string(level, '\t') << "-----" << node->scopeName << "-----" << std::endl;
        node->printScope(level);

        // Correctly increment level for nested scopes, without affecting siblings
        for(auto i = node->childrenScopes.begin(); i != node->childrenScopes.end(); i++){
            printAllTable(*i, level + 1); // Use level + 1 here
        }

        std::cout << std::string(level, '\t') << "----- end " << node->scopeName << "-----" << std::endl;
    }

    void SymbolTable::resetTable() {
        root->resetScope();
    }


    void traverseTree(Node* root, SymbolTable* sym_table) {
        if(root->type == "Program") {
            if(!root->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = root->children.begin(); i != root->children.end(); ++i) {
                    if ((*i)->type == "Main Class") {
                        // Handle the main class
                        // Added (*classNode)->value as type instead of (*i)->type. Seems its more correct
                        Class* main_class = new Class((*i)->value, (*i)->value);
                        sym_table->put((*i)->value, main_class);

                        sym_table->enterScope("Class: " + (*i)->value, main_class);
                            Variable* _this = new Variable("this", (*i)->value);
                            // main_class->addVariable(_this);
                            sym_table->put("this", _this);
                            traverseTree(*i, sym_table);
                        sym_table->exitScope();
                    } else if ((*i)->type == "ClassList") {
                        // Traverse each class within the class list
                        for (auto classNode = (*i)->children.begin(); classNode != (*i)->children.end(); ++classNode) {
                            // Added (*classNode)->value as type instead of (*i)->type. Seems its more correct
                            Class* other_class = new Class((*classNode)->value, (*classNode)->value);
                            sym_table->put((*classNode)->value, other_class);

                            sym_table->enterScope("Class: " + (*classNode)->value, other_class);
                                Variable* _this = new Variable("this", (*classNode)->value);
                                // other_class->addVariable(_this);
                                sym_table->put("this", _this);
                                traverseTree(*classNode, sym_table);
                            sym_table->exitScope();
                        }
                    }
                }
            }
        }
        if(root->type == "Main Class") {
            Method* context = new Method("main", "void");
            sym_table->put("main", context);
            // ((Class*)sym_table->getCurrentScope()->scopeContext)->addMethod(context);
            // Works with and without context below, heads up for future
            sym_table->enterScope("Method: main", context);
                // Assuming the first child of the main class is the parameter "String[] a"
                Variable* args = new Variable(root->children.front()->value, "String[]");
                // context->addParameter(args);
                sym_table->put(root->children.front()->value, args);
            sym_table->exitScope();
        }
        if(root->type == "Class") {
            if(!root->children.empty()) {
                for(auto i = root->children.begin(); i != root->children.end(); i++) {
                    traverseTree(*i, sym_table);
                }
            }
        }
        if(root->type == "Var declarations") {
            if(!root->children.empty()) {
                for(auto i = root->children.begin(); i != root->children.end(); i++) {
                    // Dont want to traverse because want to handle these `Var declarations`
                    // differently due to being variables of a class instead of method
                    traverseTree(*i, sym_table);

                    // Variable* var_dec = new Variable((*i)->children.back()->value, (*i)->children.front()->value);
                    // sym_table->put((*i)->children.back()->value, var_dec);
                    // ((Class*)sym_table->getCurrentScope()->scopeContext)->addVariable(var_dec);
                }
            }
        }
        if(root->type == "MethodDeclaration") {
            if(!root->children.empty()) {
                for(auto i = root->children.begin(); i != root->children.end(); i++) {
                    Node* type = *(*i)->children.begin();
                    Method* context = new Method((*i)->value, type->value);
                    sym_table->put((*i)->value, context);
                    ((Class*)sym_table->getCurrentScope()->getScopeContext())->addMethod(context);

                    sym_table->enterScope("Method: " + (*i)->value, context);
                        traverseTree(*i, sym_table);
                    sym_table->exitScope();
                }
            }
        }
        if(root->type == "Method") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table);
            }
        }
        if(root->type == "Method parameters") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                // Dont want to traverse because want to handle these `Var declarations`
                // differently due to being parameters instead of variables
                // traverseTree(*i, sym_table);

                Variable* var_dec = new Variable((*i)->children.back()->value, (*i)->children.front()->value);
                sym_table->put((*i)->children.back()->value, var_dec);
                ((Method*)sym_table->getCurrentScope()->getScopeContext())->addParameter(var_dec);
            }
        }
        if(root->type == "MethodContent") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table);
            }
        }
        if(root->type == "Content") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table);
            }
        }
        if(root->type == "Return") {
            // Add logic later
        }
        if(root->type == "Var declaration") {
            // Add to variable
            Variable* var_dec = new Variable(root->children.back()->value, root->children.front()->value);
            sym_table->put(root->children.back()->value, var_dec);
            ((Method*)sym_table->getCurrentScope()->getScopeContext())->addVariable(var_dec);
        }
    }
}






namespace semantic_analysis {
    std::string traverseTreeSemantically(Node* node, symbol_table::SymbolTable* sym_table) {
        if(node->type == "Program") {
            if(!node->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = node->children.begin(); i != node->children.end(); ++i) {
                    if ((*i)->type == "Main Class") {
                        // Handle the main class
                        sym_table->enterScope("Class: " + (*i)->value);
                        traverseTreeSemantically(*i, sym_table);
                        sym_table->exitScope();
                    } else if ((*i)->type == "ClassList") {
                        // Traverse each class within the class list
                        for (auto classNode = (*i)->children.begin(); classNode != (*i)->children.end(); ++classNode) {
                            // Added (*classNode)->value as type instead of (*i)->type. Seems its more correct
                            sym_table->enterScope("Class: " + (*classNode)->value);
                            traverseTreeSemantically(*classNode, sym_table);
                            sym_table->exitScope();
                        }
                    }
                }
            }
        }
        if(node->type == "Main Class") {
            sym_table->enterScope("Method: main");
            for(auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
            sym_table->exitScope();
        }
        if(node->type == "Class") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    traverseTreeSemantically(*i, sym_table);
                }
            }
        }
        if(node->type == "MethodDeclaration") {
            if(!node->children.empty()) {
                for(auto i = node->children.begin(); i != node->children.end(); i++) {
                    Node* type = *(*i)->children.begin();

                    sym_table->enterScope("Method: " + (*i)->value);
                    traverseTreeSemantically(*i, sym_table);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Method") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "MethodContent") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "Content") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "Return") {
            // Add logic later
        }
        if(node->type == "Statements") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "Statement") {
            traverseTreeSemantically(node->children.front(), sym_table);
        }

        // Now at level of statement-/expression components
        // Will first perform on statements

        if(node->type == "If-Expression-Statement") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if (lhsType != "Boolean") {
                throw std::runtime_error("error: cannot convert '" + lhsType + "' to 'boolean' on line: " + std::to_string(node->lineno));
            }

            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "If/Else-Expression-Statement") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if (lhsType != "Boolean") {
                throw std::runtime_error("error: cannot convert '" + lhsType + "' to 'boolean' on line: " + std::to_string(node->lineno));
            }

            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "While-Statement") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if (lhsType != "Boolean") {
                throw std::runtime_error("error: cannot convert '" + lhsType + "' to 'boolean' on line: " + std::to_string(node->lineno));
            }

            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }
        }
        if(node->type == "SysPrintLn") {
            // Idk the rules here ¯\_(ツ)_/¯
        }
        if(node->type == "Identifier assign") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if (lhsType != rhsType) {
                throw std::runtime_error("error: cannot convert '" + lhsType + "' to '" + rhsType + "' in assignment on line: " + std::to_string(node->lineno));
            }
        }
        if(node->type == "ArrayPositionAssignOp") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string posType = traverseTreeSemantically(*(++node->children.begin()), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);
            // Check assignment position
            if(lhsType != "int[]") {
                throw std::runtime_error("error: Expression must be of type int[] on line: " + std::to_string(node->lineno));
            }
            if(posType != "int") {
                throw std::runtime_error("error: Cannot convert assignment position '" + posType + "' to int on line: " + std::to_string(node->lineno));
            }
            if(rhsType != "int") {
                throw std::runtime_error("error: Cannot convert assignment value '" + rhsType + "' to int on line: " + std::to_string(node->lineno));
            }
        }



        // Complex Operators
        if(node->type == "LogicalAndExpression" || node->type == "LogicalOrExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != "boolean" || rhsType != "boolean") {
                throw std::runtime_error("error: " + node->type + " requires both operands to be of type 'boolean', but found '" + lhsType + "' and '" + rhsType + "' on line: " + std::to_string(node->lineno));
            }

            return "Boolean";
        }
        if(node->type == "LesserThanExpression" || node->type == "GreaterThanExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != "int" || rhsType != "int") {
                throw std::runtime_error("error: " + node->type + " requires both operands to be of type 'int', but found '" + lhsType + "' and '" + rhsType + "' on line: " + std::to_string(node->lineno));
            }

            return "Boolean";
        }
        // EqualExpression
        if(node->type == "EqualExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != rhsType) {
                throw std::runtime_error("error: Equal expression requires both operands to be of the same type, but found '" + lhsType + "' and '" + rhsType + "' on line: " + std::to_string(node->lineno));
            }

            return "Boolean";
        }


        // Mathematical Operators
        if(node->type == "AddExpression" || node->type == "SubExpression" || 
        node->type == "MultExpression" || node->type == "DivExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            // Both operands must be of type 'int'
            if(lhsType != "int" || rhsType != "int") {
                throw std::runtime_error("error: " + node->type + " requires both operands to be of type 'int', but found '" + lhsType + "' and '" + rhsType + "' on line: " + std::to_string(node->lineno));
            }

            return "int"; // The result of these operations is also an 'int'
        }


        // Programmer Operators
        if (node->type == "Array access") {

        }
        if (node->type == "Expression" && node->value == "length") {

        }
        if(node->type == "ExpressionCallMethod") {

        }


        // Lowest level components / instanciators
        if(node->type == "Identifier" || node->type == "this") {
            // Check if in scope


            // Return type
            std::cout << node->value << std::endl;
            auto identifier = sym_table->findSymbol(node->value);

            if (identifier == NULL) {
                throw std::runtime_error("error: Identifier '" + node->value + "' is not defined on line: " + std::to_string(node->lineno));
            }
            return identifier->getType();
        }
        if(node->type == "Int") {
            return "int";
        }
        if(node->type == "true" || node->type == "false") {
            return "boolean";
        }
        if(node->type == "this") {

        }
        if(node->type == "NewArray" && node->value == "int") {

        }
        if(node->type == "NewIstance") {

        }
        if(node->type == "NotOperation") {
            std::string type = traverseTreeSemantically(node->children.front(), sym_table);

            if(type != "boolean") {
                throw std::runtime_error("error: no known conversion for '" + type + "' to 'bool' on line: " + std::to_string(node->lineno));
            }

            return node->type;
        }

        return "success";
    }

    errCodes semantic_analysis(Node* node, symbol_table::SymbolTable* sym_table) {
        try {
            // [ ] Verify that all identifiers are declared
            // [ ] Type checking
            // [ ] Expressions: e.g., In a + b; both a and b should be integers
            // [ ] Statemetns: e.g., In a = b; type of a and b should be the same
            // [ ] Methods: In int a(int c) ... return b; the type of return
            //              expression (b) should be the same as type of the method declaration (int).
            // [ ] Method calls: In a(x); the type of x and c should be the same
            // [ ] Array access: In b[a]; b.length the type of a must be integer; the type of b must
            //                   be int[];
            // [ ] Out of scope: Inheritance, polymorphic calls, and other OO features
            sym_table->resetTable();
            std::cout << traverseTreeSemantically(node, sym_table) << std::endl;
            
            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEMANTIC_ERROR; // Return a semantic error code if an exception was caught
        }
    }
}