%skeleton "lalr1.cc"
%require "3.0.4"

%defines "Parser.h"
%output "Parser.cc"
%name-prefix "RL_"
%define api.namespace {rl}
%define parser_class_name {Parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%param { rl::Driver& driver }
%param { void *scanner }

%no-lines
%locations

%initial-action {

};

%code requires {
#include "Driver.h"
// #include "Module.h"
}

%code {
#include "RLDecls.h"
// #include "Module.h"
}

%define parse.trace
%define parse.error verbose

%define api.token.prefix {RL_TOK_}

%token
  END 0                   "<end of contents>"
  INVALID_TOKEN           "<invalid token>"
  PROTOCOL                "protocol"
  VOID                    "void"
  DOT                     "."
  COMMA                   ","
  SEMI_COLON              ";"
  CURLY_LEFT              "{"
  CURLY_RIGHT             "}"
  PAREN_LEFT              "("
  PAREN_RIGHT             ")"
  SQUARE_LEFT             "["
  SQUARE_RIGHT            "]"
  ;

%token <std::string> IDENTIFIER      "<identifier>"

%start SourceFile

%%

SourceFile
  : %empty
  ;

%%

void rl::Parser::error (const rl::Parser::location_type& loc,
                        const std::string& message) {
  driver.error(loc, message);
}
