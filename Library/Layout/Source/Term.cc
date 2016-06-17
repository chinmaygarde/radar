// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Term.h>

namespace rl {
namespace layout {

Term::Term() : _coefficient(1.0) {}

Term::Term(const Variable& variable, double coefficient, bool constant)
    : _variable(variable), _coefficient(coefficient), _constant(constant) {}

const Variable& Term::variable() const {
  return _variable;
}

double Term::coefficient() const {
  return _coefficient;
}

bool Term::isConstant() const {
  return _constant;
}

bool Term::serialize(core::Message& message) const {
  RL_RETURN_IF_FALSE(message.encode(_variable));
  RL_RETURN_IF_FALSE(message.encode(_coefficient));
  RL_RETURN_IF_FALSE(message.encode(_constant));

  return true;
}

bool Term::deserialize(core::Message& message, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(message.decode(_variable, ns));
  RL_RETURN_IF_FALSE(message.decode(_coefficient, ns));
  RL_RETURN_IF_FALSE(message.decode(_constant, ns));

  return true;
}

enum ArchiveKey {
  Variable,
  Coefficient,
  Constant,
};

const core::ArchiveDef Term::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "Term",
    /* .autoAssignName = */ true,
    /* .members = */
    {
        ArchiveKey::Variable,     //
        ArchiveKey::Coefficient,  //
        ArchiveKey::Constant      //
    },
};

Term::ArchiveName Term::archiveName() const {
  return core::ArchiveNameAuto;
}

bool Term::serialize(core::ArchiveItem& item) const {
  RL_RETURN_IF_FALSE(item.encodeArchivable(ArchiveKey::Variable, _variable));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Coefficient, _coefficient));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Constant, _constant));

  return true;
}

bool Term::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(
      item.decodeArchivable(ArchiveKey::Variable, _variable, ns));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Coefficient, _coefficient));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Constant, _constant));

  return true;
}

}  // namespace layout
}  // namespace rl
