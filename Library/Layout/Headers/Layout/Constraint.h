/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Expression/Expression.h>
#include <Layout/Priority.h>

namespace rl {
namespace layout {

class Constraint final : public core::ArchiveSerializable,
                         public core::MessageSerializable {
 public:
  enum Relation {
    EqualTo,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo,
  };

  Constraint();

  Constraint(core::Name name,
             const expr::Expression& expression,
             Relation relation,
             double priority);

  Constraint(const Constraint& constraint);

  Relation relation() const;

  const expr::Expression& expression() const;

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
      std::function<expr::Variable(const expr::Variable&)>;
  using ConstantResolutionCallback =
      std::function<double(const expr::Variable&)>;
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
  expr::Expression _expression;
  double _priority;

  RL_DISALLOW_ASSIGN(Constraint);
};

}  // namespace layout
}  // namespace rl
