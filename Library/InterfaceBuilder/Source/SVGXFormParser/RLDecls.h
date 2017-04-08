/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#ifndef RL_SVGXFORMPARSER_RLDECLS_H_
#define RL_SVGXFORMPARSER_RLDECLS_H_

#include "SVGXFormDriver.h"
#include "SVGXFormParser.h"

#define YY_DECL                                                              \
  rl::SVGXFormParser::symbol_type RLSVGXForm_lex(rl::SVGXFormDriver& driver, \
                                                 void* yyscanner)

YY_DECL;

#endif  // RL_SVGXFORMPARSER_RLDECLS_H_
