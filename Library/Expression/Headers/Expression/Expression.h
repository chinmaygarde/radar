/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Expression/Term.h>
#include <vector>

namespace rl {
namespace expr {

class Expression : public core::ArchiveSerializable,
                   public core::MessageSerializable {
 public:
  using Terms = std::vector<Term>;

  Expression();

  Expression(const Terms& terms, double constant);

  Expression(const Term& term);

  Expression(const Variable& variable);

  Expression(double constant);

  core::Namespace* ns() const;

  const Terms& terms() const;

  double constant() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

 private:
  Terms _terms;
  double _constant;
  core::Namespace* _inferredNamespace;

  void inferNamespace();

  RL_DISALLOW_ASSIGN(Expression);
};

}  // namespace expr
}  // namespace rl
