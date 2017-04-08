/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "SVGPathScanner.h"

namespace rl {

SVGPathScanner::SVGPathScanner(const std::string& text)
    : _scanner(nullptr), _buffer(nullptr), _isReady(false) {
  if (RLSVGPath_lex_init(&_scanner) != 0) {
    return;
  }
  _buffer = RLSVGPath__scan_string(text.data(), _scanner);
  _isReady = true;
}

bool SVGPathScanner::isReady() const {
  return _isReady;
}

yyscan_t SVGPathScanner::handle() {
  return _scanner;
}

SVGPathScanner::~SVGPathScanner() {
  if (!_isReady) {
    return;
  }

  RLSVGPath__delete_buffer(_buffer, _scanner);
  RLSVGPath_lex_destroy(_scanner);
}

}  // namespace rl
