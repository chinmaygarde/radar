/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <cstdint>

namespace rl {
namespace layout {

class Symbol {
 public:
  using Identifier = uint64_t;

  enum class Type { Invalid, External, Slack, Error, Dummy };

  Symbol();

  Symbol(Type type);

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
