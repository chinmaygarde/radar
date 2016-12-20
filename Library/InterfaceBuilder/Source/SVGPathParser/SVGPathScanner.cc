// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
