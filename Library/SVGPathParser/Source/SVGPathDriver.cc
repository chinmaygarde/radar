// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "SVGPathDriver.h"
#include "SVGPathScanner.h"

namespace rl {

SVGPathDriver::SVGPathDriver() {}

SVGPathDriver::~SVGPathDriver() {}

SVGPathDriver::ParserResult SVGPathDriver::parse(const std::string& text) {
  SVGPathScanner scanner(text);

  if (!scanner.isReady()) {
    return ParserResult::ParserError;
  }

  SVGPathParser parser(*this, scanner.handle());

  switch (parser.parse()) {
    case 0: /* parsing was successful (return is due to end-of-input) */
      return ParserResult::Success;
    case 1: /* contains a syntax error */
      return ParserResult::SyntaxError;
    case 2: /* memory exhaustion */
      return ParserResult::OutOfMemory;
  }

  return ParserResult::ParserError;
}

void SVGPathDriver::error(rl::location loc, const std::string& message) {
  printf("Error: %s\n", message.c_str());
}

}  // namespace rl
