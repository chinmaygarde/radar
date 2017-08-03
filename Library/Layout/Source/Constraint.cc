/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Message.h>
#include <Layout/Constraint.h>

namespace rl {
namespace layout {

Constraint::Constraint()
    : _relation(Relation::EqualTo), _priority(priority::Weak()) {}

Constraint::Constraint(core::Name name,
                       const Expression& expression,
                       Relation relation,
                       double priority)
    : _identifier(name),
      _relation(relation),
      _expression(expression),
      _priority(priority) {}

Constraint::Constraint(const Constraint& c)
    : _identifier(c._identifier),
      _relation(c._relation),
      _expression(c._expression),
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
  RL_RETURN_IF_FALSE(message.encode(_identifier));
  RL_RETURN_IF_FALSE(message.encode(_expression));
  RL_RETURN_IF_FALSE(message.encode(_relation));
  RL_RETURN_IF_FALSE(message.encode(_priority));

  return true;
}

bool Constraint::deserialize(core::Message& message, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(message.decode(_identifier, ns));
  RL_RETURN_IF_FALSE(message.decode(_expression, ns));
  RL_RETURN_IF_FALSE(message.decode(_relation, ns));
  RL_RETURN_IF_FALSE(message.decode(_priority, ns));

  return true;
}

enum ArchiveKey {
  Identifier,
  Expression,
  Relation,
  Priority,
};

const core::ArchiveDef Constraint::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "Constraint",
    /* .autoAssignName = */ false,
    /* .members = */
    {
        ArchiveKey::Identifier,  //
        ArchiveKey::Expression,  //
        ArchiveKey::Relation,    //
        ArchiveKey::Priority     //
    },
};

Constraint::ArchiveName Constraint::archiveName() const {
  return *_identifier.handle();
}

bool Constraint::serialize(core::ArchiveItem& item) const {
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Identifier, _identifier));
  RL_RETURN_IF_FALSE(
      item.encodeArchivable(ArchiveKey::Expression, _expression));
  RL_RETURN_IF_FALSE(item.encodeEnum(ArchiveKey::Relation, _relation));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Priority, _priority));

  return true;
}

bool Constraint::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Identifier, _identifier, ns));
  RL_RETURN_IF_FALSE(
      item.decodeArchivable(ArchiveKey::Expression, _expression, ns));
  RL_RETURN_IF_FALSE(item.decodeEnum(ArchiveKey::Relation, _relation));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Priority, _priority));

  return true;
}

}  // namespace layout
}  // namespace rl
