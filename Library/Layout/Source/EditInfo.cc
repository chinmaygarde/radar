// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/EditInfo.h>

namespace rl {
namespace layout {

EditInfo::EditInfo(const Tag& tag,
                   const Constraint& constraint,
                   double constant)
    : _tag(tag), _constraint(constraint), _constant(constant) {}

const Tag& EditInfo::tag() const {
  return _tag;
}

void EditInfo::setTag(const Tag& tag) {
  _tag = tag;
}

const Constraint& EditInfo::constraint() const {
  return _constraint;
}

double EditInfo::constant() const {
  return _constant;
}

void EditInfo::setConstant(double constant) {
  _constant = constant;
}

}  // namespace layout
}  // namespace rl
