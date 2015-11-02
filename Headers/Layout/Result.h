// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_RESULT_H_
#define RADARLOVE_LAYOUT_RESULT_H_

#include <Core/Core.h>

#include <string>

namespace rl {
namespace layout {

class Result {
 public:
  Result(const std::string& message, bool isError);

  const std::string& message() const;

  bool isError() const;

 private:
  const std::string _message;
  const bool _isError;

  RL_DISALLOW_COPY_AND_ASSIGN(Result);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_RESULT_H_
