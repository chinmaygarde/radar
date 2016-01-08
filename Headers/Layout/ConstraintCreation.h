// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_CONSTRAINTCREATION_H_
#define RADARLOVE_LAYOUT_CONSTRAINTCREATION_H_

#include <Interface/Entity.h>
#include <Layout/Constraint.h>
#include <Layout/Expression.h>
#include <Layout/Term.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

// clang-format off

/**
 *  Determines if this type is already an expression member
 */
template <class T>
struct ExpressionMember : public std::integral_constant<
    bool,
    std::is_base_of<Variable, T>::value   ||
    std::is_base_of<Term, T>::value       ||
    std::is_base_of<Expression, T>::value> {
};

/**
 *  Determines if the member can be hoisted to a type that can take part in
 *  expression construction for constraints
 */
template <class T>
struct HoistableMember : public std::integral_constant<
    bool,
    /* The member is either a variable, term or an expression itself */
    ExpressionMember<T>::value ||
    /* The member is is a number */
    std::is_arithmetic<T>::value> {
};

/**
 *  Determines if the pair of types should take part in operations that return
 *  an expression
 */
template <class A, class B>
struct Hoistable : public std::integral_constant<
    bool,
    /* The pair members are hoistable */
    HoistableMember<A>::value &&
    HoistableMember<B>::value &&
    /* Both are not numbers */
    !(std::is_arithmetic<A>::value && std::is_arithmetic<B>::value)> {
};
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
    terms.push_back(
        Term{term.variable(), -term.coefficient(), term.isConstant()});
  }
  return Expression{std::move(terms), exprA.constant() - exprB.constant()};
}

template <class A, class = core::only_if<(ExpressionMember<A>::value)>>
Expression operator*(const A& a, double m) {
  Expression expr(a);
  Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(
        Term{term.variable(), term.coefficient() * m, term.isConstant()});
  }
  return Expression{std::move(terms), expr.constant() * m};
}

template <class A, class = core::only_if<(ExpressionMember<A>::value)>>
Expression operator*(double m, const A& a) {
  return a * m;
}

template <class A, class = core::only_if<(ExpressionMember<A>::value)>>
Expression operator/(const A& a, double m) {
  Expression expr(a);
  Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(
        Term{term.variable(), term.coefficient() / m, term.isConstant()});
  }
  return Expression{std::move(terms), expr.constant() / m};
}

template <class A, class B, class = core::only_if<(Hoistable<A, B>::value)>>
Constraint operator==(const A& a, const B& b) {
  auto expr = Expression{a - b};
  return {core::Name{expr.ns()}, std::move(expr), Constraint::Relation::EqualTo,
          priority::Required};
}

template <class A, class B, class = core::only_if<(Hoistable<A, B>::value)>>
Constraint operator>=(const A& a, const B& b) {
  auto expr = Expression{a - b};
  return {core::Name{expr.ns()}, std::move(expr),
          Constraint::Relation::GreaterThanOrEqualTo, priority::Required};
}

template <class A, class B, class = core::only_if<(Hoistable<A, B>::value)>>
Constraint operator<=(const A& a, const B& b) {
  auto expr = {a - b};
  return {core::Name{expr.ns()}, std::move(expr),
          Constraint::Relation::LessThanOrEqualTo, priority::Required};
}

template <class A, class = core::only_if<(HoistableMember<A>::value)>>
Expression MakeConst(const A& a) {
  Expression expr(a);
  Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(Term{term.variable(), term.coefficient(), true});
  }
  return Expression{std::move(terms), expr.constant()};
}

inline Constraint operator|(const Constraint& constraint, double priority) {
  auto ns = constraint.ns();
  auto name = ns == nullptr ? core::DeadName : ns->create();
  return {name, constraint.expression(), constraint.relation(), priority};
}

inline Constraint operator|(double priority, const Constraint& constraint) {
  return constraint | priority;
}

inline Variable operator|(const interface::Entity& entity,
                          Variable::Property property) {
  return {entity.identifier(), property};
}

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_CONSTRAINTCREATION_H_
