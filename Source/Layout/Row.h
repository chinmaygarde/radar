// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_ROW_H_
#define RADARLOVE_LAYOUT_ROW_H_

#include <Core/Core.h>
#include <Layout/Symbol.h>

#include <map>

namespace rl {
namespace layout {

class Row {
 public:
  using Cells = std::map<Symbol, double>;

  Row(double constant);

  double constant() const;

  const Cells& cells() const;

  double add(double value);

  void insertSymbol(const Symbol& symbol, double coefficient = 1.0);

  void insertRow(const Row& row, double coefficient);

  void removeSymbol(const Symbol& symbol);

  void reverseSign();

  void solveForSymbol(const Symbol& symbol);

  void solveForSymbols(const Symbol& lhs, const Symbol& rhs);

  double coefficientForSymbol(const Symbol& symbol) const;

  void substitute(const Symbol& symbol, const Row& row);

 private:
  Cells _cells;
  double _constant;

  RL_DISALLOW_COPY_AND_ASSIGN(Row);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_ROW_H_
