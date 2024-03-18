// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 13 "parser.yy"

  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;

#line 54 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 127 "parser.tab.cc"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_classDeclaration_inf: // classDeclaration_inf
      case symbol_kind::S_mainClass: // mainClass
      case symbol_kind::S_classDeclaration: // classDeclaration
      case symbol_kind::S_varDeclaration_inf: // varDeclaration_inf
      case symbol_kind::S_varDeclaration: // varDeclaration
      case symbol_kind::S_methodDeclaration_inf: // methodDeclaration_inf
      case symbol_kind::S_methodDeclaration: // methodDeclaration
      case symbol_kind::S_methodHeader: // methodHeader
      case symbol_kind::S_methodParam: // methodParam
      case symbol_kind::S_methodContent: // methodContent
      case symbol_kind::S_methodContentParam: // methodContentParam
      case symbol_kind::S_methodContentItem: // methodContentItem
      case symbol_kind::S_type: // type
      case symbol_kind::S_statement_inf: // statement_inf
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression_inf: // expression_inf
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_factor: // factor
        value.YY_MOVE_OR_COPY< Node * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_EQOP: // EQOP
      case symbol_kind::S_LTOP: // LTOP
      case symbol_kind::S_GTOP: // GTOP
      case symbol_kind::S_ANDOP: // ANDOP
      case symbol_kind::S_OROP: // OROP
      case symbol_kind::S_NOTOP: // NOTOP
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_ASSIGNOP: // ASSIGNOP
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRINGDEC: // STRINGDEC
      case symbol_kind::S_INTDEC: // INTDEC
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSPRINTLN: // SYSPRINTLN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_RETURN: // RETURN
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_classDeclaration_inf: // classDeclaration_inf
      case symbol_kind::S_mainClass: // mainClass
      case symbol_kind::S_classDeclaration: // classDeclaration
      case symbol_kind::S_varDeclaration_inf: // varDeclaration_inf
      case symbol_kind::S_varDeclaration: // varDeclaration
      case symbol_kind::S_methodDeclaration_inf: // methodDeclaration_inf
      case symbol_kind::S_methodDeclaration: // methodDeclaration
      case symbol_kind::S_methodHeader: // methodHeader
      case symbol_kind::S_methodParam: // methodParam
      case symbol_kind::S_methodContent: // methodContent
      case symbol_kind::S_methodContentParam: // methodContentParam
      case symbol_kind::S_methodContentItem: // methodContentItem
      case symbol_kind::S_type: // type
      case symbol_kind::S_statement_inf: // statement_inf
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression_inf: // expression_inf
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_factor: // factor
        value.move< Node * > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_EQOP: // EQOP
      case symbol_kind::S_LTOP: // LTOP
      case symbol_kind::S_GTOP: // GTOP
      case symbol_kind::S_ANDOP: // ANDOP
      case symbol_kind::S_OROP: // OROP
      case symbol_kind::S_NOTOP: // NOTOP
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_ASSIGNOP: // ASSIGNOP
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRINGDEC: // STRINGDEC
      case symbol_kind::S_INTDEC: // INTDEC
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSPRINTLN: // SYSPRINTLN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_RETURN: // RETURN
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_classDeclaration_inf: // classDeclaration_inf
      case symbol_kind::S_mainClass: // mainClass
      case symbol_kind::S_classDeclaration: // classDeclaration
      case symbol_kind::S_varDeclaration_inf: // varDeclaration_inf
      case symbol_kind::S_varDeclaration: // varDeclaration
      case symbol_kind::S_methodDeclaration_inf: // methodDeclaration_inf
      case symbol_kind::S_methodDeclaration: // methodDeclaration
      case symbol_kind::S_methodHeader: // methodHeader
      case symbol_kind::S_methodParam: // methodParam
      case symbol_kind::S_methodContent: // methodContent
      case symbol_kind::S_methodContentParam: // methodContentParam
      case symbol_kind::S_methodContentItem: // methodContentItem
      case symbol_kind::S_type: // type
      case symbol_kind::S_statement_inf: // statement_inf
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression_inf: // expression_inf
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_factor: // factor
        value.copy< Node * > (that.value);
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_EQOP: // EQOP
      case symbol_kind::S_LTOP: // LTOP
      case symbol_kind::S_GTOP: // GTOP
      case symbol_kind::S_ANDOP: // ANDOP
      case symbol_kind::S_OROP: // OROP
      case symbol_kind::S_NOTOP: // NOTOP
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_ASSIGNOP: // ASSIGNOP
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRINGDEC: // STRINGDEC
      case symbol_kind::S_INTDEC: // INTDEC
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSPRINTLN: // SYSPRINTLN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_RETURN: // RETURN
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_classDeclaration_inf: // classDeclaration_inf
      case symbol_kind::S_mainClass: // mainClass
      case symbol_kind::S_classDeclaration: // classDeclaration
      case symbol_kind::S_varDeclaration_inf: // varDeclaration_inf
      case symbol_kind::S_varDeclaration: // varDeclaration
      case symbol_kind::S_methodDeclaration_inf: // methodDeclaration_inf
      case symbol_kind::S_methodDeclaration: // methodDeclaration
      case symbol_kind::S_methodHeader: // methodHeader
      case symbol_kind::S_methodParam: // methodParam
      case symbol_kind::S_methodContent: // methodContent
      case symbol_kind::S_methodContentParam: // methodContentParam
      case symbol_kind::S_methodContentItem: // methodContentItem
      case symbol_kind::S_type: // type
      case symbol_kind::S_statement_inf: // statement_inf
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression_inf: // expression_inf
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_factor: // factor
        value.move< Node * > (that.value);
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_EQOP: // EQOP
      case symbol_kind::S_LTOP: // LTOP
      case symbol_kind::S_GTOP: // GTOP
      case symbol_kind::S_ANDOP: // ANDOP
      case symbol_kind::S_OROP: // OROP
      case symbol_kind::S_NOTOP: // NOTOP
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_ASSIGNOP: // ASSIGNOP
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRINGDEC: // STRINGDEC
      case symbol_kind::S_INTDEC: // INTDEC
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSPRINTLN: // SYSPRINTLN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_RETURN: // RETURN
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_classDeclaration_inf: // classDeclaration_inf
      case symbol_kind::S_mainClass: // mainClass
      case symbol_kind::S_classDeclaration: // classDeclaration
      case symbol_kind::S_varDeclaration_inf: // varDeclaration_inf
      case symbol_kind::S_varDeclaration: // varDeclaration
      case symbol_kind::S_methodDeclaration_inf: // methodDeclaration_inf
      case symbol_kind::S_methodDeclaration: // methodDeclaration
      case symbol_kind::S_methodHeader: // methodHeader
      case symbol_kind::S_methodParam: // methodParam
      case symbol_kind::S_methodContent: // methodContent
      case symbol_kind::S_methodContentParam: // methodContentParam
      case symbol_kind::S_methodContentItem: // methodContentItem
      case symbol_kind::S_type: // type
      case symbol_kind::S_statement_inf: // statement_inf
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_expression_inf: // expression_inf
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_identifier: // identifier
      case symbol_kind::S_factor: // factor
        yylhs.value.emplace< Node * > ();
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_MINUSOP: // MINUSOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_EQOP: // EQOP
      case symbol_kind::S_LTOP: // LTOP
      case symbol_kind::S_GTOP: // GTOP
      case symbol_kind::S_ANDOP: // ANDOP
      case symbol_kind::S_OROP: // OROP
      case symbol_kind::S_NOTOP: // NOTOP
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_ASSIGNOP: // ASSIGNOP
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_LBRACKET: // LBRACKET
      case symbol_kind::S_RBRACKET: // RBRACKET
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRINGDEC: // STRINGDEC
      case symbol_kind::S_INTDEC: // INTDEC
      case symbol_kind::S_BOOL: // BOOL
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSPRINTLN: // SYSPRINTLN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_RETURN: // RETURN
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // root: goal
#line 42 "parser.yy"
                 {root = yystack_[0].value.as < Node * > ();}
#line 843 "parser.tab.cc"
    break;

  case 3: // goal: mainClass "end of file"
#line 45 "parser.yy"
      {
        yylhs.value.as < Node * > () = new Node("Program", "Start", yylineno);
        yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
        root = yylhs.value.as < Node * > ();
      }
#line 853 "parser.tab.cc"
    break;

  case 4: // goal: mainClass classDeclaration_inf "end of file"
#line 51 "parser.yy"
      {
        yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
        yylhs.value.as < Node * > ()->children.push_front(yystack_[2].value.as < Node * > ());
        // $$ = new Node("Program", "", yylineno);
        // $$->children.push_back($1);
        // $$->children.push_back($2);
        root = yylhs.value.as < Node * > ();
      }
#line 866 "parser.tab.cc"
    break;

  case 5: // classDeclaration_inf: classDeclaration
#line 63 "parser.yy"
                      {
                        yylhs.value.as < Node * > () = new Node("Program", "Start", yylineno);
                        yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                      }
#line 875 "parser.tab.cc"
    break;

  case 6: // classDeclaration_inf: classDeclaration_inf classDeclaration
#line 68 "parser.yy"
                      {
                        yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
                        yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                      }
#line 884 "parser.tab.cc"
    break;

  case 7: // mainClass: PUBLIC CLASS identifier LBRACE PUBLIC STATIC VOID MAIN LP STRINGDEC LBRACKET RBRACKET identifier RP LBRACE statement_inf RBRACE RBRACE
#line 75 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("Main Class", yystack_[15].value.as < Node * > ()->value, yylineno);
              // $$->children.push_back($3);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[5].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
            }
#line 895 "parser.tab.cc"
    break;

  case 8: // classDeclaration: CLASS identifier LBRACE RBRACE
#line 87 "parser.yy"
                  {
                    /* case with both 0 0 cases */
                    yylhs.value.as < Node * > () = new Node("No class body", "", yylineno);
                  }
#line 904 "parser.tab.cc"
    break;

  case 9: // classDeclaration: CLASS identifier LBRACE methodDeclaration_inf RBRACE
#line 92 "parser.yy"
                  {
                    /* case with 0 1-inf cases */
                    yylhs.value.as < Node * > () = new Node("Class", yystack_[3].value.as < Node * > ()->value, yylineno);
                    // $$->children.push_back($2);
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
                  }
#line 915 "parser.tab.cc"
    break;

  case 10: // classDeclaration: CLASS identifier LBRACE varDeclaration_inf RBRACE
#line 99 "parser.yy"
                  {
                    /* case with 1-inf 0 cases */
                    yylhs.value.as < Node * > () = new Node("Class", yystack_[3].value.as < Node * > ()->value, yylineno);
                    // $$->children.push_back($2);
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
                  }
#line 926 "parser.tab.cc"
    break;

  case 11: // classDeclaration: CLASS identifier LBRACE varDeclaration_inf methodDeclaration_inf RBRACE
#line 106 "parser.yy"
                  {
                    /* case with 1-inf 1-inf cases */
                    yylhs.value.as < Node * > () = new Node("Class", yystack_[4].value.as < Node * > ()->value, yylineno);
                    // $$->children.push_back($2);
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
                  }
#line 938 "parser.tab.cc"
    break;

  case 12: // varDeclaration_inf: varDeclaration
#line 116 "parser.yy"
                    {
                      yylhs.value.as < Node * > () = new Node("Var declarations", "", yylineno);
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                    }
#line 947 "parser.tab.cc"
    break;

  case 13: // varDeclaration_inf: varDeclaration_inf varDeclaration
#line 121 "parser.yy"
                    {
                      yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                    }
#line 956 "parser.tab.cc"
    break;

  case 14: // varDeclaration: type identifier SEMICOLON
#line 128 "parser.yy"
                {
                  yylhs.value.as < Node * > () = new Node("Var declaration", "", yylineno);
                  yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                  yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
                }
#line 966 "parser.tab.cc"
    break;

  case 15: // methodDeclaration_inf: methodDeclaration
#line 136 "parser.yy"
                        {
                          yylhs.value.as < Node * > () = new Node("MethodDeclaration", "", yylineno);
                          yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                        }
#line 975 "parser.tab.cc"
    break;

  case 16: // methodDeclaration_inf: methodDeclaration_inf methodDeclaration
#line 141 "parser.yy"
                        {
                          yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
                          yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                        }
#line 984 "parser.tab.cc"
    break;

  case 17: // methodDeclaration: PUBLIC type identifier LP RP methodContent
#line 149 "parser.yy"
                    {
                      /* First case of ? where no input (no parameters) */
                      yylhs.value.as < Node * > () = new Node("Method", yystack_[3].value.as < Node * > ()->value, yylineno);
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                    }
#line 995 "parser.tab.cc"
    break;

  case 18: // methodDeclaration: PUBLIC type identifier LP methodHeader RP methodContent
#line 156 "parser.yy"
                    {
                      /* Last case of certain input (parameters)*/
                      yylhs.value.as < Node * > () = new Node("Method", yystack_[4].value.as < Node * > ()->value, yylineno);
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[5].value.as < Node * > ());
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                      yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                    }
#line 1007 "parser.tab.cc"
    break;

  case 19: // methodHeader: methodParam
#line 165 "parser.yy"
                {
                  /*$$ = $1;  Simply return method param */
                  yylhs.value.as < Node * > () = new Node("Method parameters", "", yylineno);
                  yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                }
#line 1017 "parser.tab.cc"
    break;

  case 20: // methodHeader: methodHeader COMMA methodParam
#line 171 "parser.yy"
                  {
                    yylhs.value.as < Node * > () = yystack_[2].value.as < Node * > ();
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                  }
#line 1026 "parser.tab.cc"
    break;

  case 21: // methodParam: type identifier
#line 178 "parser.yy"
              {
                yylhs.value.as < Node * > () = new Node("Var declaration", "", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
                yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
              }
#line 1036 "parser.tab.cc"
    break;

  case 22: // methodContent: LBRACE RETURN expression SEMICOLON RBRACE
#line 187 "parser.yy"
                {
                  yylhs.value.as < Node * > () = new Node("Return", "", yylineno);
                  yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                }
#line 1045 "parser.tab.cc"
    break;

  case 23: // methodContent: LBRACE methodContentParam RETURN expression SEMICOLON RBRACE
#line 192 "parser.yy"
                {
                  yylhs.value.as < Node * > () = new Node("MethodContent", "", yylineno);
                  Node* returnNode = new Node("Return", "", yylineno);
                  returnNode->children.push_back(yystack_[2].value.as < Node * > ());

                  yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
                  yylhs.value.as < Node * > ()->children.push_back(returnNode);
                }
#line 1058 "parser.tab.cc"
    break;

  case 24: // methodContentParam: methodContentItem
#line 204 "parser.yy"
      {
        yylhs.value.as < Node * > () = new Node("Content", "", yylineno);
        yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
      }
#line 1067 "parser.tab.cc"
    break;

  case 25: // methodContentParam: methodContentParam methodContentItem
#line 209 "parser.yy"
      {
        yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
        yystack_[1].value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
      }
#line 1076 "parser.tab.cc"
    break;

  case 26: // methodContentItem: varDeclaration
#line 217 "parser.yy"
      {
        yylhs.value.as < Node * > () = yystack_[0].value.as < Node * > ();
      }
#line 1084 "parser.tab.cc"
    break;

  case 27: // methodContentItem: statement
#line 221 "parser.yy"
      {
        yylhs.value.as < Node * > () = new Node("Statement", "", yylineno);
        yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
      }
#line 1093 "parser.tab.cc"
    break;

  case 28: // type: INTDEC LBRACKET RBRACKET
#line 227 "parser.yy"
                               {yylhs.value.as < Node * > () = new Node("IntArrayType", "int[]", yylineno);}
#line 1099 "parser.tab.cc"
    break;

  case 29: // type: BOOL
#line 228 "parser.yy"
           {yylhs.value.as < Node * > () = new Node("BooleanType", "boolean", yylineno);}
#line 1105 "parser.tab.cc"
    break;

  case 30: // type: INTDEC
#line 229 "parser.yy"
             {yylhs.value.as < Node * > () = new Node("IntegerType", "int", yylineno);}
#line 1111 "parser.tab.cc"
    break;

  case 31: // type: identifier
#line 230 "parser.yy"
                 {yylhs.value.as < Node * > () = new Node("IdentifierType", yystack_[0].value.as < Node * > ()->value, yylineno);}
#line 1117 "parser.tab.cc"
    break;

  case 32: // statement_inf: statement
#line 234 "parser.yy"
               {
                  yylhs.value.as < Node * > () = new Node("Statements", "", yylineno);
                  yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
               }
#line 1126 "parser.tab.cc"
    break;

  case 33: // statement_inf: statement_inf statement
#line 239 "parser.yy"
               {
                  if (yystack_[1].value.as < Node * > ()->type == "Statements") {
                    // Already a Statements node, just add the new statement
                    yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                  } else {
                    // Create a new Statements node and add both statements
                    yylhs.value.as < Node * > () = new Node("Statements", "", yylineno);
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                  }
               }
#line 1143 "parser.tab.cc"
    break;

  case 34: // statement: LBRACE RBRACE
#line 253 "parser.yy"
                         {yylhs.value.as < Node * > () = new Node("Empty statement", "", yylineno);}
#line 1149 "parser.tab.cc"
    break;

  case 35: // statement: LBRACE statement_inf RBRACE
#line 254 "parser.yy"
                                       {yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > ();}
#line 1155 "parser.tab.cc"
    break;

  case 36: // statement: IF LP expression RP statement
#line 256 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("If-Expression-Statement", "", yylineno);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
            }
#line 1165 "parser.tab.cc"
    break;

  case 37: // statement: IF LP expression RP statement ELSE statement
#line 262 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("If/Else-Expression-Statement", "", yylineno);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
            }
#line 1176 "parser.tab.cc"
    break;

  case 38: // statement: WHILE LP expression RP statement
#line 269 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("While-Statement", "", yylineno);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
            }
#line 1186 "parser.tab.cc"
    break;

  case 39: // statement: SYSPRINTLN LP expression RP SEMICOLON
#line 275 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("SysPrintLn", "", yylineno);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
            }
#line 1195 "parser.tab.cc"
    break;

  case 40: // statement: identifier ASSIGNOP expression SEMICOLON
#line 280 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("Identifier assign", "", yylineno);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
            }
#line 1205 "parser.tab.cc"
    break;

  case 41: // statement: identifier LBRACKET expression RBRACKET ASSIGNOP expression SEMICOLON
#line 286 "parser.yy"
            {
              yylhs.value.as < Node * > () = new Node("ArrayPositionAssignOp", "", yylineno);
              yylhs.value.as < Node * > ()->children.push_back(yystack_[6].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
              yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
            }
#line 1216 "parser.tab.cc"
    break;

  case 42: // expression_inf: expression
#line 295 "parser.yy"
                  {
                    yylhs.value.as < Node * > () = new Node("Expressions", "", yylineno);
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                  }
#line 1225 "parser.tab.cc"
    break;

  case 43: // expression_inf: expression_inf COMMA expression
#line 300 "parser.yy"
                  {
                    yylhs.value.as < Node * > () = yystack_[2].value.as < Node * > ();
                    yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                  }
#line 1234 "parser.tab.cc"
    break;

  case 44: // expression: expression PLUSOP expression
#line 306 "parser.yy"
                                         {      /*
                                                  Create a subtree that corresponds to the AddExpression
                                                  The root of the subtree is AddExpression
                                                  The children of the AddExpression subtree are the left hand side (expression accessed through $1) and right hand side of the expression (expression accessed through $3)
                                                */
                            yylhs.value.as < Node * > () = new Node("AddExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                            /* printf("r1 "); */
                          }
#line 1249 "parser.tab.cc"
    break;

  case 45: // expression: expression MINUSOP expression
#line 316 "parser.yy"
                                            {
                            yylhs.value.as < Node * > () = new Node("SubExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                            /* printf("r2 "); */
                          }
#line 1260 "parser.tab.cc"
    break;

  case 46: // expression: expression MULTOP expression
#line 322 "parser.yy"
                                           {
                            yylhs.value.as < Node * > () = new Node("MultExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                            /* printf("r3 "); */
                          }
#line 1271 "parser.tab.cc"
    break;

  case 47: // expression: expression DIVOP expression
#line 328 "parser.yy"
                                          {
                            yylhs.value.as < Node * > () = new Node("DivExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                            /* printf("r3 "); */
                          }
#line 1282 "parser.tab.cc"
    break;

  case 48: // expression: expression EQOP expression
#line 334 "parser.yy"
                                         {
                            yylhs.value.as < Node * > () = new Node("EqualExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                          }
#line 1292 "parser.tab.cc"
    break;

  case 49: // expression: expression GTOP expression
#line 339 "parser.yy"
                                         {
                            yylhs.value.as < Node * > () = new Node("GreaterThanExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                          }
#line 1302 "parser.tab.cc"
    break;

  case 50: // expression: expression LTOP expression
#line 344 "parser.yy"
                                         {
                            yylhs.value.as < Node * > () = new Node("LesserThanExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                          }
#line 1312 "parser.tab.cc"
    break;

  case 51: // expression: expression ANDOP expression
#line 349 "parser.yy"
                                          {
                            yylhs.value.as < Node * > () = new Node("LogicalAndExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                          }
#line 1322 "parser.tab.cc"
    break;

  case 52: // expression: expression OROP expression
#line 354 "parser.yy"
                                         {
                            yylhs.value.as < Node * > () = new Node("LogicalOrExpression", "", yylineno);
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
                            yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
                          }
#line 1332 "parser.tab.cc"
    break;

  case 53: // expression: expression LBRACKET expression RBRACKET
#line 360 "parser.yy"
              {
                yylhs.value.as < Node * > () = new Node("Array access", "", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ());
                yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
              }
#line 1342 "parser.tab.cc"
    break;

  case 54: // expression: expression DOT LENGTH
#line 366 "parser.yy"
              {
                yylhs.value.as < Node * > () = new Node("Expression", "length", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
              }
#line 1351 "parser.tab.cc"
    break;

  case 55: // expression: expression DOT identifier LP RP
#line 371 "parser.yy"
              { 
                /* First case of ? where no input*/
                yylhs.value.as < Node * > () = new Node("ExpressionCallMethod", "", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[4].value.as < Node * > ());
                yylhs.value.as < Node * > ()->children.push_back(yystack_[2].value.as < Node * > ());
              }
#line 1362 "parser.tab.cc"
    break;

  case 56: // expression: expression DOT identifier LP expression_inf RP
#line 378 "parser.yy"
              {
                /* Last case of certain input */
                yylhs.value.as < Node * > () = new Node("ExpressionCallMethod", "", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[5].value.as < Node * > ());
                yylhs.value.as < Node * > ()->children.push_back(yystack_[3].value.as < Node * > ());
                yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
              }
#line 1374 "parser.tab.cc"
    break;

  case 57: // expression: TRUE
#line 386 "parser.yy"
                   {yylhs.value.as < Node * > () = new Node("boolean", "true", yylineno);}
#line 1380 "parser.tab.cc"
    break;

  case 58: // expression: FALSE
#line 387 "parser.yy"
                    {yylhs.value.as < Node * > () = new Node("boolean", "false", yylineno);}
#line 1386 "parser.tab.cc"
    break;

  case 59: // expression: identifier
#line 389 "parser.yy"
                         {yylhs.value.as < Node * > () = yystack_[0].value.as < Node * > ();}
#line 1392 "parser.tab.cc"
    break;

  case 60: // expression: THIS
#line 390 "parser.yy"
                   {yylhs.value.as < Node * > () = new Node("this", "this", yylineno);}
#line 1398 "parser.tab.cc"
    break;

  case 61: // expression: NEW INTDEC LBRACKET expression RBRACKET
#line 392 "parser.yy"
              {
                yylhs.value.as < Node * > () = new Node("NewArray", "int", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[1].value.as < Node * > ());
              }
#line 1407 "parser.tab.cc"
    break;

  case 62: // expression: NEW identifier LP RP
#line 397 "parser.yy"
              {
                yylhs.value.as < Node * > () = new Node("NewIstance", yystack_[2].value.as < Node * > ()->value, yylineno);
              }
#line 1415 "parser.tab.cc"
    break;

  case 63: // expression: NOTOP expression
#line 401 "parser.yy"
              {
                yylhs.value.as < Node * > () = new Node("NotOperation", "", yylineno);
                yylhs.value.as < Node * > ()->children.push_back(yystack_[0].value.as < Node * > ());
              }
#line 1424 "parser.tab.cc"
    break;

  case 64: // expression: factor
#line 405 "parser.yy"
                          {yylhs.value.as < Node * > () = yystack_[0].value.as < Node * > (); /* printf("r4 ");*/}
#line 1430 "parser.tab.cc"
    break;

  case 65: // identifier: ID
#line 408 "parser.yy"
                          { yylhs.value.as < Node * > () = new Node("Identifier", yystack_[0].value.as < std::string > (), yylineno);}
#line 1436 "parser.tab.cc"
    break;

  case 66: // factor: INT
#line 411 "parser.yy"
                          {  yylhs.value.as < Node * > () = new Node("int", yystack_[0].value.as < std::string > (), yylineno); /* printf("r5 ");  Here we create a leaf node Int. The value of the leaf node is $1 */}
#line 1442 "parser.tab.cc"
    break;

  case 67: // factor: LP expression RP
#line 412 "parser.yy"
                               { yylhs.value.as < Node * > () = yystack_[1].value.as < Node * > (); /* printf("r6 ");  simply return the expression */}
#line 1448 "parser.tab.cc"
    break;


#line 1452 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -56;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -12,    -3,    36,   -56,     6,    -8,   -56,   -56,    -8,     8,
     -56,   -56,     9,    47,   -56,   -56,    56,   197,    70,   -56,
     -21,    61,   -56,   199,   -56,    22,   -56,    -8,   -56,    81,
      -8,    86,   -56,   -56,    77,   -56,   -56,    94,    96,   106,
     -56,   -56,   -56,   117,    -4,    99,   109,    50,   -56,    -8,
     108,    45,   -56,   109,   -21,   -56,   110,   126,   123,   124,
     128,    16,   -56,   107,   -56,   -56,    88,   -56,   -56,    -8,
     -56,   131,   -56,    88,    16,    16,    16,   -56,    16,    16,
     -56,   -56,   -56,    20,   231,   -56,   -56,    16,   -56,    16,
      16,   134,   -56,   -56,   246,   260,   274,   288,   -15,   118,
     142,    16,    16,    16,    16,    16,    16,    16,    16,    16,
     -22,   132,    16,   302,   317,   175,   135,    65,    65,   137,
     -56,    16,   155,    29,    29,   -15,   -15,   389,   403,   403,
     375,   361,   -56,   161,   -56,   189,   153,   -56,   156,    65,
     143,   -56,   -56,   203,   -56,    33,   -56,   -56,    16,   136,
      65,   -56,   -56,    51,   347,   332,   162,   -56,   -56,    16,
     -56,   -56,   347
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     2,     0,     0,     1,     3,     0,     0,
       5,    65,     0,     0,     4,     6,     0,     0,     0,     8,
       0,    30,    29,     0,    12,     0,    15,     0,    31,     0,
       0,     0,    10,    13,     0,     9,    16,     0,     0,     0,
      28,    11,    14,     0,     0,     0,     0,     0,    19,     0,
       0,     0,    17,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    26,     0,    24,    27,    31,    18,    20,     0,
      34,     0,    32,     0,     0,     0,     0,    66,     0,     0,
      57,    58,    60,     0,     0,    59,    64,     0,    25,     0,
       0,     0,    35,    33,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      67,     0,     0,    44,    45,    46,    47,    48,    50,    49,
      51,    52,    54,     0,    22,     0,     0,    40,     0,     0,
      36,    38,    39,     0,    62,     0,    53,    23,     0,     0,
       0,    61,    55,     0,    42,     0,     0,    37,    56,     0,
      41,     7,    43
  };

  const short
  parser::yypgoto_[] =
  {
     -56,   -56,   -56,   -56,   -56,   173,   -56,    -6,   167,    57,
     -56,   130,   151,   -56,   133,   -16,    71,   -55,   -56,    14,
      -5,   -56
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     9,     4,    10,    23,    62,    25,    26,
      47,    48,    52,    63,    64,    27,    71,    65,   153,    84,
      85,    86
  };

  const unsigned char
  parser::yytable_[] =
  {
      12,   110,    72,    13,    30,    46,     7,   112,    14,    21,
      22,    24,    28,     1,   132,    28,    93,    33,    28,    11,
      11,     5,    37,    77,    78,    39,    21,    22,    49,    16,
       8,    79,     8,    11,   103,   104,     6,    11,    49,    28,
      77,    78,   152,    35,    55,   110,    66,    20,    79,    28,
      99,   112,    73,    80,    81,    82,    83,    11,    66,    53,
     158,    11,   140,   141,    91,    57,    73,    17,    54,   159,
      80,    81,    82,    83,    11,    21,    22,    58,   100,    59,
      60,    18,    36,    31,    72,    57,    11,    61,    94,    95,
      96,    36,    97,    98,    93,   157,    29,    58,    41,    59,
      60,   113,    20,   114,   115,   133,    11,    89,    38,    40,
      90,    42,    73,    73,    44,   123,   124,   125,   126,   127,
     128,   129,   130,   131,    43,    45,   135,    57,    50,    51,
      56,    74,    75,    69,    73,   143,    76,    21,    22,    58,
     121,    59,    60,   116,    73,    73,    57,    70,    11,    87,
     122,    57,    92,   134,   142,   139,    57,   156,    58,   154,
      59,    60,   155,    58,   144,    59,    60,    11,    58,   145,
      59,    60,    11,   162,   147,   148,   150,    11,   101,   102,
     103,   104,    15,   161,    68,   105,   106,   107,   108,   109,
      34,   110,   101,   102,   103,   104,    88,   112,   138,   105,
     106,   107,   108,   109,    67,   110,   101,   102,   103,   104,
     149,   112,   146,   105,   106,   107,   108,   109,    19,   110,
      32,     0,    20,     0,    20,   112,   151,    21,    22,    21,
      22,     0,     0,     0,   101,   102,   103,   104,    11,     0,
      11,   105,   106,   107,   108,   109,     0,   110,   111,   101,
     102,   103,   104,   112,     0,   117,   105,   106,   107,   108,
     109,     0,   110,   101,   102,   103,   104,     0,   112,   118,
     105,   106,   107,   108,   109,     0,   110,   101,   102,   103,
     104,     0,   112,   119,   105,   106,   107,   108,   109,     0,
     110,   101,   102,   103,   104,     0,   112,   120,   105,   106,
     107,   108,   109,     0,   110,   101,   102,   103,   104,     0,
     112,     0,   105,   106,   107,   108,   109,     0,   110,   136,
     101,   102,   103,   104,   112,     0,     0,   105,   106,   107,
     108,   109,     0,   110,   137,   101,   102,   103,   104,   112,
       0,     0,   105,   106,   107,   108,   109,     0,   110,   160,
     101,   102,   103,   104,   112,     0,     0,   105,   106,   107,
     108,   109,     0,   110,   101,   102,   103,   104,     0,   112,
       0,   105,   106,   107,   108,     0,     0,   110,   101,   102,
     103,   104,     0,   112,     0,   105,   106,   107,     0,     0,
       0,   110,   101,   102,   103,   104,     0,   112,     0,     0,
     106,   107,     0,     0,     0,   110,   101,   102,   103,   104,
       0,   112,     0,     0,     0,     0,     0,     0,     0,   110,
       0,     0,     0,     0,     0,   112
  };

  const short
  parser::yycheck_[] =
  {
       5,    16,    57,     8,    20,     9,     0,    22,     0,    30,
      31,    17,    17,    25,    36,    20,    71,    23,    23,    41,
      41,    24,    27,     7,     8,    30,    30,    31,    44,    20,
      24,    15,    24,    41,     5,     6,     0,    41,    54,    44,
       7,     8,     9,    21,    49,    16,    51,    25,    15,    54,
      30,    22,    57,    37,    38,    39,    40,    41,    63,     9,
       9,    41,   117,   118,    69,    20,    71,    20,    18,    18,
      37,    38,    39,    40,    41,    30,    31,    32,    83,    34,
      35,    25,    25,    22,   139,    20,    41,    42,    74,    75,
      76,    34,    78,    79,   149,   150,    26,    32,    21,    34,
      35,    87,    25,    89,    90,   110,    41,    19,    27,    23,
      22,    17,   117,   118,     8,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    28,     8,   112,    20,    29,    20,
      22,     8,     8,    23,   139,   121,     8,    30,    31,    32,
      22,    34,    35,     9,   149,   150,    20,    21,    41,    42,
       8,    20,    21,    21,    17,    20,    20,    21,    32,   145,
      34,    35,   148,    32,     9,    34,    35,    41,    32,     8,
      34,    35,    41,   159,    21,    19,    33,    41,     3,     4,
       5,     6,     9,    21,    54,    10,    11,    12,    13,    14,
      23,    16,     3,     4,     5,     6,    63,    22,    23,    10,
      11,    12,    13,    14,    53,    16,     3,     4,     5,     6,
     139,    22,    23,    10,    11,    12,    13,    14,    21,    16,
      21,    -1,    25,    -1,    25,    22,    23,    30,    31,    30,
      31,    -1,    -1,    -1,     3,     4,     5,     6,    41,    -1,
      41,    10,    11,    12,    13,    14,    -1,    16,    17,     3,
       4,     5,     6,    22,    -1,     9,    10,    11,    12,    13,
      14,    -1,    16,     3,     4,     5,     6,    -1,    22,     9,
      10,    11,    12,    13,    14,    -1,    16,     3,     4,     5,
       6,    -1,    22,     9,    10,    11,    12,    13,    14,    -1,
      16,     3,     4,     5,     6,    -1,    22,     9,    10,    11,
      12,    13,    14,    -1,    16,     3,     4,     5,     6,    -1,
      22,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
       3,     4,     5,     6,    22,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,     3,     4,     5,     6,    22,
      -1,    -1,    10,    11,    12,    13,    14,    -1,    16,    17,
       3,     4,     5,     6,    22,    -1,    -1,    10,    11,    12,
      13,    14,    -1,    16,     3,     4,     5,     6,    -1,    22,
      -1,    10,    11,    12,    13,    -1,    -1,    16,     3,     4,
       5,     6,    -1,    22,    -1,    10,    11,    12,    -1,    -1,
      -1,    16,     3,     4,     5,     6,    -1,    22,    -1,    -1,
      11,    12,    -1,    -1,    -1,    16,     3,     4,     5,     6,
      -1,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    22
  };

  const signed char
  parser::yystos_[] =
  {
       0,    25,    44,    45,    47,    24,     0,     0,    24,    46,
      48,    41,    63,    63,     0,    48,    20,    20,    25,    21,
      25,    30,    31,    49,    50,    51,    52,    58,    63,    26,
      58,    22,    21,    50,    51,    21,    52,    63,    27,    63,
      23,    21,    17,    28,     8,     8,     9,    53,    54,    58,
      29,    20,    55,     9,    18,    63,    22,    20,    32,    34,
      35,    42,    50,    56,    57,    60,    63,    55,    54,    23,
      21,    59,    60,    63,     8,     8,     8,     7,     8,    15,
      37,    38,    39,    40,    62,    63,    64,    42,    57,    19,
      22,    63,    21,    60,    62,    62,    62,    62,    62,    30,
      63,     3,     4,     5,     6,    10,    11,    12,    13,    14,
      16,    17,    22,    62,    62,    62,     9,     9,     9,     9,
       9,    22,     8,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    36,    63,    21,    62,    17,    17,    23,    20,
      60,    60,    17,    62,     9,     8,    23,    21,    19,    59,
      33,    23,     9,    61,    62,    62,    21,    60,     9,    18,
      17,    21,    62
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    43,    44,    45,    45,    46,    46,    47,    48,    48,
      48,    48,    49,    49,    50,    51,    51,    52,    52,    53,
      53,    54,    55,    55,    56,    56,    57,    57,    58,    58,
      58,    58,    59,    59,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    63,    64,    64
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     3,     1,     2,    18,     4,     5,
       5,     6,     1,     2,     3,     1,     2,     6,     7,     1,
       3,     2,     5,     6,     1,     2,     1,     1,     3,     1,
       1,     1,     1,     2,     2,     3,     5,     7,     5,     5,
       4,     7,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     3,     5,     6,     1,     1,     1,
       1,     5,     4,     2,     1,     1,     1,     3
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "PLUSOP", "MINUSOP",
  "MULTOP", "DIVOP", "INT", "LP", "RP", "EQOP", "LTOP", "GTOP", "ANDOP",
  "OROP", "NOTOP", "DOT", "SEMICOLON", "COMMA", "ASSIGNOP", "LBRACE",
  "RBRACE", "LBRACKET", "RBRACKET", "CLASS", "PUBLIC", "STATIC", "VOID",
  "MAIN", "STRINGDEC", "INTDEC", "BOOL", "IF", "ELSE", "WHILE",
  "SYSPRINTLN", "LENGTH", "TRUE", "FALSE", "THIS", "NEW", "ID", "RETURN",
  "$accept", "root", "goal", "classDeclaration_inf", "mainClass",
  "classDeclaration", "varDeclaration_inf", "varDeclaration",
  "methodDeclaration_inf", "methodDeclaration", "methodHeader",
  "methodParam", "methodContent", "methodContentParam",
  "methodContentItem", "type", "statement_inf", "statement",
  "expression_inf", "expression", "identifier", "factor", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    42,    42,    44,    50,    62,    67,    74,    86,    91,
      98,   105,   115,   120,   127,   135,   140,   148,   155,   164,
     170,   177,   186,   191,   203,   208,   216,   220,   227,   228,
     229,   230,   233,   238,   253,   254,   255,   261,   268,   274,
     279,   285,   294,   299,   306,   316,   322,   328,   334,   339,
     344,   349,   354,   359,   365,   370,   377,   386,   387,   389,
     390,   391,   396,   400,   405,   408,   411,   412
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2071 "parser.tab.cc"

