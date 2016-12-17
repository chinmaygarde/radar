%skeleton "lalr1.cc"
%require "3.0.4"

%defines "SVGPathParser.h"
%output "SVGPathParser.cc"
%name-prefix "RLSVGPath_"
%define api.namespace {rl}
%define parser_class_name {SVGPathParser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%param { rl::SVGPathDriver& driver }
%param { void *scanner }

%no-lines
%locations

%initial-action {

};

%code requires {
#include "SVGPathDriver.h"
}

%code {
#include "RLDecls.h"
}

%define parse.trace
%define parse.error verbose

%define api.token.prefix {RL_TOK_}

%token
  END 0                          "<end of contents>"
  INVALID                        "<invalid token>"
  MOVE_ABS                       "MoveTo (Absolute)"
  MOVE_REL                       "MoveTo (Relative)"
  CLOSE                          "Close"
  LINE_ABS                       "Line (Absolute)"
  LINE_REL                       "Line (Relative)"
  LINE_HORIZONTAL_ABS            "Line Horizontal (Absolute)"
  LINE_HORIZONTAL_REL            "Line Horizontal (Relative)"
  LINE_VERTICAL_ABS              "Line Vertical (Absolute)"
  LINE_VERTICAL_REL              "Line Vertical (Relative)"
  CURVE_ABS                      "Curve (Absolute)"
  CURVE_REL                      "Curve (Relative)"
  SHORTHAND_CURVE_ABS            "Shorthand Curve (Absolute)"
  SHORTHAND_CURVE_REL            "Shorthand Curve (Relative)"
  QUAD_CURVE_ABS                 "Quad Curve (Absolute)"
  QUAD_CURVE_REL                 "Quad Curve (Relative)"
  SHORTHAND_QUAD_CURVE_ABS       "Shorthand Quad Curve (Absolute)"
  SHORTHAND_QUAD_CURVE_REL       "Shorthand Quad Curve (Relative)"
  ELLIPTIC_ARC_ABS               "Elliptic Arc (Absolute)"
  ELLIPTIC_ARC_REL               "Elliptic Arc (Relative)"
  ;

%token <rl::SVGNumber> NUMBER      "<number>"

%start PathElements

%type <rl::SVGElement> PathElement
%type <rl::SVGMoveElement> Move
%type <rl::SVGCloseElement> Close
%type <rl::SVGLineElement> Line
%type <rl::SVGLineHorizontalElement> LineHorizontal
%type <rl::SVGLineVerticalElement> LineVertical
%type <rl::SVGCurveElement> Curve
%type <rl::SVGShorthandCurveElement> ShorthandCurve
%type <rl::SVGQuadCurveElement> QuadCurve
%type <rl::SVGShorthandQuadCurveElement> ShorthandQuadCurve
%type <rl::SVGEllipticArcElement> EllipticArc
%type <std::vector<rl::geom::Point>> XYCoordinates
%type <rl::geom::Point> XYCoordinate
%type <std::vector<rl::SVGNumber>> Numbers
%type <std::vector<rl::SVGArcParam>> ArcParams
%type <rl::SVGArcParam> ArcParam

%%

PathElements
  : PathElement
  | PathElements PathElement
  ;

PathElement
  : Move                { driver.visitElement($1); }
  | Close               { driver.visitElement($1); }
  | Line                { driver.visitElement($1); }
  | LineHorizontal      { driver.visitElement($1); }
  | LineVertical        { driver.visitElement($1); }
  | Curve               { driver.visitElement($1); }
  | ShorthandCurve      { driver.visitElement($1); }
  | QuadCurve           { driver.visitElement($1); }
  | ShorthandQuadCurve  { driver.visitElement($1); }
  | EllipticArc         { driver.visitElement($1); }
  ;

Move
  : MOVE_ABS XYCoordinates { $$ = rl::SVGMoveElement{true, $2}; }
  | MOVE_REL XYCoordinates { $$ = rl::SVGMoveElement{false, $2}; }
  ;

Close
  : CLOSE { $$ = rl::SVGCloseElement{}; }
  ;

Line
  : LINE_ABS XYCoordinates    { $$ = rl::SVGLineElement{true, $2}; }
  | LINE_REL XYCoordinates    { $$ = rl::SVGLineElement{false, $2}; }
  ;

LineHorizontal
  : LINE_HORIZONTAL_ABS Numbers   { $$ = rl::SVGLineHorizontalElement{true, $2}; }
  | LINE_HORIZONTAL_REL Numbers   { $$ = rl::SVGLineHorizontalElement{false, $2}; }
  ;

LineVertical
  : LINE_VERTICAL_ABS Numbers  { $$ = rl::SVGLineVerticalElement{true, $2}; }
  | LINE_VERTICAL_REL Numbers  { $$ = rl::SVGLineVerticalElement{false, $2}; }
  ;

Curve
  : CURVE_ABS XYCoordinates  { $$ = rl::SVGCurveElement{true, $2}; }
  | CURVE_REL XYCoordinates  { $$ = rl::SVGCurveElement{false, $2}; }
  ;

ShorthandCurve
  : SHORTHAND_CURVE_ABS XYCoordinates { $$ = rl::SVGShorthandCurveElement{true, $2}; }
  | SHORTHAND_CURVE_REL XYCoordinates { $$ = rl::SVGShorthandCurveElement{false, $2}; }
  ;

QuadCurve
  : QUAD_CURVE_ABS XYCoordinates { $$ = rl::SVGQuadCurveElement{true, $2}; }
  | QUAD_CURVE_REL XYCoordinates { $$ = rl::SVGQuadCurveElement{false, $2}; }
  ;

ShorthandQuadCurve
  : SHORTHAND_QUAD_CURVE_ABS XYCoordinates { $$ = rl::SVGShorthandQuadCurveElement{true, $2}; }
  | SHORTHAND_QUAD_CURVE_REL XYCoordinates { $$ = rl::SVGShorthandQuadCurveElement{false, $2}; }
  ;

EllipticArc
  : ELLIPTIC_ARC_ABS ArcParams { $$ = rl::SVGEllipticArcElement{true, $2}; }
  | ELLIPTIC_ARC_REL ArcParams { $$ = rl::SVGEllipticArcElement{false, $2}; }
  ;

XYCoordinates
  : XYCoordinate                { $$.emplace_back($1); }
  | XYCoordinates XYCoordinate  { $1.emplace_back($2); $$ = std::move($1); }
  ;

XYCoordinate
  : NUMBER NUMBER     { $$ = rl::geom::Point($1, $2); }
  ;

Numbers
  : NUMBER              { $$.emplace_back($1); }
  | Numbers NUMBER      { $1.emplace_back($2); $$ = std::move($1); }
  ;

ArcParams
  : ArcParam              { $$.emplace_back($1); }
  | ArcParams ArcParam    { $1.emplace_back($2); $$ = std::move($1); }
  ;

ArcParam
  : NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER { $$ = rl::SVGArcParam { $1, $2, $3, $4, $5, $6, $7 }; }
  ;


%%

void rl::SVGPathParser::error (const rl::SVGPathParser::location_type& loc,
                        const std::string& message) {
  driver.error(loc, message);
}
