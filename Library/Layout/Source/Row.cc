// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Row.h"
#include "LayoutUtilities.h"

namespace rl {
namespace layout {

Row::Row(double constant) : _constant(constant), _constantHasUpdate(true) {}

double Row::constant() const {
  return _constant;
}

const Row::Cells& Row::cells() const {
  return _cells;
}

double Row::add(double value) {
  setConstantUpdated(!NearZero(value));
  _constant += value;
  return _constant;
}

void Row::insertSymbol(const Symbol& symbol, double coefficient) {
  double val = _cells[symbol];

  if (NearZero(val + coefficient)) {
    _cells.erase(symbol);
  } else {
    _cells[symbol] = val + coefficient;
  }
}

void Row::insertRow(const Row& other, double coefficient) {
  auto delta = other.constant() * coefficient;
  _constant += delta;
  setConstantUpdated(!NearZero(delta));
  for (auto const& cell : other.cells()) {
    insertSymbol(cell.first, cell.second * coefficient);
  }
}

void Row::removeSymbol(const Symbol& symbol) {
  _cells.erase(symbol);
}

void Row::reverseSign() {
  _constant = -_constant;
  setConstantUpdated(!NearZero(_constant));
  for (auto& cell : _cells) {
    cell.second = -cell.second;
  }
}

void Row::solve(const Symbol& symbol) {
  RL_ASSERT(_cells.find(symbol) != _cells.end());

  double coefficient = -1.0 / _cells[symbol];

  _cells.erase(symbol);

  setConstantUpdated(coefficient != 1.0);

  _constant *= coefficient;
  for (auto& cell : _cells) {
    cell.second = cell.second * coefficient;
  }
}

void Row::solve(const Symbol& lhs, const Symbol& rhs) {
  insertSymbol(lhs, -1.0);
  solve(rhs);
}

double Row::coefficientForSymbol(const Symbol& symbol) const {
  auto found = _cells.find(symbol);
  if (found == _cells.end()) {
    return 0.0;
  } else {
    return found->second /* coefficient */;
  }
}

void Row::substitute(const Symbol& symbol, const Row& row) {
  auto cell = _cells.find(symbol);

  if (cell == _cells.end()) {
    return;
  }

  auto coefficient = cell->second;
  _cells.erase(cell);
  insertRow(row, coefficient);
}

bool Row::constantHasUpdate() const {
  return _constantHasUpdate;
}

void Row::resolveConstantUpdate() {
  _constantHasUpdate = false;
}

void Row::setConstantUpdated(bool updated) {
  _constantHasUpdate = _constantHasUpdate || updated;
}

}  // namespace layout
}  // namespace rl
