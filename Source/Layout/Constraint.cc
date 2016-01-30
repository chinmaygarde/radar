// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Constraint.h>

namespace rl {
namespace layout {

Constraint::Constraint()
    : _relation(Relation::EqualTo), _priority(priority::Weak) {}

Constraint::Constraint(core::Name name,
                       const Expression& expression,
                       Relation relation,
                       double priority)
    : _identifier(name),
      _expression(expression),
      _relation(relation),
      _priority(priority) {}

Constraint::Constraint(const Constraint& c)
    : _identifier(c._identifier),
      _expression(c._expression),
      _relation(c._relation),
      _priority(c._priority) {}

Constraint::Relation Constraint::relation() const {
  return _relation;
}

const Expression& Constraint::expression() const {
  return _expression;
}

double Constraint::priority() const {
  return _priority;
}

bool Constraint::hasProxies() const {
  for (const auto& term : _expression.terms()) {
    if (term.variable().isProxy()) {
      return true;
    }
  }
  return false;
}

bool Constraint::hasConstantTerms() const {
  for (const auto& term : _expression.terms()) {
    if (term.isConstant()) {
      return true;
    }
  }
  return false;
}

Constraint Constraint::resolveProxies(
    core::Namespace& ns,
    ProxyVariableReplacementCallback replacement,
    ConstantResolutionCallback constantResolution) const {
  double accumulatedConstant = 0.0;

  Expression::Terms terms;
  for (auto term : _expression.terms()) {
    /*
     *  Perform proxy replacement over all variables
     */
    if (term.variable().isProxy()) {
      auto replaced = replacement(term.variable());
      RL_ASSERT_MSG(
          !replaced.isProxy(),
          "The results of proxy replacement must not itself be a proxy");
      term = {replaced, term.coefficient(), term.isConstant()};
    }

    /*
     *  If any of the terms after proxy resolution are constants, perform
     *  constant resolution now
     */
    if (term.isConstant()) {
      /*
       *  We are going to eliminate the term entirely and use a constant in its
       *  place in the final equation.
       */
      auto constantReplacement = constantResolution(term.variable());
      accumulatedConstant += term.coefficient() * constantReplacement;
    } else {
      terms.push_back(term);
    }
  }

  return Constraint{
      core::Name(ns),
      {std::move(terms), _expression.constant() + accumulatedConstant},
      _relation,
      _priority};
}

core::Namespace* Constraint::ns() const {
  return _expression.ns();
}

bool Constraint::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_identifier);
  success &= message.encode(_expression);
  success &= message.encode(_relation);
  success &= message.encode(_priority);
  return success;
}

bool Constraint::deserialize(core::Message& message, core::Namespace* ns) {
  auto success = true;
  success &= message.decode(_identifier, ns);
  success &= message.decode(_expression, ns);
  success &= message.decode(_relation, ns);
  success &= message.decode(_priority, ns);
  return success;
}

enum ArchiveKey {
  Expression,
  Relation,
  Priority,
};

const core::ArchiveDef Constraint::ArchiveDefinition = {
    .superClass = nullptr,
    .className = "Constraint",
    .autoAssignName = false,
    .members =
        {
            ArchiveKey::Expression,  //
            ArchiveKey::Relation,    //
            ArchiveKey::Priority     //
        },
};

Constraint::ArchiveName Constraint::archiveName() const {
  return *_identifier.handle();
}

bool Constraint::serialize(core::ArchiveItem& item) const {
  auto result = true;
  result &= item.encodeArchivable(ArchiveKey::Expression, _expression);
  result &= item.encodeEnum(ArchiveKey::Relation, _relation);
  result &= item.encode(ArchiveKey::Priority, _priority);
  return result;
}

bool Constraint::deserialize(core::ArchiveItem& item) {
  auto result = true;
  result &= item.decodeArchivable(ArchiveKey::Expression, _expression);
  result &= item.decodeEnum(ArchiveKey::Relation, _relation);
  result &= item.decode(ArchiveKey::Priority, _priority);
  return result;
}

}  // namespace layout
}  // namespace rl
