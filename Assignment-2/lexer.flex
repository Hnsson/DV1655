%top{
    #include "parser.tab.hh"
    #define YY_DECL yy::parser::symbol_type yylex()
    #include "Node.h"
    int lexical_errors = 0;
}
%option yylineno noyywrap nounput batch noinput stack 
%%

"+"                     {if(USE_LEX_ONLY) {printf("PLUSOP ");} else {return yy::parser::make_PLUSOP(yytext);}}
"-"                     {if(USE_LEX_ONLY) {printf("MINUSOP ");} else {return yy::parser::make_MINUSOP(yytext);}}
"*"                     {if(USE_LEX_ONLY) {printf("MULTOP ");} else {return yy::parser::make_MULTOP(yytext);}}
"/"                     {if(USE_LEX_ONLY) {printf("DIVOP ");} else {return yy::parser::make_DIVOP(yytext);}}
"=="                    {if(USE_LEX_ONLY) {printf("EQOP ");} else {return yy::parser::make_EQOP(yytext);}}
"<"                     {if(USE_LEX_ONLY) {printf("LTOP ");} else {return yy::parser::make_LTOP(yytext);}}
">"                     {if(USE_LEX_ONLY) {printf("GTOP ");} else {return yy::parser::make_GTOP(yytext);}}
"||"                    {if(USE_LEX_ONLY) {printf("OROP ");} else {return yy::parser::make_OROP(yytext);}}
"&&"                    {if(USE_LEX_ONLY) {printf("ANDOP ");} else {return yy::parser::make_ANDOP(yytext);}}
"!"                     {if(USE_LEX_ONLY) {printf("NOTOP ");} else {return yy::parser::make_NOTOP(yytext);}}
"."                     {if(USE_LEX_ONLY) {printf("DOT ");} else {return yy::parser::make_DOT(yytext);}}
";"                     {if(USE_LEX_ONLY) {printf("SEMICOLON ");} else {return yy::parser::make_SEMICOLON(yytext);}}
","                     {if(USE_LEX_ONLY) {printf("COMMA ");} else {return yy::parser::make_COMMA(yytext);}}
"="                     {if(USE_LEX_ONLY) {printf("ASSIGNOP ");} else {return yy::parser::make_ASSIGNOP(yytext);}}
"("                     {if(USE_LEX_ONLY) {printf("LP ");} else {return yy::parser::make_LP(yytext);}}
")"                     {if(USE_LEX_ONLY) {printf("RP ");} else {return yy::parser::make_RP(yytext);}}
"{"                     {if(USE_LEX_ONLY) {printf("LBRACE ");} else {return yy::parser::make_LBRACE(yytext);}}
"}"                     {if(USE_LEX_ONLY) {printf("RBRACE ");} else {return yy::parser::make_RBRACE(yytext);}}
"["                     {if(USE_LEX_ONLY) {printf("LBRACKET ");} else {return yy::parser::make_LBRACKET(yytext);}}
"]"                     {if(USE_LEX_ONLY) {printf("RBRACKET ");} else {return yy::parser::make_RBRACKET(yytext);}}
"class"                 {if(USE_LEX_ONLY) {printf("CLASS ");} else {return yy::parser::make_CLASS(yytext);}}
"public"                {if(USE_LEX_ONLY) {printf("PUBLIC ");} else {return yy::parser::make_PUBLIC(yytext);}}
"static"                {if(USE_LEX_ONLY) {printf("STATIC ");} else {return yy::parser::make_STATIC(yytext);}}
"void"                  {if(USE_LEX_ONLY) {printf("VOID ");} else {return yy::parser::make_VOID(yytext);}}
"main"                  {if(USE_LEX_ONLY) {printf("MAIN ");} else {return yy::parser::make_MAIN(yytext);}}
"return"                {if(USE_LEX_ONLY) {printf("RETURN ");} else {return yy::parser::make_RETURN(yytext);}}
"String"                {if(USE_LEX_ONLY) {printf("STRINGDEC ");} else {return yy::parser::make_STRINGDEC(yytext);}}
"int"                   {if(USE_LEX_ONLY) {printf("INTDEC ");} else {return yy::parser::make_INTDEC(yytext);}}
"boolean"               {if(USE_LEX_ONLY) {printf("BOOL ");} else {return yy::parser::make_BOOL(yytext);}}
"if"                    {if(USE_LEX_ONLY) {printf("IF ");} else {return yy::parser::make_IF(yytext);}}
"else"                  {if(USE_LEX_ONLY) {printf("ELSE ");} else {return yy::parser::make_ELSE(yytext);}}
"while"                 {if(USE_LEX_ONLY) {printf("WHILE ");} else {return yy::parser::make_WHILE(yytext);}}
"System.out.println"    {if(USE_LEX_ONLY) {printf("SYSPRINTLN ");} else {return yy::parser::make_SYSPRINTLN(yytext);}}
"length"                {if(USE_LEX_ONLY) {printf("LENGTH ");} else {return yy::parser::make_LENGTH(yytext);}}
"true"                  {if(USE_LEX_ONLY) {printf("TRUE ");} else {return yy::parser::make_TRUE(yytext);}}
"false"                 {if(USE_LEX_ONLY) {printf("FALSE ");} else {return yy::parser::make_FALSE(yytext);}}
"this"                  {if(USE_LEX_ONLY) {printf("THIS ");} else {return yy::parser::make_THIS(yytext);}}
"new"                   {if(USE_LEX_ONLY) {printf("NEW ");} else {return yy::parser::make_NEW(yytext);}}

0|[1-9][0-9]*           {if(USE_LEX_ONLY) {printf("INT ");} else {return yy::parser::make_INT(yytext);}}
[a-zA-Z_][a-zA-Z0-9_]*    {if(USE_LEX_ONLY) {printf("ID ");} else {return yy::parser::make_ID(yytext);}}
"//"[^\n]*                {if(USE_LEX_ONLY) {printf("COMMENT ");}}

[ \t\n\r]+              {}
.                       { if(!lexical_errors) fprintf(stderr, "Lexical errors found! See the logs below: \n"); fprintf(stderr,"\t@error at line %d. Character %s is not recognized\n", yylineno, yytext); lexical_errors = 1;}
<<EOF>>                  {return yy::parser::make_END();}
%%