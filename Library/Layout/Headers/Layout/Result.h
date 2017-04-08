/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
