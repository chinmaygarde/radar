// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_RIDL_RLDECLS_H_
#define RL_RIDL_RLDECLS_H_

#include "Parser.h"
#include "Driver.h"

#define YY_DECL \
  rl::Parser::symbol_type RL_lex(rl::Driver& driver, void* yyscanner)

YY_DECL;

#endif  // RL_RIDL_RLDECLS_H_
