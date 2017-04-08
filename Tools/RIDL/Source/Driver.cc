/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Driver.h"
#include "Scanner.h"

namespace rl {

Driver::Driver() {}

Driver::~Driver() {}

Driver::ParserResult Driver::parse(const std::string& text) {
  Scanner scanner(text);

  if (!scanner.isReady()) {
    return ParserResult::ParserError;
  }

  Parser parser(*this, scanner.handle());

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

void Driver::error(rl::location loc, const std::string& message) {
  printf("Error: %s\n", message.c_str());
}

}  // namespace rl
