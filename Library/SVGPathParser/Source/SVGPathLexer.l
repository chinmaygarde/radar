
%{

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "RLDecls.h"

#define CURRENT_LOC driver.location

%}

%option 8bit noyywrap nounput batch debug noinput never-interactive reentrant

%option outfile="SVGPathLexer.cc"
%option header-file="SVGPathLexer.h"
%option prefix="RLSVGPath_"

D                       [0-9]
L                       [a-zA-Z_]
A                       [a-zA-Z_0-9]
WS                      [ \t\v\n\f]

%%

%{
  CURRENT_LOC.step();
%}

<<EOF>>                return rl::SVGPathParser::make_END(CURRENT_LOC);

"//".*                 {  /* Comment Consumed */  }


"protocol"             return rl::SVGPathParser::make_PROTOCOL(CURRENT_LOC);
"void"                 return rl::SVGPathParser::make_VOID(CURRENT_LOC);


{L}{A}*                return rl::SVGPathParser::make_IDENTIFIER(yytext, CURRENT_LOC);


"."                    return rl::SVGPathParser::make_DOT(CURRENT_LOC);
","                    return rl::SVGPathParser::make_COMMA(CURRENT_LOC);
";"                    return rl::SVGPathParser::make_SEMI_COLON(CURRENT_LOC);
"{"                    return rl::SVGPathParser::make_CURLY_LEFT(CURRENT_LOC);
"}"                    return rl::SVGPathParser::make_CURLY_RIGHT(CURRENT_LOC);
"("                    return rl::SVGPathParser::make_PAREN_LEFT(CURRENT_LOC);
")"                    return rl::SVGPathParser::make_PAREN_RIGHT(CURRENT_LOC);
"["                    return rl::SVGPathParser::make_SQUARE_LEFT(CURRENT_LOC);
"]"                    return rl::SVGPathParser::make_SQUARE_RIGHT(CURRENT_LOC);


{WS}+                  {  /* Whitespace Consumed */  }
.                      return rl::SVGPathParser::make_INVALID_TOKEN(CURRENT_LOC);

%%
