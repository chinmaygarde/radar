// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <Interface/Identifier.h>

namespace rl {
namespace interface {

Identifier::Identifier(MemberID member, LocalID primary, LocalID counterpart)
    : _member(member), _primary(primary), _counterpart(counterpart) {}

Identifier::Factory::Factory(Identifier::LocalID local) : _local(local) {}

Identifier Identifier::Factory::acquire() {
  return {++_lastMember, _local, 0};
}

Identifier::MemberID Identifier::member() const {
  return _member;
}

Identifier::LocalID Identifier::primary() const {
  return _primary;
}

Identifier::LocalID Identifier::counterpart() const {
  return _counterpart;
}

void Identifier::switchPrimary(LocalID newPrimary) {
  RL_ASSERT_MSG(_primary != 0,
                "Must have a counterpart who holds the primary reference");
  RL_ASSERT(_counterpart == 0);
  _counterpart = _primary;
  _primary = newPrimary;
}

bool Identifier::operator==(const Identifier& o) const {
  return _member == o._member && _primary == o._primary &&
         _counterpart == o._counterpart;
}

bool Identifier::operator!=(const Identifier& o) const {
  return _member != o._member || _primary != o._primary ||
         _counterpart != o._counterpart;
}

size_t Identifier::hash() const {
  size_t seed = 0;
  core::HashCombine(seed, _member);
  core::HashCombine(seed, _primary);
  core::HashCombine(seed, _counterpart);
  return seed;
}

Identifier IdentifierNone(0, 0, 0);

}  // namespace interface
}  // namespace rl
