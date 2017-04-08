/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Scanner.h"

namespace rl {

Scanner::Scanner(const std::string& text)
    : _scanner(nullptr), _buffer(nullptr), _isReady(false) {
  if (RL_lex_init(&_scanner) != 0) {
    return;
  }
  _buffer = RL__scan_string(text.data(), _scanner);
  _isReady = true;
}

bool Scanner::isReady() const {
  return _isReady;
}

yyscan_t Scanner::handle() {
  return _scanner;
}

Scanner::~Scanner() {
  if (!_isReady) {
    return;
  }

  RL__delete_buffer(_buffer, _scanner);
  RL_lex_destroy(_scanner);
}

}  // namespace rl
