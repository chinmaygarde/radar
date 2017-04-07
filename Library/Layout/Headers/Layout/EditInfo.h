// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
