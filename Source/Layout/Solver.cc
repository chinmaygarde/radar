// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#warning iterator "first" are stored in separate variables unnecessarily
#include <Layout/Solver.h>
#include <Layout/Utilities.h>
#include <Layout/Priority.h>

namespace rl {
namespace layout {

const Result& Solver::addConstraint(const Constraint& constraint) {
  if (_constraints.find(constraint) != _constraints.end()) {
    return ResultDuplicateConstraint;
  }

  Tag tag(SymbolInvalid, SymbolInvalid);

  auto row = createRow(constraint, tag);

  auto subject = chooseSubjectForRow(*row, tag);

  if (subject.type() == Symbol::Type::Invalid && allDummiesInRow(*row)) {
    if (!NearZero(row->constant())) {
      return ResultUnsatisfiableConstraint;
    } else {
      subject = tag.marker();
    }
  }

  if (subject.type() == Symbol::Type::Invalid) {
    if (!addWithArtificialVariableOnRow(*row)) {
      return ResultUnsatisfiableConstraint;
    }
  } else {
    row->solveForSymbol(subject);
    substitute(subject, *row);
    _rows.insert({subject, std::move(row)});
  }

  _constraints.insert({std::move(constraint), tag});

  return optimizeObjectiveRow(*_objective);
}

const Result& Solver::removeConstraint(const Constraint& constraint) {
  auto foundConstraint = _constraints.find(constraint);
  if (foundConstraint == _constraints.end()) {
    return ResultUnknownConstraint;
  }

  Tag tag = foundConstraint->second;
  _constraints.erase(foundConstraint);

  removeConstraintEffects(constraint, tag);

  auto foundRow = _rows.find(tag.marker());
  if (foundRow != _rows.end()) {
    _rows.erase(foundRow);
  } else {
    auto leavingSymbol = leavingRowPairForMarkerSymbol(tag.marker());

    auto rowPair = _rows.find(leavingSymbol);
    if (rowPair == _rows.end()) {
      return ResultInternalSolverError;
    }

    auto leaving = rowPair->first;
    std::unique_ptr<Row> row(std::move(rowPair->second));
    RL_ASSERT(rowPair->second == nullptr);
    _rows.erase(rowPair);
    row->solveForSymbols(leaving, tag.marker());
    substitute(tag.marker(), *row);
  }

  return optimizeObjectiveRow(*_objective);
}

bool Solver::hasConstraint(const Constraint& constraint) const {
  return _constraints.find(constraint) != _constraints.end();
}

static inline bool IsValidNonRequiredPriority(double priority) {
  return (priority >= 0.0 && priority < priority::Required);
}

const Result& Solver::addEditVariable(const Variable& variable,
                                      double priority) {
  if (_edits.find(variable) != _edits.end()) {
    return ResultDuplicateEditVariable;
  }

  if (!IsValidNonRequiredPriority(priority)) {
    return ResultBadRequiredStrength;
  }

  Constraint constraint(Expression{{Term{variable, 1.0}}, 0.0},
                        Constraint::Relation::EqualTo, priority);

  if (addConstraint(constraint) != ResultSuccess) {
    return ResultInternalSolverError;
  }

  _edits.emplace(variable, _constraints.at(constraint), constraint, 0.0);

  return ResultSuccess;
}

const Result& Solver::removeEditVariable(const Variable& variable) {
  auto foundEdit = _edits.find(variable);

  if (foundEdit == _edits.end()) {
    return ResultUnknownEditVariable;
  }

  if (removeConstraint(foundEdit->second.constraint()) != ResultSuccess) {
    return ResultInternalSolverError;
  }

  _edits.erase(foundEdit);
  return ResultSuccess;
}

bool Solver::hasEditVariable(const Variable& variable) const {
  return _edits.find(variable) != _edits.end();
}

const Result& Solver::suggestValueForVariable(const Variable& variable,
                                              double value) {
  auto foundEdit = _edits.find(variable);

  if (foundEdit == _edits.end()) {
    return ResultUnknownEditVariable;
  }

  suggestValueForEditInfoWithoutDualOptimization(foundEdit->second, value);

  return dualOptimize();
}

void Solver::flushUpdates() {
#warning WIP skipped
}

Symbol Solver::symbolForVariable(const Variable& variable) {
  auto foundSymbol = _vars.find(variable);

  if (foundSymbol != _vars.end()) {
    return foundSymbol->second;
  }

  auto symbol = Symbol{Symbol::Type::External};
  _vars[variable] = symbol;
  return symbol;
}

std::unique_ptr<Row> Solver::createRow(const Constraint& constraint, Tag& tag) {
  auto const& expression = constraint.expression();
  auto row = core::make_unique<Row>(expression.constant());

  for (const auto& term : expression.terms()) {
    if (NearZero(term.coefficient())) {
      continue;
    }

    auto symbol = symbolForVariable(term.variable());
    auto foundRow = _rows.find(symbol);

    if (foundRow != _rows.end()) {
      row->insertRow(*(foundRow->second), term.coefficient());
    } else {
      row->insertSymbol(symbol, term.coefficient());
    }
  }

  switch (constraint.relation()) {
    case Constraint::Relation::LessThanOrEqualTo:
    case Constraint::Relation::GreaterThanOrEqualTo: {
      double coeffcient =
          constraint.relation() == Constraint::Relation::LessThanOrEqualTo
              ? 1.0
              : -1.0;

      auto slack = Symbol{Symbol::Type::Slack};
      tag.setOther(slack);
      row->insertSymbol(slack, coeffcient);

      if (constraint.priority() < priority::Required) {
        auto error = Symbol{Symbol::Type::Error};

        tag.setOther(error);

        row->insertSymbol(error, -coeffcient);

        _objective->insertSymbol(error, constraint.priority());
      }
    } break;
    case Constraint::Relation::EqualTo: {
      if (constraint.priority() < priority::Required) {
        auto errPlus = Symbol{Symbol::Type::Error};
        auto errMinus = Symbol{Symbol::Type::Error};

        tag.setMarker(errPlus);
        tag.setOther(errMinus);

        row->insertSymbol(errPlus, -1.0);
        row->insertSymbol(errMinus, 1.0);

        _objective->insertSymbol(errPlus, constraint.priority());
        _objective->insertSymbol(errMinus, constraint.priority());
      } else {
        auto dummy = Symbol{Symbol::Type::Dummy};
        tag.setMarker(dummy);
        row->insertSymbol(dummy);
      }
    } break;
  }

  if (row->constant() < 0.0) {
    row->reverseSign();
  }

  return row;
}

Symbol Solver::chooseSubjectForRow(const Row& row, const Tag& tag) const {
  for (const auto& cell : row.cells()) {
    if (cell.first.type() == Symbol::Type::External) {
      return cell.first;
    }
  }

  switch (tag.marker().type()) {
    case Symbol::Type::Slack:
    case Symbol::Type::Error:
      if (row.coefficientForSymbol(tag.marker()) < 0.0) {
        return tag.marker();
      }
      break;
    default:
      break;
  }

  switch (tag.other().type()) {
    case Symbol::Type::Slack:
    case Symbol::Type::Error:
      if (row.coefficientForSymbol(tag.other()) < 0.0) {
        return tag.other();
      }
      break;
    default:
      break;
  }

  return SymbolInvalid;
}

bool Solver::allDummiesInRow(const Row& row) const {
  for (const auto& cell : row.cells()) {
    if (cell.first.type() != Symbol::Type::Dummy) {
      return false;
    }
  }
  return true;
}

bool Solver::addWithArtificialVariableOnRow(const Row& row) {
  auto artificial = Symbol{Symbol::Type::Slack};
#warning check if the row emplace and artificial setup is a copy
  _rows.emplace(artificial, row);
  _artificial = core::make_unique<Row>(row);

  const auto& result = optimizeObjectiveRow(*_artificial);

  if (result.isError()) {
    return false;
  }

  auto success = NearZero(_artificial->constant());
  _artificial.reset();

  auto foundRowIterator = _rows.find(artificial);
  if (foundRowIterator != _rows.end()) {
    std::unique_ptr<Row> foundRow(std::move(foundRowIterator->second));
    _rows.erase(foundRowIterator);

    if (foundRow->cells().size() == 0) {
      return success;
    }

    const auto& entering = anyPivotableSymbol(*foundRow);
    if (entering.type() == Symbol::Type::Invalid) {
      return false;
    }

    foundRow->solveForSymbols(artificial, entering);
    substitute(entering, *foundRow);
    _rows[entering].swap(foundRow);
  }

  for (const auto& row : _rows) {
    row.second->removeSymbol(artificial);
  }

  _objective->removeSymbol(artificial);
  return success;
}

const Result& Solver::optimizeObjectiveRow(const Row& objective) {
  while (true) {
    auto entering = enteringSymbolForObjectiveRow(objective);
    if (entering.type() == Symbol::Type::Invalid) {
      return ResultSuccess;
    }

    auto leavingSymbol = leavingRowForEnteringSymbol(entering);

    auto leavingPair = _rows.find(leavingSymbol);
    if (leavingPair == _rows.end()) {
      return ResultInternalSolverError;
    }

    auto leaving = Symbol{leavingPair->first};

    std::unique_ptr<Row> row(std::move(leavingPair->second));
    RL_ASSERT(leavingPair->second == nullptr);

    _rows.erase(leavingPair);

    row->solveForSymbols(leaving, entering);
    substitute(entering, *row);

    _rows[entering].swap(row);
  }
}

const Symbol& Solver::enteringSymbolForObjectiveRow(const Row& objective) {
  const auto& cells = objective.cells();

  for (const auto& cell : cells) {
    if (cell.first.type() != Symbol::Type::Dummy && cell.second < 0.0) {
      return cell.first;
    }
  }

  return SymbolInvalid;
}

#warning remove the word "pair" from these APIs
Symbol Solver::leavingRowForEnteringSymbol(const Symbol& entering) const {
  auto ratio = std::numeric_limits<double>::max();
  auto found = SymbolInvalid;
  for (const auto& row : _rows) {
    if (row.first.type() != Symbol::Type::External) {
      auto temp = row.second->coefficientForSymbol(entering);
      if (temp < 0.0) {
        auto temp_ratio = -row.second->constant() / temp;
        if (temp_ratio < ratio) {
          ratio = temp_ratio;
          found = row.first;
        }
      }
    }
  }
  return found;
}

void Solver::substitute(const Symbol& symbol, const Row& row) {
  for (const auto& i : _rows) {
    i.second->substitute(symbol, row);
    if (i.first.type() != Symbol::Type::External &&
        i.second->constant() < 0.0) {
      _infeasibleRows.push_back(i.first);
    }
  }

  _objective->substitute(symbol, row);

  if (_artificial) {
    _artificial->substitute(symbol, row);
  }
}

Symbol Solver::anyPivotableSymbol(const Row& row) const {
  for (const auto& symbol : row.cells()) {
    switch (symbol.first.type()) {
      case Symbol::Type::Slack:
      case Symbol::Type::Error:
        return symbol.first;
        break;
      default:
        break;
    }
  }
  return SymbolInvalid;
}

void Solver::removeConstraintEffects(const Constraint& constraint,
                                     const Tag& tag) {
  if (tag.marker().type() == Symbol::Type::Error) {
    removeMarkerEffects(tag.marker(), constraint.priority());
  }

  if (tag.other().type() == Symbol::Type::Error) {
    removeMarkerEffects(tag.other(), constraint.priority());
  }
}

void Solver::removeMarkerEffects(const Symbol& marker, double strength) {
  auto foundRow = _rows.find(marker);
  if (foundRow != _rows.end()) {
    _objective->insertRow(*(foundRow->second), -strength);
  } else {
    _objective->insertSymbol(marker, -strength);
  }
}

Symbol Solver::leavingRowPairForMarkerSymbol(const Symbol& marker) const {
  auto r1 = std::numeric_limits<double>::max();
  auto r2 = std::numeric_limits<double>::max();

  auto first = SymbolInvalid;
  auto second = SymbolInvalid;
  auto third = SymbolInvalid;

  for (const auto& i : _rows) {
    double c = i.second->coefficientForSymbol(marker);
    if (c == 0.0) {
      continue;
    }
    if (i.first.type() == Symbol::Type::External) {
      third = i.first;
    } else if (c < 0.0) {
      auto r = -i.second->constant() / c;
      if (r < r1) {
        r1 = r;
        first = i.first;
      }
    } else {
      auto r = i.second->constant() / c;
      if (r < r2) {
        r2 = r;
        second = i.first;
      }
    }
  }

  if (first != SymbolInvalid) {
    return first;
  }

  if (second != SymbolInvalid) {
    return second;
  }

  return third;
}

void Solver::suggestValueForEditInfoWithoutDualOptimization(EditInfo& info,
                                                            double value) {
  auto delta = value - info.constant();
  info.setConstant(value);

  {
    auto foundRow = _rows.find(info.tag().marker());
    if (foundRow != _rows.end()) {
      if (foundRow->second->add(-delta) < 0.0) {
        _infeasibleRows.push_back(foundRow->first);
      }
      return;
    }
  }

  {
    auto foundRow = _rows.find(info.tag().other());
    if (foundRow != _rows.end()) {
      if (foundRow->second->add(delta) < 0.0) {
        _infeasibleRows.push_back(foundRow->first);
      }
      return;
    }
  }

  {
    for (const auto& i : _rows) {
      double coeff = i.second->coefficientForSymbol(info.tag().marker());
      if (coeff != 0.0 && i.second->add(delta * coeff) < 0.0 &&
          i.first.type() != Symbol::Type::External) {
        _infeasibleRows.push_back(i.first);
      }
    }
  }
}

const Result& Solver::dualOptimize() {
  while (_infeasibleRows.size() != 0) {
    auto leaving = _infeasibleRows.back();
    _infeasibleRows.pop_back();

    auto foundRow = _rows.find(leaving);
    if (foundRow != _rows.end() && foundRow->second->constant() < 0.0) {
      const auto& entering = dualEnteringSymbolForRow(*(foundRow->second));

      if (entering.type() == Symbol::Type::Invalid) {
        return ResultInternalSolverError;
      }

      std::unique_ptr<Row> row(std::move(foundRow->second));
      _rows.erase(foundRow);

      row->solveForSymbols(leaving, entering);
      substitute(entering, *row);
      _rows[entering].swap(row);
    }
  }
  return ResultSuccess;
}

Symbol Solver::dualEnteringSymbolForRow(const Row& row) {
  auto entering = SymbolInvalid;
  auto ratio = std::numeric_limits<double>::max();

  for (const auto& symbol : row.cells()) {
    auto value = symbol.second;
    if (value > 0.0 && symbol.first.type() != Symbol::Type::Dummy) {
      auto coeff = _objective->coefficientForSymbol(symbol.first);
      auto r = coeff / value;

      if (r < ratio) {
        ratio = r;
        entering = symbol.first;
      }
    }
  }

  return entering;
}

/*
 *  Bulk Editors
 */

template <class T>
const Result& Solver::bulkEdit(const std::list<T>& items,
                               UpdateCallback<T> applier,
                               UpdateCallback<T> undoer) {
  std::vector<std::reference_wrapper<T>> applied;
  auto needsCleanup = false;

  auto& result = ResultSuccess;

  for (const auto& item : items) {
    result = applier(item);
    if (result == ResultSuccess) {
      applied.push_back(item);
    } else {
      needsCleanup = true;
      break;
    }
  }

  if (needsCleanup) {
    for (auto i = applied.rbegin(); i != applied.rend(); ++i) {
      undoer(*i);
    }
  }

  return result;
}

const Result& Solver::addConstraints(const std::list<Constraint>& constraints) {
  UpdateCallback<Constraint> applier =
      std::bind(&Solver::addConstraint, this, std::placeholders::_1);
  UpdateCallback<Constraint> undoer =
      std::bind(&Solver::removeConstraint, this, std::placeholders::_1);

  return bulkEdit(constraints, applier, undoer);
}

const Result& Solver::removeConstraints(
    const std::list<Constraint>& constraints) {
  UpdateCallback<Constraint> applier =
      std::bind(&Solver::removeConstraint, this, std::placeholders::_1);
  UpdateCallback<Constraint> undoer =
      std::bind(&Solver::addConstraint, this, std::placeholders::_1);

  return bulkEdit(constraints, applier, undoer);
}

const Result& Solver::addEditVariables(const std::list<Variable> variables,
                                       double priority) {
  UpdateCallback<Variable> applier = [&, priority](const Variable& variable) {
    addEditVariable(variable, priority);
  };
  UpdateCallback<Variable> undoer =
      std::bind(&Solver::removeEditVariable, this, std::placeholders::_1);

  return bulkEdit(variables, applier, undoer);
}

const Result& Solver::removeEditVariables(const std::list<Variable> variables) {
  UpdateCallback<Variable> applier =
      std::bind(&Solver::removeEditVariable, this, std::placeholders::_1);
  UpdateCallback<Variable> undoer = [&](const Variable& variable) {
    addEditVariable(variable, _edits.at(variable).constraint().priority());
  };

  return bulkEdit(variables, applier, undoer);
}

}  // namespace layout
}  // namespace rl
