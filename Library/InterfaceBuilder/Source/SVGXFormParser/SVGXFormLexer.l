
%{

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "RLDecls.h"
#include "SVGXFormElement.h"

#define CURRENT_LOC driver.location
#define FOUND(token) return rl::SVGXFormParser::make_##token(CURRENT_LOC);
#define FOUND_ARG(token, arg) return rl::SVGXFormParser::make_##token(arg, CURRENT_LOC);

%}

%option 8bit noyywrap nounput batch debug noinput never-interactive reentrant

%option outfile="SVGXFormLexer.cc"
%option header-file="SVGXFormLexer.h"
%option prefix="RLSVGXForm_"

NUMBER_COMPS                  [0-9\+\-\.eE]
SEPARATORS                    [, \t\v\n\f]

%%

%{
  CURRENT_LOC.step();
%}

<<EOF>>                FOUND(END)
{NUMBER_COMPS}*        FOUND_ARG(NUMBER, rl::SVGXFormNumberParse(yytext))
"matrix"               FOUND(MATRIX)
"translate"            FOUND(TRANSLATE)
"scale"                FOUND(SCALE)
"rotate"               FOUND(ROTATE)
"skewX"                FOUND(SKEW_X)
"skewY"                FOUND(SKEW_Y)
"("                    FOUND(LEFT_BRACE)
")"                    FOUND(RIGHT_BRACE)
{SEPARATORS}+          {  /* Separators Consumed */  }
.                      FOUND(INVALID)

%%
