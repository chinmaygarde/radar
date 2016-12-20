// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_SVGXFORMPARSER_RLDECLS_H_
#define RL_SVGXFORMPARSER_RLDECLS_H_

#include "SVGXFormParser.h"
#include "SVGXFormDriver.h"

#define YY_DECL                                                              \
  rl::SVGXFormParser::symbol_type RLSVGXForm_lex(rl::SVGXFormDriver& driver, \
                                                 void* yyscanner)

YY_DECL;

#endif  // RL_SVGXFORMPARSER_RLDECLS_H_
