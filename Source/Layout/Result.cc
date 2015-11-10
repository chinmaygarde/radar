// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Result.h>

namespace rl {
namespace layout {

Result::Result(Type type, bool isError) : _type(type), _isError(isError) {
}

bool Result::isError() const {
  return _isError;
}

// clang-format off
extern const Result ResultSuccess(
    Result::Type::Success, false);
extern const Result ResultUnimplemented(
    Result::Type::Unimplemented);
extern const Result ResultDuplicateConstraint(
    Result::Type::DuplicateConstraint);
extern const Result ResultUnsatisfiableConstraint(
    Result::Type::UnsatisfiableConstraint);
extern const Result ResultUnknownConstraint(
    Result::Type::UnknownConstraint);
extern const Result ResultDuplicateEditVariable(
    Result::Type::DuplicateConstraint);
extern const Result ResultBadRequiredStrength(
    Result::Type::BadRequiredStrength);
extern const Result ResultUnknownEditVariable(
    Result::Type::UnknownEditVariable);
extern const Result ResultInternalSolverError(
    Result::Type::InternalSolverError);
// clang-format on
}  // namespace layout
}  // namespace rl
