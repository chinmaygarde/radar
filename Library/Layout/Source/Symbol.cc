/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
