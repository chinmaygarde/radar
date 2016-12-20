// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
