// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_SOLVER_H_
#define RADARLOVE_LAYOUT_SOLVER_H_

#include <Core/Core.h>
#include <Layout/Constraint.h>
#include <Layout/Tag.h>
#include <Layout/Symbol.h>
#include <Layout/Row.h>
#include <Layout/Variable.h>
#include <Layout/EditInfo.h>
#include <Layout/Result.h>

#include <map>
#include <list>
#include <functional>

namespace rl {
namespace layout {

class Solver {
 public:
  Result addConstraints(const std::list<Constraint>& constraints);

  Result addConstraint(const Constraint& constraint);

  Result removeConstraints(const std::list<Constraint>& constraints);

  Result removeConstraint(const Constraint& constraint);

  bool hasConstraint() const;

  Result addEditVariables(const std::list<Variable> variables, double priority);

  Result removeEditVariables(const std::list<Variable> variables);

  bool hasEditVariable(const Variable& variable) const;

  Result suggestValueForVariable(const Variable& variable, double value);

  void flushUpdates();

 private:
  std::map<Constraint, Tag> _constraints;
  std::map<Symbol, Row> _rows;
  std::map<Variable, Symbol> _vars;
  std::map<Variable, EditInfo> _edits;
  std::list<Symbol> _infeasibleRows;
  Row _objective;
  Row _artificial;

  template <class T>
  using UpdateCallback = std::function<void(const T&)>;

  template <class T>
  const Result& bulkEdit(const std::list<T>& items,
                         UpdateCallback<T> applier,
                         UpdateCallback<T> undoer);

  const Symbol& symbolForVariable(const Variable& variable);

  const Row& createRow(const Constraint& constraint, const Tag& tag);

  const Symbol& chooseSubjectForRow(const Row& row, const Tag& tag) const;

  bool allDummiesInRow(const Row& row) const;

  bool addWithArtificialVariableInRow(const Row& row);

  const Result& optimizeObjectiveRow(const Row& row);

  const Symbol& enteringSymbolForObjectiveRow(const Row& objective);

  using SymbolRowPair = std::pair<const Symbol&, const Row&>;

  SymbolRowPair leavingRowForEnteringSymbol(const Symbol& entering) const;

  void substitute(const Symbol& symbol, const Row& row);

  const Symbol& anyPivtableSymbol(const Row& row) const;

  void removeConstraintEffects(const Constraint& constraint, const Tag& tag);

  void removeMarkerEffects(const Symbol& marker, double strength);

  SymbolRowPair leavingRowPairForMarkerSymbol(const Symbol& marker) const;

  void suggestValueForEditInfoWithoutDualOptimization(const EditInfo& info,
                                                      double value);

  const Result& dualOptimize();

  const Symbol& dualEnteringSymbolForRow(const Row& row);

  RL_DISALLOW_COPY_AND_ASSIGN(Solver);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SOLVER_H_
