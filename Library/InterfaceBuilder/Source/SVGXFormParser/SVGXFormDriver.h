// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_SVGXFORMPARSER_SVGXFORMDRIVER_H_
#define RADAR_SVGXFORMPARSER_SVGXFORMDRIVER_H_

#include <Core/Macros.h>
#include <string>
#include "SVGXFormElement.h"
#include "location.hh"

namespace rl {

class SVGXFormDriver {
 public:
  enum class ParserResult {
    Success,
    SyntaxError,
    ParserError,
    OutOfMemory,
  };

  SVGXFormDriver();

  ~SVGXFormDriver();

  ParserResult parse(const std::string& text);

  rl::location location;

 private:
  friend class SVGXFormParser;

  void error(rl::location loc, const std::string& message);

  template <class T>
  void visitElement(const T& element);

  RL_DISALLOW_COPY_AND_ASSIGN(SVGXFormDriver);
};

}  // namespace rl

#endif  // RADAR_SVGXFORMPARSER_SVGXFORMDRIVER_H_
