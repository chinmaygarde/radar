// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_IDENTIFIER_H_
#define RADARLOVE_INTERFACE_IDENTIFIER_H_

#include <Core/Macros.h>
#include <atomic>

namespace rl {
namespace interface {

class Identifier {
 public:
  using MemberID = uint32_t;
  using LocalID = uint16_t;

  class Factory {
   public:
    Factory(LocalID local);

    Identifier acquire();

   private:
    std::atomic<MemberID> _lastMember;
    LocalID _local;

    RL_DISALLOW_COPY_AND_ASSIGN(Factory);
  };

  Identifier(MemberID member, LocalID primary, LocalID counterpart);

  MemberID member() const;

  LocalID primary() const;

  LocalID counterpart() const;

  void switchPrimary(LocalID newPrimary);

  size_t hash() const;

  bool operator==(const Identifier& other) const;

  bool operator!=(const Identifier& other) const;

  struct Compare {
    constexpr bool operator()(const Identifier& a, const Identifier& b) const {
      return a._member < b._member;
    }
  };

 private:
  MemberID _member;
  LocalID _primary;
  LocalID _counterpart;
};

static_assert(rl_trivially_copyable(Identifier),
              "Identifiers are trivially copyable");

extern Identifier IdentifierNone;

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_IDENTIFIER_H_
