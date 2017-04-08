/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RL_SVGPATHPARSER_RLDECLS_H_
#define RL_SVGPATHPARSER_RLDECLS_H_

#include "SVGPathDriver.h"
#include "SVGPathParser.h"

#define YY_DECL                                                           \
  rl::SVGPathParser::symbol_type RLSVGPath_lex(rl::SVGPathDriver& driver, \
                                               void* yyscanner)

YY_DECL;

#endif  // RL_SVGPATHPARSER_RLDECLS_H_
