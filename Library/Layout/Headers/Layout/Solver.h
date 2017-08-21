/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Expression/Variable.h>
#include <Layout/Constraint.h>
#include <Layout/EditInfo.h>
#include <Layout/Result.h>
#include <Layout/Suggestion.h>
#include <Layout/Symbol.h>
#include <Layout/Tag.h>
#include <functional>
#include <list>
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

  Result addEditVariables(const std::vector<expr::Variable> variables,
                          double priority);

  Result addEditVariable(const expr::Variable& variable, double priority);

  Result removeEditVariables(const std::vector<expr::Variable> variables);

  Result removeEditVariable(const expr::Variable& variable);

  bool hasEditVariable(const expr::Variable& variable) const;

  Result applySuggestion(const Suggestion& suggestion);

  Result suggestValueForVariable(const expr::Variable& variable, double value);

  size_t constraintsCount() const;

  size_t editVariableCount() const;

  enum class FlushResult {
    NoUpdates,
    Updated,
  };

  using SolverUpdateCallback =
      std::function<FlushResult(const expr::Variable&, double value)>;
  size_t flushUpdates(SolverUpdateCallback callback) const;

 private:
  core::Namespace& _localNS;
  std::map<Constraint, Tag, Constraint::Compare> _constraints;
  std::map<Symbol, std::unique_ptr<Row>, Symbol::Compare> _rows;
  std::unordered_map<expr::Variable,
                     Symbol,
                     expr::Variable::Hash,
                     expr::Variable::Equal>
      _vars;
  std::unordered_map<expr::Variable,
                     EditInfo,
                     expr::Variable::Hash,
                     expr::Variable::Equal>
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

  Symbol symbolForVariable(const expr::Variable& variable);

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

  void suggestValueForEditInfoWithoutDualOptimization(EditInfo& info,
                                                      double value);

  Result dualOptimize();

  Symbol dualEnteringSymbolForRow(const Row& row);

  RL_DISALLOW_COPY_AND_ASSIGN(Solver);
};

}  // namespace layout
}  // namespace rl
