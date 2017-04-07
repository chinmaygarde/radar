// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

namespace rl {
namespace layout {

enum class Result {
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

}  // namespace layout
}  // namespace rl
