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
  enum class Type {
    Success,
    Unimplemented,
    DuplicateConstraint,
    UnsatisfiableConstraint,
    UnknownConstraint,
    DuplicateEditVariable,
    BadRequiredStrength,
    UnknownEditVariable,
    InternalSolverError,
  };

  Result(Type type);

  bool isError() const;

  bool operator==(const Result& other) const { return _type == other._type; }

  bool operator!=(const Result& other) const { return _type != other._type; }

 private:
  Type _type;
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_RESULT_H_
