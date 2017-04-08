/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Layout/Variable.h>

namespace rl {
namespace layout {

class Term : public core::ArchiveSerializable,
             public core::MessageSerializable {
 public:
  Term();

  Term(const Variable& variable, double coefficient, bool constant);

  const Variable& variable() const;

  double coefficient() const;

  bool isConstant() const;

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

 private:
  Variable _variable;
  double _coefficient;
  bool _constant;
};

}  // namespace layout
}  // namespace rl
