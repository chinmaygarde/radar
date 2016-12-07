
%{

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "RLDecls.h"

#define CURRENT_LOC driver.location
#define FOUND(token) return rl::SVGPathParser::make_##token(CURRENT_LOC);
#define FOUND_ARG(token, arg) return rl::SVGPathParser::make_##token(arg, CURRENT_LOC);

%}

%option 8bit noyywrap nounput batch debug noinput never-interactive reentrant

%option outfile="SVGPathLexer.cc"
%option header-file="SVGPathLexer.h"
%option prefix="RLSVGPath_"

NUMBER_COMPS                  [0-9\+\-\.eE]
SEPARATORS                    [, \t\v\n\f]

%%

%{
  CURRENT_LOC.step();
%}

<<EOF>>                FOUND(END)
{NUMBER_COMPS}*        FOUND_ARG(NUMBER, yytext)
"M"                    FOUND(MOVE_ABS)
"m"                    FOUND(MOVE_REL)
"z"                    FOUND(CLOSE)
"Z"                    FOUND(CLOSE)
"L"                    FOUND(LINE_ABS)
"l"                    FOUND(LINE_REL)
"H"                    FOUND(LINE_HORIZONTAL_ABS)
"h"                    FOUND(LINE_HORIZONTAL_REL)
"V"                    FOUND(LINE_VERTICAL_ABS)
"v"                    FOUND(LINE_VERTICAL_REL)
"C"                    FOUND(CURVE_ABS)
"c"                    FOUND(CURVE_REL)
"S"                    FOUND(SHORTHAND_CURVE_ABS)
"s"                    FOUND(SHORTHAND_CURVE_REL)
"Q"                    FOUND(QUAD_CURVE_ABS)
"q"                    FOUND(QUAD_CURVE_REL)
"T"                    FOUND(SHORTHAND_QUAD_CURVE_ABS)
"t"                    FOUND(SHORTHAND_QUAD_CURVE_REL)
"A"                    FOUND(ELLIPTIC_ARC_ABS)
"a"                    FOUND(ELLIPTIC_ARC_REL)
{SEPARATORS}+          {  /* Separators Consumed */  }
.                      FOUND(INVALID)

%%
