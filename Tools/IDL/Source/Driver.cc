// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Driver.h"
#include "Scanner.h"

namespace rl {

Driver::Driver() {}

Driver::~Driver() {}

Driver::Result Driver::parse(const std::string& text) {
  Scanner scanner(text);

  if (!scanner.isReady()) {
    return Result(ParserResult::ParserError, {});
  }

  Parser parser(*this, scanner.handle());

  switch (parser.parse()) {
    case 0: /* parsing was successful (return is due to end-of-input) */
      return Result(ParserResult::Success, {});
    case 1: /* contains a syntax error */
      return Result(ParserResult::SyntaxError, {});
    case 2: /* memory exhaustion */
      return Result(ParserResult::OutOfMemory, {});
  }

  return Result(ParserResult::ParserError, {});
}

void Driver::error(rl::location loc, const std::string& message) {
  printf("Error: %s\n", message.c_str());
}

}  // namespace rl
