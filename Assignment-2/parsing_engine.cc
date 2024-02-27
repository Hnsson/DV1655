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


    bool traverseTree(Node* root, SymbolTable* sym_table, bool& encounteredError) {
        if(root->type == "Program") {
            if(!root->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = root->children.begin(); i != root->children.end(); ++i) {
                    // Handle the main class
                    // Added (*classNode)->value as type instead of (*i)->type. Seems its more correct
                    Class* main_class = new Class((*i)->value, (*i)->value);
                    if (sym_table->findSymbol((*i)->value) != NULL)  { std::cerr << "[S1] @error at line " + std::to_string((*i)->lineno) + ": Class '" + (*i)->value + "' is already defined" << std::endl; encounteredError = true; }
                    sym_table->put((*i)->value, main_class);

                    sym_table->enterScope("Class: " + (*i)->value, main_class);
                        Variable* _this = new Variable("this", (*i)->value);
                        // main_class->addVariable(_this);
                        sym_table->put("this", _this);
                        traverseTree(*i, sym_table, encounteredError);
                    sym_table->exitScope();
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
                    traverseTree(*i, sym_table, encounteredError);
                }
            }
        }
        if(root->type == "Var declarations") {
            if(!root->children.empty()) {
                for(auto i = root->children.begin(); i != root->children.end(); i++) {
                    // Dont want to traverse because want to handle these `Var declarations`
                    // differently due to being variables of a class instead of method
                    // traverseTree(*i, sym_table);

                    Variable* var_dec = new Variable((*i)->children.back()->value, (*i)->children.front()->value);
                    // if (sym_table->findSymbol((*i)->children.back()->value) != NULL) throw std::runtime_error("[S1] error: Variable '" + (*i)->children.back()->value + "' is already defined on line: " + std::to_string((*i)->lineno));
                    if (((Class*)sym_table->getCurrentScope()->getScopeContext())->lookupVariable((*i)->children.back()->value) != NULL)  { std::cerr << "[S1] @error at line " + std::to_string((*i)->lineno) + ": Variable '" + (*i)->children.back()->value + "' is already defined" << std::endl; encounteredError = true; }
                    sym_table->put((*i)->children.back()->value, var_dec);
                    ((Class*)sym_table->getCurrentScope()->getScopeContext())->addVariable(var_dec);
                }
            }
        }
        if(root->type == "MethodDeclaration") {
            if(!root->children.empty()) {
                for(auto i = root->children.begin(); i != root->children.end(); i++) {
                    Node* type = *(*i)->children.begin();
                    Method* context = new Method((*i)->value, type->value);
                    // if (sym_table->findSymbol((*i)->value) != NULL) throw std::runtime_error("[S2] error: Method '" + (*i)->value + "' is already defined");
                    if (((Class*)sym_table->getCurrentScope()->getScopeContext())->lookupMethod((*i)->value) != NULL)  { std::cerr << "[S2] @error at line " + std::to_string((*i)->lineno) + ": Method '" + (*i)->value + "' is already defined" << std::endl; encounteredError = true; }
                    sym_table->put((*i)->value, context);
                    ((Class*)sym_table->getCurrentScope()->getScopeContext())->addMethod(context);

                    sym_table->enterScope("Method: " + (*i)->value, context);
                        traverseTree(*i, sym_table, encounteredError);
                    sym_table->exitScope();
                }
            }
        }
        if(root->type == "Method") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table, encounteredError);
            }
        }
        if(root->type == "Method parameters") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                // Dont want to traverse because want to handle these `Var declarations`
                // differently due to being parameters instead of variables
                traverseTree(*i, sym_table, encounteredError);
                Variable* var_dec = (Variable*)sym_table->findSymbol((*i)->children.back()->value);


                // Variable* var_dec = new Variable((*i)->children.back()->value, (*i)->children.front()->value);
                // if (sym_table->findSymbol((*i)->children.back()->value) != NULL) throw std::runtime_error("[S3] error: Method parameter '" + (*i)->children.back()->value + "' is already defined");
                // sym_table->put((*i)->children.back()->value, var_dec);
                ((Method*)sym_table->getCurrentScope()->getScopeContext())->addParameter(var_dec);
            }
        }
        if(root->type == "MethodContent") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table, encounteredError);
            }
        }
        if(root->type == "Content") {
            for (auto i = root->children.begin(); i != root->children.end(); i++) {
                traverseTree(*i, sym_table, encounteredError);
            }
        }
        if(root->type == "Return") {
            // Add logic later
        }
        if(root->type == "Var declaration") {
            // Add to variable
            Variable* var_dec = new Variable(root->children.back()->value, root->children.front()->value);
            // if (sym_table->findSymbol(root->children.back()->value) != NULL) throw std::runtime_error("[S3] error: Variable '" + root->children.back()->value + "' is already defined on line: " + std::to_string(root->lineno));
            if (((Method*)sym_table->getCurrentScope()->getScopeContext())->lookupVariable(root->children.back()->value) != NULL)  { std::cerr << "[S4] @error at line " + std::to_string(root->children.back()->lineno) + ": Variable '" + root->children.back()->value + "' is already defined" << std::endl; encounteredError = true; }
            sym_table->put(root->children.back()->value, var_dec);
            ((Method*)sym_table->getCurrentScope()->getScopeContext())->addVariable(var_dec);
        }

        return encounteredError;
    }


    errCodes syntax_analysis(Node* root, SymbolTable* sym_table) {
        try {
            bool encounteredError = false;
            traverseTree(root, sym_table, encounteredError);

            if(encounteredError) throw std::runtime_error("");

            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEMANTIC_ERROR; // Return a semantic error code if an exception was caught
        }
    }
}

namespace semantic_analysis {
    std::string traverseTreeSemantically(Node* node, symbol_table::SymbolTable* sym_table) {
        if(node->type == "Program") {
            if(!node->children.empty()) {
                // Assuming the first child is always the main class and the rest are part of the class list
                for(auto i = node->children.begin(); i != node->children.end(); ++i) {
                    // Handle the classes
                    sym_table->enterScope("", NULL);
                    traverseTreeSemantically(*i, sym_table);
                    sym_table->exitScope();
                }
            }
        }
        if(node->type == "Main Class") {
            sym_table->enterScope("", NULL);
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

                    sym_table->enterScope("", NULL);
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
            std::string returnType = traverseTreeSemantically(node->children.back(), sym_table);
            std::string methodType = ((symbol_table::Method*)sym_table->getCurrentScope()->getScopeContext())->getType();
            // Add logic later
            if (returnType != methodType) {
                std:: cerr << "[0.1] @error at line " + std::to_string(node->children.back()->lineno) + ": cannot convert return type of '" + returnType + "' to '" + methodType + "'" << std::endl;
            }
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
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }

            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            // std::cout << "THE TYPE IS: " << lhsType << std::endl;
            if (lhsType != "boolean") {
                std:: cerr << "[0] @error at line " + std::to_string(node->lineno) + ": cannot convert '" + lhsType + "' to 'boolean'" << std::endl;
            }
        }
        if(node->type == "If/Else-Expression-Statement") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }

            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            // std::cout << "THE TYPE IS: " << lhsType << std::endl;
            if (lhsType != "boolean") {
                std::cerr << "[1] @error at line " + std::to_string(node->lineno) + ": cannot convert '" + lhsType + "' to 'boolean'" << std::endl;
            }
        }
        if(node->type == "While-Statement") {
            for (auto i = node->children.begin(); i != node->children.end(); i++) {
                traverseTreeSemantically(*i, sym_table);
            }

            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if (lhsType != "boolean") {
                std::cerr << "[2] @error at line " + std::to_string(node->lineno) + ": cannot convert '" + lhsType + "' to 'boolean'" << std::endl;
            }
        }
        if(node->type == "SysPrintLn") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if (lhsType != "int") {
                std::cerr << "[3] @error at line " + std::to_string(node->lineno) + ": cannot convert '" + lhsType + "' to 'int'" << std::endl;
            }
        }
        if(node->type == "Identifier assign") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if (lhsType != rhsType) {
                std::cerr << "[4] @error at line " + std::to_string(node->lineno) + ": cannot convert '" + lhsType + "' to '" + rhsType + "' in assignment" << std::endl;
            }
        }
        if(node->type == "ArrayPositionAssignOp") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string posType = traverseTreeSemantically(*(++node->children.begin()), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);
            // Check assignment position
            if(lhsType != "int[]") {
                std::cerr << "[5] @error at line " + std::to_string(node->lineno) + ": Expression must be of type int[]" << std::endl;
            }
            if(posType != "int") {
                std::cerr << "[6] @error at line " + std::to_string(node->lineno) + ": Cannot convert assignment position '" + posType + "' to int" << std::endl;
            }
            if(rhsType != "int") {
                std::cerr << "[7] @error at line " + std::to_string(node->lineno) + ": Cannot convert assignment value '" + rhsType + "' to int" << std::endl;
            }
        }



        // Complex Operators
        if(node->type == "LogicalAndExpression" || node->type == "LogicalOrExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != "boolean" || rhsType != "boolean") {
                std::cerr << "[8] @error at line " + std::to_string(node->lineno) + ": " + node->type + " requires both operands to be of type 'boolean', but found '" + lhsType + "' and '" + rhsType + "'" << std::endl;
            }

            return "boolean";
        }
        if(node->type == "LesserThanExpression" || node->type == "GreaterThanExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != "int" || rhsType != "int") {
                std::cerr << "[9] @error at line " + std::to_string(node->lineno) + ": " + node->type + " requires both operands to be of type 'int', but found '" + lhsType + "' and '" + rhsType + "'" << std::endl;
            }

            return "boolean";
        }
        // EqualExpression
        if(node->type == "EqualExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != rhsType) {
                std::cerr << "[10] @error at line " + std::to_string(node->lineno) + ": Equal expression requires both operands to be of the same type, but found '" + lhsType + "' and '" + rhsType + "'" << std::endl;
            }

            return "boolean";
        }


        // Mathematical Operators
        if(node->type == "AddExpression" || node->type == "SubExpression" || 
        node->type == "MultExpression" || node->type == "DivExpression") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            // Both operands must be of type 'int'
            if(lhsType != "int" || rhsType != "int") {
                std::cerr << "[11] @error at line " + std::to_string(node->lineno) + ": " + node->type + " requires both operands to be of type 'int', but found '" + lhsType + "' and '" + rhsType + "'" << std::endl;
            }

            return "int"; // The result of these operations is also an 'int'
        }


        // Programmer Operators
        if (node->type == "Array access") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);
            std::string rhsType = traverseTreeSemantically(node->children.back(), sym_table);

            if(lhsType != "int[]") {
                std::cerr << "[12.1] @error at line " + std::to_string(node->lineno) + ": accessing array requires an array of type 'int[]', but found '" + lhsType + "'" << std::endl;
            }
            else if(rhsType != "int") {
                std::cerr << "[12.2] @error at line " + std::to_string(node->lineno) + ": accessing array requires access position of type 'int' but found '" + rhsType + "'" << std::endl;
            }

            return "int";
        }
        if (node->type == "Expression" && node->value == "length") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if(lhsType != "int[]") {
                std::cerr << "[13] @error at line " + std::to_string(node->lineno) + ": length of expression requires expression of type 'int[]', but found '" + lhsType + "'" << std::endl;
            }

            return "int";
        }
        if(node->type == "ExpressionCallMethod") {
            // [X] - Check if using this as expression then you need to check if this is called from a class (lhs)
            // [X] - Check if the identifier function exists in the expression above and has correct return type (mhs)
            // [X] - (IF EXISTS) Check if the number of arguments and argument types are correct to the function from mhs (rhs)
            // First step
            std::string lhsName = traverseTreeSemantically(node->children.front(), sym_table);

            // Because the left hand side is always a class instance, look at the root for classes with that name
            auto lhs_class = (symbol_table::Class*)sym_table->getRootScope()->lookup(lhsName);
            if (lhs_class == NULL) {
                std::cerr << "[14.1] @error at line " + std::to_string(node->lineno) + ": Instance of '" + lhsName + "' is not defined" << std::endl;
                throw std::runtime_error("[SEGFAULT] error: Could not cast");
            }

            // Second step
            std::string mhsName = (*(++node->children.begin()))->value;
            // symbol_table::Method* _method = ((symbol_table::Class*)sym_table->findSymbol(lhsName))->lookupMethod(mhsName);
            symbol_table::Method* _method = lhs_class->lookupMethod(mhsName);
            if(_method == NULL) {
                std::cerr << "[14.2] @error at line " + std::to_string(node->lineno) + ": Method '"+ mhsName + "' of instance '" + lhsName + "' is not defined" << std::endl;
                throw std::runtime_error("[SEGFAULT] error: Could not cast");
            }

            // Third step if exists
            auto it = node->children.begin();
            std::advance(it, 2);
            if (it != node->children.end()) {
                int n_input_params = 0;
                // Third child exists -> input parameters exists
                // std::cout << "Input parameters exists" << std::endl;
                for (auto i = (*it)->children.begin(); i != (*it)->children.end(); i++) {
                    n_input_params++;
                }

                // Check if number parameters is correct
                if (n_input_params != _method->getParamSize()) {
                    std::cerr << "[15] @error at line " + std::to_string(node->lineno) + ": Method '"+ mhsName + "' has (" + std::to_string(_method->getParamSize()) + ") parameter(s), (" + std::to_string(n_input_params) + ") was given" << std::endl;
                }
                // Check if types are correct
                std::vector<symbol_table::Variable*> _method_params = _method->getParams();
                int j = 0;
                for (auto i = (*it)->children.begin(); i != (*it)->children.end(); i++) {
                    // symbol_table::Variable* _method_param = _method->lookupParameter(param->value);
                    std::string lhsName = traverseTreeSemantically(*i, sym_table);
                    if (lhsName != _method_params[j]->getType()) {
                        std::cerr << "[16] @error at line " + std::to_string(node->lineno) + ": expected parameter type (" + _method_params[j]->getType() + ") but (" + lhsName + ") was given" << std::endl;
                    }
                    j++;
                }
            } else {
                // Third child does NOT exists -> no input parameters
                // std::cout << "No input parameters" << std::endl;
            }


            // std::cout << "NEW INSTANCE OF: " << lhsName << std::endl;
            // std::cout << "CALLING METHOD: " << mhsName << " WITH TYPE: " << _method->getType() << std::endl;
            return _method->getType();
        }


        // Lowest level components / instanciators
        if(node->type == "Identifier" || node->type == "this") {
            auto identifier = sym_table->findSymbol(node->value);

            if (identifier == NULL) {
                std::cerr << "[17] @error at line " + std::to_string(node->lineno) + ": Identifier '" + node->value + "' is not defined" << std::endl;
                return "undefined";
                // throw std::runtime_error("[SEGFAULT] error: Could not find identifier");
            }
            return identifier->getType();
        }
        if(node->type == "int") {
            return "int";
        }
        if(node->type == "boolean") {
            return "boolean";
        }
        if(node->type == "NewArray" && node->value == "int") {
            std::string lhsType = traverseTreeSemantically(node->children.front(), sym_table);

            if(lhsType != "int") {
                std::cerr << "[19] @error at line " + std::to_string(node->lineno) + ": length of expression requires expression of type 'int[]', but found '" + lhsType + "'" << std::endl;
            }

            return "int[]";
        }
        if(node->type == "NewIstance") {
            auto instance = sym_table->findSymbol(node->value);

            if (instance == NULL) {
                std::cerr << "[20] @error at line " + std::to_string(node->lineno) + ": Instance of '" + node->value + "' is not defined" << std::endl;
            }
            return instance->getName();
        }
        if(node->type == "NotOperation") {
            std::string type = traverseTreeSemantically(node->children.front(), sym_table);
            // std::cout << "REEEEEE: " << type << std::endl;

            if(type != "boolean") {
                std::cerr << "[21] @error at line " + std::to_string(node->lineno) + ": no known conversion for '" + type + "' to 'bool'" << std::endl;
            }

            return type;
        }

        return node->type;
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
            traverseTreeSemantically(node, sym_table);
            
            return errCodes::SUCCESS; // If no exceptions were thrown, analysis succeeded
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return errCodes::SEMANTIC_ERROR; // Return a semantic error code if an exception was caught
        }
    }
}