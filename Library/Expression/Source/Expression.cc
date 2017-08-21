/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Message.h>
#include <Expression/Expression.h>

namespace rl {
namespace expr {

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
  RL_RETURN_IF_FALSE(message.encode(_terms));
  RL_RETURN_IF_FALSE(message.encode(_constant));

  return true;
}

bool Expression::deserialize(core::Message& message, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(message.decode(_terms, ns));
  RL_RETURN_IF_FALSE(message.decode(_constant, ns));

  return true;
}

enum ArchiveKey {
  Terms,
  Constant,
};

const core::ArchiveDef Expression::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "Expression",
    /* .autoAssignName = */ true,
    /* .members = */
    {
        ArchiveKey::Terms,    //
        ArchiveKey::Constant  //
    }};

Expression::ArchiveName Expression::archiveName() const {
  return core::ArchiveNameAuto;
}

bool Expression::serialize(core::ArchiveItem& item) const {
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Terms, _terms));
  RL_RETURN_IF_FALSE(item.encode(ArchiveKey::Constant, _constant));

  return true;
}

bool Expression::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Terms, _terms, ns));
  RL_RETURN_IF_FALSE(item.decode(ArchiveKey::Constant, _constant));

  return true;
}

}  // namespace expr
}  // namespace rl
