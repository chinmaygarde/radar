// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Expression.h>

namespace rl {
namespace layout {

Expression::Expression() : _constant(0.0), _inferredNamespace(nullptr) {
  inferNamespace();
}

Expression::Expression(const Terms& terms, double constant)
    : _terms(terms), _constant(constant), _inferredNamespace(nullptr) {
  inferNamespace();
}

Expression::Expression(const Term& term)
    : _terms({term}), _constant(0.0), _inferredNamespace(nullptr) {
  inferNamespace();
}

Expression::Expression(const Variable& variable)
    : _terms({Term{variable, 1.0, false}}),
      _constant(0.0),
      _inferredNamespace(nullptr) {
  inferNamespace();
}

Expression::Expression(double constant)
    : _constant(constant), _inferredNamespace(nullptr) {
  inferNamespace();
}

const Expression::Terms& Expression::terms() const {
  return _terms;
}

double Expression::constant() const {
  return _constant;
}

core::Namespace* Expression::ns() const {
  return _inferredNamespace;
}

void Expression::inferNamespace() {
  core::Namespace* inferred = nullptr;
  for (const auto& term : _terms) {
    auto variableNS = term.variable().identifier().ns();
    if (variableNS != inferred) {
      inferred = variableNS;
      break;
    }
  }
  _inferredNamespace = inferred;
}

bool Expression::serialize(core::Message& message) const {
  auto success = true;
  success &= message.encode(_terms);
  success &= message.encode(_constant);
  return success;
}

bool Expression::deserialize(core::Message& message, core::Namespace* ns) {
  auto success = true;
  success &= message.decode(_terms, ns);
  success &= message.decode(_constant, ns);
  return success;
}

enum ArchiveKey {
  Terms,
  Constant,
};

const core::ArchiveDef Expression::ArchiveDefinition = {
    .superClass = nullptr,
    .className = "Expression",
    .autoAssignName = true,
    .members = {
        ArchiveKey::Terms,    //
        ArchiveKey::Constant  //
    }};

Expression::ArchiveName Expression::archiveName() const {
  return core::ArchiveNameAuto;
}

bool Expression::serialize(core::ArchiveItem& item) const {
  auto result = true;
  result &= item.encode(ArchiveKey::Terms, _terms);
  result &= item.encode(ArchiveKey::Constant, _constant);
  return result;
}

bool Expression::deserialize(core::ArchiveItem& item) {
  auto result = true;
  result &= item.decode(ArchiveKey::Terms, _terms);
  result &= item.decode(ArchiveKey::Constant, _constant);
  return result;
}

}  // namespace layout
}  // namespace rl
