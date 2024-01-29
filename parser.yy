%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires{
  #include <string>
  #include "Node.h"
  #define USE_LEX_ONLY false //change this macro to true if you want to isolate the lexer from the parser.
}

%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
}

// definition of set of tokens. All tokens are of type string
%token <std::string> PLUSOP MINUSOP MULTOP DIVOP INT LP RP EQOP LTOP GTOP ANDOP OROP NOTOP DOT SEMICOLON COMMA ASSIGNOP LBRACE RBRACE LBRACKET RBRACKET CLASS PUBLIC STATIC VOID MAIN STRINGDEC INTDEC BOOL IF ELSE WHILE SYSPRINTLN LENGTH TRUE FALSE THIS NEW ID RETURN
%token END 0 "end of file"

//definition of operator precedence. See https://www.gnu.org/software/bison/manual/bison.html#Precedence-Decl
%left EQOP ANDOP OROP
%left LTOP GTOP
%left PLUSOP MINUSOP
%left MULTOP DIVOP
%left LBRACKET RBRACKET DOT LENGTH
%left NOTOP
%nonassoc RP
%nonassoc ELSE

// definition of the production rules. All production rules are of type Node
%type <Node *> root goal mainClass classDeclaration methodContentItem classDeclaration_inf varDeclaration varDeclaration_inf methodDeclaration methodDeclaration_inf methodContent methodContentParam methodHeader methodParam expression expression_inf factor statement statement_inf identifier type

%%
root:       goal {root = $1;};

goal: mainClass END
      {
        $$ = $1;
        root = $$;
      }
    | mainClass classDeclaration_inf END
      {
        $$ = $1;
        $$->children.push_back($2);
        root = $$;
      }
    ;

mainClass: PUBLIC CLASS identifier LBRACE PUBLIC STATIC VOID MAIN LP STRINGDEC LBRACKET RBRACKET identifier RP LBRACE statement_inf RBRACE RBRACE
            {
              $$ = new Node("Main Class", "", yylineno);
              $$->children.push_back($3);
              $$->children.push_back($13);
              $$->children.push_back($16);
            }
         ;

/*
classDeclaration_inf: classDeclaration {$$ = $1;}
                    | classDeclaration classDeclaration_inf
                    {
                        $$ = new Node("Class list", "", yylineno);
                        $$->children.push_back($1);
                        $$->children.push_back($2);
                      }
                    ;
*/
classDeclaration_inf: classDeclaration
                      {
                        $$ = new Node("ClassList", "", yylineno);
                        $$->children.push_back($1);
                      }
                    | classDeclaration_inf classDeclaration
                      {
                        $$ = $1;
                        $$->children.push_back($2);
                      }
                    ;




classDeclaration: CLASS identifier LBRACE RBRACE
                  {
                    /* case with both 0 0 cases */
                    $$ = new Node("No class body", "", yylineno);
                  }
                | CLASS identifier LBRACE methodDeclaration_inf RBRACE
                  {
                    /* case with 0 1-inf cases */
                    $$ = new Node("Class", "", yylineno);
                    $$->children.push_back($2);
                    $$->children.push_back($4);
                  }
                | CLASS identifier LBRACE varDeclaration_inf RBRACE
                  {
                    /* case with 1-inf 0 cases */
                    $$ = new Node("Class", "", yylineno);
                    $$->children.push_back($2);
                    $$->children.push_back($4);
                  }
                | CLASS identifier LBRACE varDeclaration_inf methodDeclaration_inf RBRACE
                  {
                    /* case with 1-inf 1-inf cases */
                    $$ = new Node("Class", "", yylineno);
                    $$->children.push_back($2);
                    $$->children.push_back($4);
                    $$->children.push_back($5);
                  }
                ;
/*
varDeclaration_inf: varDeclaration {$$ = $1;}
                  | varDeclaration varDeclaration_inf
                    {
                      $$ = new Node("Var declarations", "", yylineno);
                      $$->children.push_back($1);
                      $$->children.push_back($2);
                    }
                  ;
*/
varDeclaration_inf: varDeclaration
                    {
                      $$ = new Node("Var declarations", "", yylineno);
                      $$->children.push_back($1);
                    }
                  | varDeclaration_inf varDeclaration
                    {
                      $$ = $1;
                      $$->children.push_back($2);
                    }
                  ;

varDeclaration: type identifier SEMICOLON
                {
                  $$ = new Node("Var declaration", "", yylineno);
                  $$->children.push_back($1);
                  $$->children.push_back($2);
                }
              ;


/*
methodDeclaration_inf: methodDeclaration {$$ = $1;}
                     | methodDeclaration methodDeclaration_inf
                        {
                          $$ = new Node("MethodDeclaration", "", yylineno);
                          $$->children.push_back($1);
                          $$->children.push_back($2);
                        }
                     ;
*/
methodDeclaration_inf: methodDeclaration
                        {
                          $$ = new Node("MethodDeclaration", "", yylineno);
                          $$->children.push_back($1);
                        }
                     | methodDeclaration_inf methodDeclaration
                        {
                          $$ = $1;
                          $$->children.push_back($2);
                        }
                     ;


methodDeclaration: PUBLIC type identifier LP RP methodContent
                    {
                      /* First case of ? where no input (no parameters) */
                      $$ = new Node("Method", "", yylineno);
                      $$->children.push_back($2);
                      $$->children.push_back($3);
                      $$->children.push_back($6);
                    }
                 | PUBLIC type identifier LP methodHeader RP methodContent
                    {
                      /* Last case of certain input (parameters)*/
                      $$ = new Node("Method", "", yylineno);
                      $$->children.push_back($2);
                      $$->children.push_back($3);
                      $$->children.push_back($5);
                      $$->children.push_back($7);
                    }


methodHeader: methodParam { $$ = $1; /* Simply return method param */}
               | methodParam COMMA methodHeader
                  {
                    $$ = new Node("Method parameters", "", yylineno);
                    $$->children.push_back($1);
                    $$->children.push_back($3);
                  }
               ;

methodParam: type identifier 
              {
                $$ = new Node("Method parameter", "", yylineno);
                $$->children.push_back($1);
                $$->children.push_back($2);
              }
            ;


methodContent: LBRACE RETURN expression SEMICOLON RBRACE
                {
                  $$ = new Node("Return", "", yylineno);
                  $$->children.push_back($3);
                }
             | LBRACE methodContentParam RETURN expression SEMICOLON RBRACE
                {
                  $$ = new Node("MethodContent", "", yylineno);
                  Node* returnNode = new Node("Return", "", yylineno);
                  returnNode->children.push_back($4);

                  $$->children.push_back($2);
                  $$->children.push_back(returnNode);
                }
             ;

methodContentParam
    : methodContentItem
      {
        $$ = new Node("Content", "", yylineno);
        $$->children.push_back($1);
      }
    | methodContentParam methodContentItem
      {
        $$ = $1;
        $1->children.push_back($2);
      }
    ;

methodContentItem
    : varDeclaration
      {
        $$ = $1;
      }
    | statement
      {
        $$ = $1;
      }
    ;

/* 
methodContentParam: varDeclaration
                      {
                        $$ = $1;
                      }
                  | statement {$$ = $1;}
                  | methodContentParam varDeclaration
                    {
                      $$ = $1;
                      $$->children.push_back($2);
                    }
                  | methodContentParam statement
                    {
                      $$ = $1;
                      $$->children.push_back($2);
                    }
                  ; */
/*
methodContentParam
    : contentItem
      {
        $$ = new Node("MethodContents", "", yylineno);
        $$->children.push_back($1);
      }
    | methodContentParam contentItem
      {
        $$ = $1;
        $$->children.push_back($2);
      }
    ;

contentItem
    : varDeclaration_inf
      {
        $$ = $1;
      }
    | statement_inf
      {
        $$ = $1;
      }
    ;
*/

type: INTDEC LBRACKET RBRACKET {$$ = new Node("IntArrayType", "int[]", yylineno);}
    | BOOL {$$ = new Node("BooleanType", "bool", yylineno);}
    | INTDEC {$$ = new Node("IntegerType", "int", yylineno);}
    | identifier {$$ = new Node("IdentifierType", "id", yylineno); $$->children.push_back($1);}
    ;


// Newer, should put all statements under a node `Statements:`
statement_inf: statement 
               {
                $$ = new Node("Statements", "", yylineno);
                $$->children.push_back($1);
               }
             | statement_inf statement
               {
                  $$ = $1;
                  $$->children.push_back($2);
               }
             ;
/* Old
statement_inf: statement 
               {
                $$ = $1;  //Simply return statement
               }
             | statement statement_inf  
               {
                  $$ = new Node("Statements", "", yylineno);
                  $$->children.push_back($1);
                  $$->children.push_back($2);
               }
             ;
*/

statement: LBRACE RBRACE {$$ = new Node("Empty statement", "", yylineno);}
         | LBRACE statement_inf RBRACE {$$ = $2;}
         | IF LP expression RP statement
            {
              $$ = new Node("If-Expression-Statement", "", yylineno);
              $$->children.push_back($3);
              $$->children.push_back($5);
            }
         | IF LP expression RP statement ELSE statement
            {
              $$ = new Node("If/Else-Expression-Statement", "", yylineno);
              $$->children.push_back($3);
              $$->children.push_back($5);
              $$->children.push_back($7);
            }
         | WHILE LP expression RP statement
            {
              $$ = new Node("While-Statement", "", yylineno);
              $$->children.push_back($3);
              $$->children.push_back($5);
            }
         | SYSPRINTLN LP expression RP SEMICOLON
            {
              $$ = new Node("SysPrintLn", "", yylineno);
              $$->children.push_back($3);
            }
         | identifier ASSIGNOP expression SEMICOLON
            {
              $$ = new Node("Identifier assign", "", yylineno);
              $$->children.push_back($1);
              $$->children.push_back($3);
            }
         | identifier LBRACKET expression RBRACKET ASSIGNOP expression SEMICOLON
            {
              $$ = new Node("ArrayPositionAssignOp", "", yylineno);
              $$->children.push_back($1);
              $$->children.push_back($3);
              $$->children.push_back($6);
            }
         ;

expression_inf: expression {$$ = $1; /* Simply return the expression */}
    | expression COMMA expression_inf {
        $$ = new Node("LIST", "params", yylineno);
        $$->children.push_back($1);
        $$->children.push_back($3);
    }
    ;

expression: expression PLUSOP expression {      /*
                                                  Create a subtree that corresponds to the AddExpression
                                                  The root of the subtree is AddExpression
                                                  The children of the AddExpression subtree are the left hand side (expression accessed through $1) and right hand side of the expression (expression accessed through $3)
                                                */
                            $$ = new Node("AddExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r1 "); */
                          }
            | expression MINUSOP expression {
                            $$ = new Node("SubExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r2 "); */
                          }
            | expression MULTOP expression {
                            $$ = new Node("MultExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r3 "); */
                          }
            | expression DIVOP expression {
                            $$ = new Node("DivExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                            /* printf("r3 "); */
                          }
            | expression EQOP expression {
                            $$ = new Node("EqualExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression GTOP expression {
                            $$ = new Node("GreaterThanExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression LTOP expression {
                            $$ = new Node("LesserThanExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression ANDOP expression {
                            $$ = new Node("LogicalAndExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression OROP expression {
                            $$ = new Node("LogicalOrExpression", "", yylineno);
                            $$->children.push_back($1);
                            $$->children.push_back($3);
                          }
            | expression LBRACKET expression RBRACKET
              {
                $$ = new Node("Array access", "", yylineno);
                $$->children.push_back($1);
                $$->children.push_back($3);
              }
            | expression DOT LENGTH 
              {
                $$ = new Node("Expression", "length", yylineno);
                $$->children.push_back($1);
              }
            | expression DOT identifier LP RP
              { 
                /* First case of ? where no input*/
                $$ = new Node("ExpressionCallMethod", "", yylineno);
                $$->children.push_back($1);
                $$->children.push_back($3);
              }
            | expression DOT identifier LP expression_inf RP
              {
                /* Last case of certain input */
                $$ = new Node("ExpressionCallMethod", "", yylineno);
                $$->children.push_back($1);
                $$->children.push_back($3);
                $$->children.push_back($5);
              }

            | TRUE {$$ = new Node("Boolean", "true", yylineno);}
            | FALSE {$$ = new Node("Boolean", "false", yylineno);}
            | identifier {$$ = $1;}
            | THIS {$$ = new Node("this", "", yylineno);}
            | NEW INTDEC LBRACKET expression RBRACKET
              {
                $$ = new Node("NewArray", "int", yylineno);
                $$->children.push_back($4);
              }
            | NEW identifier LP RP
              {
                $$ = new Node("NewIstance", "", yylineno);
                $$->children.push_back($2);
              }
            | NOTOP expression
              {
                $$ = new Node("NotOperation", "", yylineno);
                $$->children.push_back($2);
              }
            | factor      {$$ = $1; /* printf("r4 ");*/}
            ;

identifier: ID            { $$ = new Node("Identifier: ", $1, yylineno);}
    ;

factor:     INT           {  $$ = new Node("Int", $1, yylineno); /* printf("r5 ");  Here we create a leaf node Int. The value of the leaf node is $1 */}
            | LP expression RP { $$ = $2; /* printf("r6 ");  simply return the expression */}
    ;
