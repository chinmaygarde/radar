// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_RIDL_DRIVER_H_
#define RL_RIDL_DRIVER_H_

#include <Core/Core.h>

#include <string>

#include "location.hh"

namespace rl {

class Driver {
 public:
  enum class ParserResult {
    Success,
    SyntaxError,
    ParserError,
    OutOfMemory,
  };

  Driver();

  ~Driver();

  ParserResult parse(const std::string& text);

  location location;

 private:
  friend class Parser;

  void error(rl::location loc, const std::string& message);

  RL_DISALLOW_COPY_AND_ASSIGN(Driver);
};

}  // namespace rl

#endif  // RL_RIDL_DRIVER_H_
