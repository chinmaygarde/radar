// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_EXPRESSION_H_
#define RADARLOVE_LAYOUT_EXPRESSION_H_

#include <Core/Core.h>
#include <Layout/Term.h>
#include <Layout/ExpressionMember.h>

#include <vector>

namespace rl {
namespace layout {

class Expression : public ExpressionMember {
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

// clang-format off
template <class T>
struct HoistableMember : public std::integral_constant<
    bool,
    std::is_base_of<ExpressionMember, T>::value ||
    std::is_arithmetic<T>::value> {};

template <class A, class B>
struct Hoistable : public std::integral_constant<
    bool,
    HoistableMember<A>::value ||
    HoistableMember<B>::value> {};
// clang-format on

template <class A, class B, class = core::only_if<(Hoistable<A, B>::value)>>
Expression operator+(const A& a, const B& b) {
  Expression exprA(a);
  Expression exprB(b);
  auto terms = exprA.terms();
  for (const auto& term : exprB.terms()) {
    terms.push_back(term);
  }
  return Expression{std::move(terms), exprA.constant() + exprB.constant()};
}

template <class A, class B, class = core::only_if<(Hoistable<A, B>::value)>>
Expression operator-(const A& a, const B& b) {
  Expression exprA(a);
  Expression exprB(b);
  auto terms = exprA.terms();
  for (const auto& term : exprB.terms()) {
    terms.push_back(Term{term.variable(), -term.coefficient()});
  }
  return Expression{std::move(terms), exprA.constant() - exprB.constant()};
}

template <class A,
          class = core::only_if<(std::is_base_of<ExpressionMember, A>::value)>>
Expression operator*(const A& a, double m) {
  Expression expr(a);
  Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(Term{term.variable(), term.coefficient() * m});
  }
  return Expression{std::move(terms), expr.constant() * m};
}

template <class A,
          class = core::only_if<(std::is_base_of<ExpressionMember, A>::value)>>
Expression operator*(double m, const A& a) {
  return a * m;
}

template <class A,
          class = core::only_if<(std::is_base_of<ExpressionMember, A>::value)>>
Expression operator/(const A& a, double m) {
  Expression expr(a);
  Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(Term{term.variable(), term.coefficient() / m});
  }
  return Expression{std::move(terms), expr.constant() / m};
}

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_EXPRESSION_H_
