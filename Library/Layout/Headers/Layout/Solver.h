// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_SOLVER_H_
#define RADARLOVE_LAYOUT_SOLVER_H_

#include <Core/Core.h>
#include <Layout/Constraint.h>
#include <Layout/EditInfo.h>
#include <Layout/Result.h>
#include <Layout/Suggestion.h>
#include <Layout/Symbol.h>
#include <Layout/Tag.h>
#include <Layout/Variable.h>

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

namespace rl {
namespace layout {

class Row;

class Solver {
 public:
  Solver(core::Namespace& localNS);

  ~Solver();

  Result addConstraints(const std::vector<Constraint>& constraints);

  Result addConstraint(const Constraint& constraint);

  Result removeConstraints(const std::vector<Constraint>& constraints);

  Result removeConstraint(const Constraint& constraint);

  bool hasConstraint(const Constraint& constraint) const;

  Result addEditVariables(const std::vector<Variable> variables,
                          double priority);

  Result addEditVariable(const Variable& variable, double priority);

  Result removeEditVariables(const std::vector<Variable> variables);

  Result removeEditVariable(const Variable& variable);

  bool hasEditVariable(const Variable& variable) const;

  Result applySuggestion(const Suggestion& suggestion);

  size_t constraintsCount() const;

  enum class FlushResult {
    NoUpdates,
    Updated,
  };

  using SolverUpdateCallback =
      std::function<void(const Variable&, double value)>;
  FlushResult flushUpdates(SolverUpdateCallback callback) const;

 private:
  core::Namespace& _localNS;
  std::map<Constraint, Tag, Constraint::Compare> _constraints;
  std::map<Symbol, std::unique_ptr<Row>, Symbol::Compare> _rows;
  std::unordered_map<Variable, Symbol, Variable::Hash, Variable::Equal> _vars;
  std::unordered_map<Variable, EditInfo, Variable::Hash, Variable::Equal>
      _edits;
  std::list<Symbol> _infeasibleRows;
  std::unique_ptr<Row> _objective;
  std::unique_ptr<Row> _artificial;

  template <class T>
  using UpdateCallback = std::function<Result(const T&)>;

  template <class T>
  Result bulkEdit(const std::vector<T>& items,
                  UpdateCallback<T> applier,
                  UpdateCallback<T> undoer);

  Symbol symbolForVariable(const Variable& variable);

  std::unique_ptr<Row> createRow(const Constraint& constraint, Tag& tag);

  Symbol chooseSubjectForRow(const Row& row, const Tag& tag) const;

  bool allDummiesInRow(const Row& row) const;

  bool addWithArtificialVariableOnRow(const Row& row);

  Result optimizeObjectiveRow(const Row& row);

  Symbol enteringSymbolForObjectiveRow(const Row& objective);

  Symbol leavingSymbolForEntering(const Symbol& entering) const;

  void substitute(const Symbol& symbol, const Row& row);

  Symbol pivotableSymbol(const Row& row) const;

  void removeConstraintEffects(const Constraint& constraint, const Tag& tag);

  void removeMarkerEffects(const Symbol& marker, double strength);

  Symbol leavingSymbolForMarker(const Symbol& marker) const;

  Result suggestValueForVariable(const Variable& variable, double value);

  void suggestValueForEditInfoWithoutDualOptimization(EditInfo& info,
                                                      double value);

  Result dualOptimize();

  Symbol dualEnteringSymbolForRow(const Row& row);

  RL_DISALLOW_COPY_AND_ASSIGN(Solver);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_SOLVER_H_
