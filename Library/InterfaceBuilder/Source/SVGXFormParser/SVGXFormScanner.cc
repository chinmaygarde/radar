/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "SVGXFormScanner.h"

namespace rl {

SVGXFormScanner::SVGXFormScanner(const std::string& text)
    : _scanner(nullptr), _buffer(nullptr), _isReady(false) {
  if (RLSVGXForm_lex_init(&_scanner) != 0) {
    return;
  }
  _buffer = RLSVGXForm__scan_string(text.data(), _scanner);
  _isReady = true;
}

bool SVGXFormScanner::isReady() const {
  return _isReady;
}

yyscan_t SVGXFormScanner::handle() {
  return _scanner;
}

SVGXFormScanner::~SVGXFormScanner() {
  if (!_isReady) {
    return;
  }

  RLSVGXForm__delete_buffer(_buffer, _scanner);
  RLSVGXForm_lex_destroy(_scanner);
}

}  // namespace rl
