// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_EXPRESSION_H_
#define RADARLOVE_LAYOUT_EXPRESSION_H_

#include <Core/Core.h>
#include <Layout/Term.h>

#include <vector>

namespace rl {
namespace layout {

class Expression {
 public:
  using Terms = std::vector<Term>;

  Expression(const Terms& terms, double constant);

  Expression(const Term& term);

  Expression(const Variable& variable);

  Expression(double constant);

  const Terms& terms() const;

  double constant() const;

 private:
  const Terms _terms;
  const double _constant;

  RL_DISALLOW_ASSIGN(Expression);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_EXPRESSION_H_
