// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_TERM_H_
#define RADARLOVE_LAYOUT_TERM_H_

#include <Core/Core.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

class Term : public core::Serializable {
 public:
  Term();

  Term(const Variable& variable, double coefficient = 1.0);

  const Variable& variable() const;

  double coefficient() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message) override;

 private:
  Variable _variable;
  double _coefficient;

  RL_DISALLOW_ASSIGN(Term);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_TERM_H_
