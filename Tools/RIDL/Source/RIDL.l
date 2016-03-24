
%{

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "RLDecls.h"

#define CURRENT_LOC driver.location

%}

%option 8bit noyywrap nounput batch debug noinput never-interactive reentrant

%option outfile="Lexer.cc"
%option header-file="Lexer.h"
%option prefix="RL_"

D                       [0-9]
L                       [a-zA-Z_]
A                       [a-zA-Z_0-9]
WS                      [ \t\v\n\f]

%%

%{
  CURRENT_LOC.step();
%}

<<EOF>>                return rl::Parser::make_END(CURRENT_LOC);

"//".*                 {  /* Comment Consumed */  }


"protocol"             return rl::Parser::make_PROTOCOL(CURRENT_LOC);
"void"                 return rl::Parser::make_VOID(CURRENT_LOC);


{L}{A}*                return rl::Parser::make_IDENTIFIER(yytext, CURRENT_LOC);


"."                    return rl::Parser::make_DOT(CURRENT_LOC);
","                    return rl::Parser::make_COMMA(CURRENT_LOC);
";"                    return rl::Parser::make_SEMI_COLON(CURRENT_LOC);
"{"                    return rl::Parser::make_CURLY_LEFT(CURRENT_LOC);
"}"                    return rl::Parser::make_CURLY_RIGHT(CURRENT_LOC);
"("                    return rl::Parser::make_PAREN_LEFT(CURRENT_LOC);
")"                    return rl::Parser::make_PAREN_RIGHT(CURRENT_LOC);
"["                    return rl::Parser::make_SQUARE_LEFT(CURRENT_LOC);
"]"                    return rl::Parser::make_SQUARE_RIGHT(CURRENT_LOC);


{WS}+                  {  /* Whitespace Consumed */  }
.                      return rl::Parser::make_INVALID_TOKEN(CURRENT_LOC);

%%
