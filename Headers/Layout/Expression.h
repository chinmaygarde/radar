// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_EXPRESSION_H_
#define RADARLOVE_LAYOUT_EXPRESSION_H_

#include <Core/Core.h>
#include <Layout/Term.h>

#include <list>

namespace rl {
namespace layout {

class Expression {
 public:
  Expression(const std::list<Term>& terms, double constant);

  const std::list<Term>& terms() const;

  double constant() const;

  Expression operator+(double m) const;
  Expression operator+(const Variable& v) const;
  Expression operator+(const Term& t) const;
  Expression operator+(const Expression& e) const;

  Expression operator-(double m) const;
  Expression operator-(const Variable& v) const;
  Expression operator-(const Term& t) const;
  Expression operator-(const Expression& e) const;

  Expression operator*(double m) const;
  Expression operator/(double m) const;

 private:
  const std::list<Term> _terms;
  const double _constant;

  RL_DISALLOW_ASSIGN(Expression);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_EXPRESSION_H_
