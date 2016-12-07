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

%token <std::string> NUMBER      "<number>"

%start PathElements

%%

PathElements
  : PathElement
  | PathElements PathElement
  ;

PathElement
  : Move
  | Close
  | Line
  | LineHorizontal
  | LineVertical
  | Curve
  | ShorthandCurve
  | QuadCurve
  | ShorthandQuadCurve
  | EllipticArc
  ;

Move
  : MOVE_ABS XYCoordinates
  | MOVE_REL XYCoordinates
  ;

Close
  : CLOSE
  ;

Line
  : LINE_ABS XYCoordinates
  | LINE_REL XYCoordinates
  ;

LineHorizontal
  : LINE_HORIZONTAL_ABS Numbers
  | LINE_HORIZONTAL_REL Numbers
  ;

LineVertical
  : LINE_VERTICAL_ABS Numbers
  | LINE_VERTICAL_REL Numbers
  ;

Curve
  : CURVE_ABS XYCoordinates
  | CURVE_REL XYCoordinates
  ;

ShorthandCurve
  : SHORTHAND_CURVE_ABS XYCoordinates
  | SHORTHAND_CURVE_REL XYCoordinates
  ;

QuadCurve
  : QUAD_CURVE_ABS XYCoordinates
  | QUAD_CURVE_REL XYCoordinates
  ;

ShorthandQuadCurve
  : SHORTHAND_QUAD_CURVE_ABS XYCoordinates
  | SHORTHAND_QUAD_CURVE_REL XYCoordinates
  ;

EllipticArc
  : ELLIPTIC_ARC_ABS ArcParams
  | ELLIPTIC_ARC_REL ArcParams
  ;

XYCoordinates
  : XYCoordinate
  | XYCoordinates XYCoordinate
  ;

XYCoordinate
  : NUMBER NUMBER
  ;

Numbers
  : NUMBER
  | Numbers NUMBER
  ;

ArcParams
  : ArcParam
  | ArcParams ArcParam
  ;

ArcParam
  : NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER
  ;


%%

void rl::SVGPathParser::error (const rl::SVGPathParser::location_type& loc,
                        const std::string& message) {
  driver.error(loc, message);
}
