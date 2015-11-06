// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Result.h>

namespace rl {
namespace layout {

Result::Result(Type type, const std::string& message, bool isError)
    : _type(type), _message(message), _isError(isError) {
}

const std::string& Result::message() const {
  return _message;
}

bool Result::isError() const {
  return _isError;
}

// clang-format off
extern const Result ResultSuccess(
    Result::Type::Success, "Success", false);
extern const Result ResultUnimplemented(
    Result::Type::Unimplemented, "Unimplemented");
extern const Result ResultDuplicateConstraint(
    Result::Type::DuplicateConstraint, "Duplicate Constraint");
extern const Result ResultUnsatisfiableConstraint(
    Result::Type::UnsatisfiableConstraint, "Unsatisfiable Constraint");
extern const Result ResultUnknownConstraint(
    Result::Type::UnknownConstraint, "Unknown Constraint");
extern const Result ResultDuplicateEditVariable(
    Result::Type::DuplicateConstraint, "Duplicate Edit Variable");
extern const Result ResultBadRequiredStrength(
    Result::Type::BadRequiredStrength, "Bad Required Strength");
extern const Result ResultUnknownEditVariable(
    Result::Type::UnknownEditVariable, "Unknown Edit Variable");
extern const Result ResultInternalSolverError(
    Result::Type::InternalSolverError, "Internal Solver Error");
// clang-format on
}  // namespace layout
}  // namespace rl
