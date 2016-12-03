// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_SVGPATHPARSER_RLDECLS_H_
#define RL_SVGPATHPARSER_RLDECLS_H_

#include "SVGPathParser.h"
#include "SVGPathDriver.h"
#include "SVGPathLexer.h"

#define YY_DO_BEFORE_ACTION
#define YY_NEW_FILE

#define YY_DECL                                                    \
  rl::SVGPathParser::symbol_type RL_lex(rl::SVGPathDriver& driver, \
                                        void* yyscanner)

YY_DECL;

#endif  // RL_SVGPATHPARSER_RLDECLS_H_
