// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_EDITINFO_H_
#define RADARLOVE_LAYOUT_EDITINFO_H_

#include <Core/Core.h>
#include <Layout/Constraint.h>
#include <Layout/Tag.h>

namespace rl {
namespace layout {

class EditInfo {
 public:
#warning move these around
  EditInfo(const Tag& tag, Constraint&& constraint, double constant);

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

#endif  // RADARLOVE_LAYOUT_EDITINFO_H_
