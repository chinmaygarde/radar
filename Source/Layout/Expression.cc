// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Expression.h>

namespace rl {
namespace layout {

Expression::Expression(const std::list<Term>& terms, double constant)
    : _terms(terms), _constant(constant) {
}

const std::list<Term>& Expression::terms() const {
  return _terms;
}

double Expression::constant() const {
  return _constant;
}

/*
 *  Expression
 */

Expression operator+(const Expression& expr, double m) {
  return {expr.terms(), expr.constant() + m};
}

Expression operator+(const Expression& expr, const Variable& v) {
  auto terms = expr.terms();
  terms.push_back({v, 1.0});
  return {std::move(terms), expr.constant()};
}

Expression operator+(const Expression& expr, const Term& t) {
  auto terms = expr.terms();
  terms.push_back(t);
  return {std::move(terms), expr.constant()};
}

Expression operator+(const Expression& expr, const Expression& e) {
  auto terms = expr.terms();
  for (const auto& term : e.terms()) {
    terms.push_back(term);
  }
  return {std::move(terms), expr.constant() + e.constant()};
}

Expression operator-(const Expression& expr, double m) {
  return {expr.terms(), expr.constant() - m};
}

Expression operator-(const Expression& expr, const Variable& v) {
  auto terms = expr.terms();
  terms.push_back({v, -1.0});
  return {std::move(terms), expr.constant()};
}

Expression operator-(const Expression& expr, const Term& t) {
  auto terms = expr.terms();
  terms.push_back({t.variable(), -t.coefficient()});
  return {std::move(terms), expr.constant()};
}

Expression operator-(const Expression& expr, const Expression& e) {
  auto terms = expr.terms();
  for (const auto& term : e.terms()) {
    terms.push_back({term.variable(), -term.coefficient()});
  }
  return {std::move(terms), expr.constant() - e.constant()};
}

Expression operator*(const Expression& expr, double m) {
  std::list<Term> terms;
  for (const auto& term : expr.terms()) {
    terms.push_back({term.variable(), term.coefficient() * m});
  }
  return {std::move(terms), expr.constant() * m};
}

Expression operator/(const Expression& expr, double m) {
  return expr * (1.0 / m);
}

/*
 *  Terms
 */

Expression operator+(const Term& term, double m) {
  return {{term}, m};
}

Expression operator+(const Term& term, const Variable& v) {
  return {{term, Term{v, 1.0}}, 0.0};
}

Expression operator+(const Term& term, const Term& t) {
  return {{term, t}, 0.0};
}

Expression operator+(const Term& term, const Expression& e) {
  auto terms = e.terms();
  terms.push_back(term);
  return {std::move(terms), e.constant()};
}

Expression operator-(const Term& term, double m) {
  return {{term}, -m};
}

Expression operator-(const Term& term, const Variable& v) {
  return {{term, Term{v, -1.0}}, 0.0};
}

Expression operator-(const Term& term, const Term& t) {
  return {{term, Term{t.variable(), -t.coefficient()}}, 0.0};
}

Expression operator-(const Term& term, const Expression& e) {
  std::list<Term> terms = {term};
  for (const auto& term : e.terms()) {
    terms.push_back({term.variable(), -term.coefficient()});
  }
  return {std::move(terms), -e.constant()};
}

/*
 *  Variables
 */

Expression operator+(const Variable& variable, double m) {
  return {{Term{variable}}, m};
}

Expression operator+(const Variable& variable, const Variable& v) {
  return {{Term{variable}, Term{v}}, 0.0};
}

Expression operator+(const Variable& variable, const Term& t) {
  return {{Term{variable}, t}, 0.0};
}

Expression operator+(const Variable& variable, const Expression& e) {
  auto terms = e.terms();
  terms.push_back(Term{variable});
  return {std::move(terms), e.constant()};
}

Expression operator-(const Variable& variable, double m) {
  return {{Term{variable}}, -m};
}

Expression operator-(const Variable& variable, const Variable& v) {
  return {{Term{variable}, Term{v, -1.0}}, 0.0};
}

Expression operator-(const Variable& variable, const Term& t) {
  return {{Term{variable}, Term{t.variable(), -t.coefficient()}}, 0.0};
}

Expression operator-(const Variable& variable, const Expression& e) {
  std::list<Term> terms = {Term{variable}};
  for (const auto& term : e.terms()) {
    terms.push_back(Term{term.variable(), -term.coefficient()});
  }
  return {std::move(terms), -e.constant()};
}

Term operator*(const Variable& variable, double m) {
  return Term{variable, m};
}

Term operator/(const Variable& variable, double m) {
  return Term{variable, 1.0 / m};
}

}  // namespace layout
}  // namespace rl
