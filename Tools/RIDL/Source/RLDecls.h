/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RL_RIDL_RLDECLS_H_
#define RL_RIDL_RLDECLS_H_

#include "Driver.h"
#include "Parser.h"

#define YY_DECL \
  rl::Parser::symbol_type RL_lex(rl::Driver& driver, void* yyscanner)

YY_DECL;

#endif  // RL_RIDL_RLDECLS_H_
