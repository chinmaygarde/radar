// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_SUGGESTION_H_
#define RADARLOVE_LAYOUT_SUGGESTION_H_

#include <Core/Core.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

class Suggestion {
 public:
  Suggestion(const Variable& variable, double value);

  const Variable& variable() const;

  double value() const;

 private:
  Variable _variable;
  double _value;

  RL_DISALLOW_ASSIGN(Suggestion);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SUGGESTION_H_
