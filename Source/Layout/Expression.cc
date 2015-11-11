// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Expression.h>
#include <Layout/Utilities.h>

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

Expression Expression::operator+(double m) const {
  return {_terms, _constant + m};
}

Expression Expression::operator+(const Variable& v) const {
  auto terms = _terms;
  terms.push_back({v, 1.0});
  return {std::move(terms), _constant};
}

Expression Expression::operator+(const Term& t) const {
  auto terms = _terms;
  terms.push_back(t);
  return {std::move(terms), _constant};
}

Expression Expression::operator+(const Expression& e) const {
  auto terms = _terms;
  for (const auto& term : e._terms) {
    terms.push_back(term);
  }
  return {std::move(terms), _constant + e._constant};
}

Expression Expression::operator-(double m) const {
  return {_terms, _constant - m};
}

Expression Expression::operator-(const Variable& v) const {
  auto terms = _terms;
  terms.push_back({v, -1.0});
  return {std::move(terms), _constant};
}

Expression Expression::operator-(const Term& t) const {
  auto terms = _terms;
  terms.push_back({t.variable(), -t.coefficient()});
  return {std::move(terms), _constant};
}

Expression Expression::operator-(const Expression& e) const {
  auto terms = _terms;
  for (const auto& term : _terms) {
    terms.push_back({term.variable(), -term.coefficient()});
  }
  return {std::move(terms), _constant - e._constant};
}

Expression Expression::operator*(double m) const {
  if (NearZero(m)) {
    return {{}, 0.0};
  }

  std::list<Term> terms;
  for (const auto& term : _terms) {
    terms.push_back({term.variable(), term.coefficient() * m});
  }
  return {std::move(terms), _constant * m};
}

Expression Expression::operator/(double m) const {
  return *this * (1.0 / m);
}

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

}  // namespace layout
}  // namespace rl
