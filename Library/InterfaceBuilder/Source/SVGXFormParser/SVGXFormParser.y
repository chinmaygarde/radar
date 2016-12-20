%skeleton "lalr1.cc"
%require "3.0.4"

%defines "SVGXFormParser.h"
%output "SVGXFormParser.cc"
%name-prefix "RLSVGXForm_"
%define api.namespace {rl}
%define parser_class_name {SVGXFormParser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%param { rl::SVGXFormDriver& driver }
%param { void *scanner }

%no-lines
%locations

%initial-action {

};

%code requires {
#include "SVGXFormDriver.h"
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
  MATRIX                         "Matrix"
  TRANSLATE                      "Translate"
  SCALE                          "Scale"
  ROTATE                         "Rotate"
  SKEW_X                         "Skew X"
  SKEW_Y                         "Skew Y"
  LEFT_BRACE                     "("
  RIGHT_BRACE                    ")"
  ;

%token <rl::SVGNumber> NUMBER      "<number>"

%start XFormElements

%type <rl::SVGXFormMatrix> Matrix
%type <rl::SVGXFormTranslate> Translate
%type <rl::SVGXFormScale> Scale
%type <rl::SVGXFormRotate> Rotate
%type <rl::SVGXFormSkewX> SkewX
%type <rl::SVGXFormSkewY> SkewY

%%

XFormElements
  : XFormElement
  | XFormElements XFormElement
  ;

XFormElement
  : Matrix      { driver.visitElement($1); }
  | Translate   { driver.visitElement($1); }
  | Scale       { driver.visitElement($1); }
  | Rotate      { driver.visitElement($1); }
  | SkewX       { driver.visitElement($1); }
  | SkewY       { driver.visitElement($1); }
  ;

Matrix
  : MATRIX LEFT_BRACE NUMBER NUMBER NUMBER NUMBER NUMBER NUMBER RIGHT_BRACE { $$ = {$3, $4, $5, $6, $7, $8, }; }
  ;

Translate
  : TRANSLATE LEFT_BRACE NUMBER RIGHT_BRACE           { $$ = { $3, 0.0 }; }
  | TRANSLATE LEFT_BRACE NUMBER NUMBER RIGHT_BRACE    { $$ = { $3, $4 }; }
  ;

Scale
  : SCALE LEFT_BRACE NUMBER RIGHT_BRACE          { $$ = { $3, $3 }; }
  | SCALE LEFT_BRACE NUMBER NUMBER RIGHT_BRACE   { $$ = { $3, $4 }; }
  ;

Rotate
  : ROTATE LEFT_BRACE NUMBER RIGHT_BRACE                { $$ = { $3 }; }
  | ROTATE LEFT_BRACE NUMBER NUMBER NUMBER RIGHT_BRACE  { $$ = { $3, {$4, $5} }; }
  ;

SkewX
  : SKEW_X LEFT_BRACE NUMBER RIGHT_BRACE { $$ = { $3 }; }
  ;

SkewY
  : SKEW_Y LEFT_BRACE NUMBER RIGHT_BRACE { $$ = { $3 }; }
  ;

%%

void rl::SVGXFormParser::error (const rl::SVGXFormParser::location_type& loc,
                                const std::string& message) {
  driver.error(loc, message);
}
