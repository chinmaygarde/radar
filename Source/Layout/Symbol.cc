// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Symbol.h>

namespace rl {
namespace layout {

static Symbol::Identifier LastSymbolIdentifier = 0;

Symbol::Symbol() : _type(Type::Invalid), _identifier(0) {}

Symbol::Symbol(Type type)
    : _type(type),
      _identifier(type == Type::Invalid ? 0 : ++LastSymbolIdentifier) {}

Symbol::Symbol(Type type, Identifier identifier)
    : _type(type), _identifier(identifier) {}

Symbol::Type Symbol::type() const {
  return _type;
}

}  // namespace layout
}  // namespace rl
