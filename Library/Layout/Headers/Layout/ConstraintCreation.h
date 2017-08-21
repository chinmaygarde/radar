/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Entity/Entity.h>
#include <Expression/Expression.h>
#include <Expression/Term.h>
#include <Expression/Variable.h>
#include <Layout/Constraint.h>

// clang-format off

/**
 *  Determines if this type is already an expression member
 */
template <class T>
struct ExpressionMember : public std::integral_constant<
    bool,
    std::is_base_of<rl::expr::Variable, T>::value   ||
    std::is_base_of<rl::expr::Term, T>::value       ||
    std::is_base_of<rl::expr::Expression, T>::value> {
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

template <class A, class B, class = rl::core::only_if<(Hoistable<A, B>::value)>>
rl::expr::Expression operator+(const A& a, const B& b) {
  rl::expr::Expression exprA(a);
  rl::expr::Expression exprB(b);
  auto terms = exprA.terms();
  for (const auto& term : exprB.terms()) {
    terms.push_back(term);
  }
  return rl::expr::Expression{std::move(terms),
                              exprA.constant() + exprB.constant()};
}

template <class A, class B, class = rl::core::only_if<(Hoistable<A, B>::value)>>
rl::expr::Expression operator-(const A& a, const B& b) {
  rl::expr::Expression exprA(a);
  rl::expr::Expression exprB(b);
  auto terms = exprA.terms();
  for (const auto& term : exprB.terms()) {
    terms.push_back(rl::expr::Term{term.variable(), -term.coefficient(),
                                   term.isConstant()});
  }
  return rl::expr::Expression{std::move(terms),
                              exprA.constant() - exprB.constant()};
}

template <class A, class = rl::core::only_if<(ExpressionMember<A>::value)>>
rl::expr::Expression operator*(const A& a, double m) {
  rl::expr::Expression expr(a);
  rl::expr::Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(rl::expr::Term{term.variable(), term.coefficient() * m,
                                   term.isConstant()});
  }
  return rl::expr::Expression{std::move(terms), expr.constant() * m};
}

template <class A, class = rl::core::only_if<(ExpressionMember<A>::value)>>
rl::expr::Expression operator*(double m, const A& a) {
  return a * m;
}

template <class A, class = rl::core::only_if<(ExpressionMember<A>::value)>>
rl::expr::Expression operator/(const A& a, double m) {
  rl::expr::Expression expr(a);
  rl::expr::Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(rl::expr::Term{term.variable(), term.coefficient() / m,
                                   term.isConstant()});
  }
  return rl::expr::Expression{std::move(terms), expr.constant() / m};
}

template <class A, class B, class = rl::core::only_if<(Hoistable<A, B>::value)>>
rl::layout::Constraint operator==(const A& a, const B& b) {
  auto expr = rl::expr::Expression{a - b};
  return {rl::core::Name{expr.ns()}, std::move(expr),
          rl::layout::Constraint::Relation::EqualTo,
          rl::layout::priority::Required()};
}

template <class A, class B, class = rl::core::only_if<(Hoistable<A, B>::value)>>
rl::layout::Constraint operator>=(const A& a, const B& b) {
  auto expr = rl::expr::Expression{a - b};
  return {rl::core::Name{expr.ns()}, std::move(expr),
          rl::layout::Constraint::Relation::GreaterThanOrEqualTo,
          rl::layout::priority::Required()};
}

template <class A, class B, class = rl::core::only_if<(Hoistable<A, B>::value)>>
rl::layout::Constraint operator<=(const A& a, const B& b) {
  auto expr = rl::expr::Expression{a - b};
  return {rl::core::Name{expr.ns()}, std::move(expr),
          rl::layout::Constraint::Relation::LessThanOrEqualTo,
          rl::layout::priority::Required()};
}

template <class A, class = rl::core::only_if<(HoistableMember<A>::value)>>
rl::expr::Expression MakeConst(const A& a) {
  rl::expr::Expression expr(a);
  rl::expr::Expression::Terms terms;
  for (const auto& term : expr.terms()) {
    terms.push_back(rl::expr::Term{term.variable(), term.coefficient(), true});
  }
  return rl::expr::Expression{std::move(terms), expr.constant()};
}

inline rl::layout::Constraint operator|(
    const rl::layout::Constraint& constraint,
    double priority) {
  return {rl::core::Name{constraint.ns()}, constraint.expression(),
          constraint.relation(), priority};
}

inline rl::layout::Constraint operator|(
    double priority,
    const rl::layout::Constraint& constraint) {
  return constraint | priority;
}

inline rl::expr::Variable operator|(const rl::entity::Entity& entity,
                                    rl::expr::Variable::Property property) {
  return {entity.identifier(), property};
}
