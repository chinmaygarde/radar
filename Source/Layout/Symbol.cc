// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Layout/Symbol.h>

namespace rl {
namespace layout {

static Symbol::Identifier LastSymbolIdentifier = 0;

Symbol::Symbol(Type type) : _type(type), _identifier(++LastSymbolIdentifier) {
}

Symbol::Type Symbol::type() const {
  return _type;
}

const Symbol SymbolInvalid(Symbol::Type::Invalid);

}  // namespace layout
}  // namespace rl
