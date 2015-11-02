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
  enum class Type { Invalid, External, Slack, Error, Dummy };

  Symbol(Type type);

 private:
  const Type _type;

  RL_DISALLOW_ASSIGN(Symbol);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SYMBOL_H_
