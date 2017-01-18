// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_SVGPATHPARSER_SVGPATHSCANNER_H_
#define RL_SVGPATHPARSER_SVGPATHSCANNER_H_

#include <Core/Core.h>
#include "RLDecls.h"
#include "SVGPathLexer.h"

namespace rl {

class SVGPathScanner {
 public:
  SVGPathScanner(const std::string& text);

  ~SVGPathScanner();

  bool isReady() const;

  yyscan_t handle();

 private:
  yyscan_t _scanner;
  YY_BUFFER_STATE _buffer;
  bool _isReady;

  RL_DISALLOW_COPY_AND_ASSIGN(SVGPathScanner);
};

}  // namespace rl

#endif  // RL_SVGPATHPARSER_SVGPATHSCANNER_H_
