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
  const Result& addConstraints(const std::list<Constraint>& constraints);

  const Result& addConstraint(const Constraint& constraint);

  const Result& removeConstraints(const std::list<Constraint>& constraints);

  const Result& removeConstraint(const Constraint& constraint);

  bool hasConstraint(const Constraint& constraint) const;

  const Result& addEditVariables(const std::list<Variable> variables,
                                 double priority);

  const Result& addEditVariable(const Variable& variable, double priority);

  const Result& removeEditVariables(const std::list<Variable> variables);

  const Result& removeEditVariable(const Variable& variable);

  bool hasEditVariable(const Variable& variable) const;

  const Result& suggestValueForVariable(const Variable& variable, double value);

  void flushUpdates();

 private:
  std::map<Constraint, Tag> _constraints;
  std::map<Symbol, std::unique_ptr<Row>> _rows;
  std::map<Variable, Symbol> _vars;
  std::map<Variable, EditInfo> _edits;
  std::list<Symbol> _infeasibleRows;
  std::unique_ptr<Row> _objective;
  std::unique_ptr<Row> _artificial;

  template <class T>
  using UpdateCallback = std::function<void(const T&)>;

  template <class T>
  const Result& bulkEdit(const std::list<T>& items,
                         UpdateCallback<T> applier,
                         UpdateCallback<T> undoer);

  Symbol symbolForVariable(const Variable& variable);

  std::unique_ptr<Row> createRow(const Constraint& constraint, Tag& tag);

  Symbol chooseSubjectForRow(const Row& row, const Tag& tag) const;

  bool allDummiesInRow(const Row& row) const;

  bool addWithArtificialVariableOnRow(const Row& row);

  const Result& optimizeObjectiveRow(const Row& row);

  const Symbol& enteringSymbolForObjectiveRow(const Row& objective);

  Symbol leavingRowForEnteringSymbol(const Symbol& entering) const;

  void substitute(const Symbol& symbol, const Row& row);

  Symbol anyPivotableSymbol(const Row& row) const;

  void removeConstraintEffects(const Constraint& constraint, const Tag& tag);

  void removeMarkerEffects(const Symbol& marker, double strength);

  Symbol leavingRowPairForMarkerSymbol(const Symbol& marker) const;

  void suggestValueForEditInfoWithoutDualOptimization(EditInfo& info,
                                                      double value);

  const Result& dualOptimize();

  Symbol dualEnteringSymbolForRow(const Row& row);

  RL_DISALLOW_COPY_AND_ASSIGN(Solver);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SOLVER_H_
