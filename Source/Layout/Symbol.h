// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_SYMBOL_H_
#define RADARLOVE_LAYOUT_SYMBOL_H_

#include <Core/Core.h>

namespace rl {
namespace layout {

class Symbol {
 public:
  using Identifier = uint64_t;

  enum class Type { Invalid, External, Slack, Error, Dummy };

  Symbol();

  explicit Symbol(Type type);

  Type type() const;

  bool operator==(const Symbol& other) const {
    return _identifier == other._identifier;
  }

  bool operator!=(const Symbol& other) const {
    return _identifier != other._identifier;
  }

  struct Compare {
    constexpr bool operator()(const Symbol& lhs, const Symbol& rhs) const {
      return lhs._identifier < rhs._identifier;
    }
  };

 private:
  Type _type;
  Identifier _identifier;

  Symbol(Type type, Identifier identifier);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SYMBOL_H_
