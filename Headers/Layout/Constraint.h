// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_CONSTRAINT_H_
#define RADARLOVE_LAYOUT_CONSTRAINT_H_

#include <Core/Core.h>
#include <Layout/Expression.h>
#include <Layout/Priority.h>

namespace rl {
namespace layout {

class Constraint : public core::ArchiveSerializable,
                   public core::MessageSerializable {
 public:
  enum Relation {
    EqualTo,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo,
  };

  Constraint();

  Constraint(core::Name name,
             const Expression& expression,
             Relation relation,
             double priority);

  Constraint(const Constraint& constraint);

  Relation relation() const;

  const Expression& expression() const;

  double priority() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

  bool hasProxies() const;

  bool hasConstantTerms() const;

  core::Namespace* ns() const;

  using ProxyVariableReplacementCallback =
      std::function<Variable(const Variable&)>;
  using ConstantResolutionCallback = std::function<double(const Variable&)>;
  Constraint resolveProxies(
      core::Namespace& ns,
      ProxyVariableReplacementCallback replacement,
      ConstantResolutionCallback constantResolution) const;

  struct Compare {
    bool operator()(const Constraint& lhs, const Constraint& rhs) const {
      return lhs._identifier < rhs._identifier;
    }
  };

 private:
  core::Name _identifier;
  Relation _relation;
  Expression _expression;
  double _priority;

  RL_DISALLOW_ASSIGN(Constraint);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_CONSTRAINT_H_
