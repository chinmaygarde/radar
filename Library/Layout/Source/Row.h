/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Layout/Symbol.h>
#include <map>

namespace rl {
namespace layout {

class Row {
 public:
  using Cells = std::map<Symbol, double, Symbol::Compare>;

  Row(double constant);

  Row(const Row& row) = default;

  double constant() const;

  const Cells& cells() const;

  double add(double value);

  void insertSymbol(const Symbol& symbol, double coefficient = 1.0);

  void insertRow(const Row& row, double coefficient);

  void removeSymbol(const Symbol& symbol);

  void reverseSign();

  void solve(const Symbol& symbol);

  void solve(const Symbol& lhs, const Symbol& rhs);

  double coefficientForSymbol(const Symbol& symbol) const;

  void substitute(const Symbol& symbol, const Row& row);

  bool constantHasUpdate() const;

  void resolveConstantUpdate();

 private:
  Cells _cells;
  double _constant;
  bool _constantHasUpdate;

  void setConstantUpdated(bool updated);

  RL_DISALLOW_ASSIGN(Row);
};

}  // namespace layout
}  // namespace rl
