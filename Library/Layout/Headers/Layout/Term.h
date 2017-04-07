// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
