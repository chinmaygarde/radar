/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Layout/Constraint.h>
#include "Tag.h"

namespace rl {
namespace layout {

class EditInfo {
 public:
  EditInfo(const Tag& tag, const Constraint& constraint, double constant);

  const Tag& tag() const;

  void setTag(const Tag& tag);

  const Constraint& constraint() const;

  double constant() const;

  void setConstant(double constant);

 private:
  Tag _tag;
  Constraint _constraint;
  double _constant;

  RL_DISALLOW_COPY_AND_ASSIGN(EditInfo);
};

}  // namespace layout
}  // namespace rl
