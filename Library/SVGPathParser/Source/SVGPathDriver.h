// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_SVGPATHPARSER_SVGPATHDRIVER_H_
#define RL_SVGPATHPARSER_SVGPATHDRIVER_H_

#include <Core/Core.h>

#include <string>

#include "location.hh"

namespace rl {

class SVGPathDriver {
 public:
  enum class ParserResult {
    Success,
    SyntaxError,
    ParserError,
    OutOfMemory,
  };

  SVGPathDriver();

  ~SVGPathDriver();

  ParserResult parse(const std::string& text);

  rl::location location;

 private:
  friend class SVGPathParser;

  void error(rl::location loc, const std::string& message);

  RL_DISALLOW_COPY_AND_ASSIGN(SVGPathDriver);
};

}  // namespace rl

#endif  // RL_SVGPATHPARSER_SVGPATHDRIVER_H_
