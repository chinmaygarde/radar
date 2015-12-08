// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Core.h>
#include <Layout/ConstraintCreation.h>
#include <Layout/Solver.h>
#include <gtest/gtest.h>

#include <map>

TEST(LayoutTest, SimpleOperatorOverloadedConstruction) {
  rl::layout::Expression expr({}, 1.0);
  rl::layout::Expression expr2({}, 2.0);
  auto expr3 = expr + expr2;
  ASSERT_EQ(expr3.constant(), 3.0);
  ASSERT_EQ(expr3.terms().size(), 0);
}

TEST(LayoutTest, TermConstruction) {
  rl::layout::Variable v(1);
  rl::layout::Term term(v, 1.0);
  rl::layout::Term term2(v, 2.0);

  auto expr = term + 1.0;
  auto expr2 = term + v;
  auto expr3 = term + term2;
  auto expr4 = term + expr3;

  ASSERT_EQ(expr.terms().size(), 1);
  ASSERT_EQ(expr.constant(), 1.0);
  ASSERT_EQ(expr2.terms().size(), 2);
  ASSERT_EQ(expr3.terms().size(), 2);
  ASSERT_EQ(expr4.terms().size(), 3);
}

TEST(LayoutTest, VariableConstruction) {
  rl::layout::Variable v(1);
  rl::layout::Term term2(v, 2.0);

  auto expr = v + 1.0;
  auto expr2 = v + v;
  auto expr3 = v + term2;
  auto expr4 = v + expr3;

  ASSERT_EQ(expr.terms().size(), 1);
  ASSERT_EQ(expr.constant(), 1.0);
  ASSERT_EQ(expr2.terms().size(), 2);
  ASSERT_EQ(expr3.terms().size(), 2);
  ASSERT_EQ(expr4.terms().size(), 3);
}

TEST(LayoutTest, DoubleConstruction) {
  rl::layout::Variable v(1);
  rl::layout::Term term2(v, 2.0);

  auto expr2 = 2.0 + v;
  auto expr3 = 3.0 + term2;
  auto expr4 = 4.0 + expr3;
  auto term5 = -2.0 * v;
  auto term6 = v * -2.0;
  auto term = v / -2.0;

  ASSERT_EQ(expr2.terms().size(), 1);
  ASSERT_EQ(expr3.terms().size(), 1);
  ASSERT_EQ(expr4.terms().size(), 1);
  ASSERT_EQ(term5.terms().size(), 1);
  ASSERT_EQ(term5.terms()[0].coefficient(), -2.0);
  ASSERT_EQ(term6.terms().size(), 1);
  ASSERT_EQ(term6.terms()[0].coefficient(), -2.0);
  ASSERT_EQ(term.terms().size(), 1);
  ASSERT_EQ(term.terms()[0].coefficient(), -0.5);
}

TEST(LayoutTest, ComplexOperationOverload) {
  rl::layout::Variable v1(1);
  rl::layout::Variable v2(2);
  rl::layout::Variable v3(3);
  rl::layout::Variable v4(4);
  rl::layout::Variable v5(5);
  rl::layout::Variable v6(6);

  auto expr = (2.0 * v1) + (v2 / 0.5) + (v3 * 3) - 300.0;
  ASSERT_EQ(expr.constant(), -300.0);
  ASSERT_EQ(expr.terms().size(), 3);
  ASSERT_EQ((expr.terms()[0]).coefficient(), 2.0);
  ASSERT_EQ((expr.terms()[1]).coefficient(), 2.0);
  ASSERT_EQ((expr.terms()[2]).coefficient(), 3.0);

  auto expr2 = (3.0 * v4) - (v5 / 0.25) + (v6 * -6.0) + 10.0;
  ASSERT_EQ(expr2.constant(), 10);
  ASSERT_EQ(expr2.terms().size(), 3);
  ASSERT_EQ((expr2.terms()[0]).coefficient(), 3.0);
  ASSERT_EQ((expr2.terms()[1]).coefficient(), -4.0);
  ASSERT_EQ((expr2.terms()[2]).coefficient(), -6.0);

  auto expr3 = (3.0 * v4) - 3.0 * ((v5 / 0.25) + (v6 * -6.0) - 10.0);
  ASSERT_EQ(expr3.constant(), 30.0);
  ASSERT_EQ(expr3.terms().size(), 3);
  ASSERT_EQ((expr3.terms()[0]).coefficient(), 3.0);
  ASSERT_EQ((expr3.terms()[1]).coefficient(), -12.0);
  ASSERT_EQ((expr3.terms()[2]).coefficient(), 18.0);

  auto expr4 = expr - 2 * expr3;
  ASSERT_EQ(expr4.constant(), -360.0);
  ASSERT_EQ(expr4.terms().size(), 6);
}

TEST(LayoutTest, ConstraintCreation) {
  rl::layout::Variable v(1);

  rl::layout::Constraint constraint = 2.0 * v + 35 == 0;
  ASSERT_EQ(constraint.expression().terms().size(), 1);
  ASSERT_EQ(constraint.expression().constant(), 35);
  ASSERT_EQ(constraint.relation(), rl::layout::Constraint::Relation::EqualTo);
  ASSERT_EQ(constraint.priority(), rl::layout::priority::Required);

  rl::layout::Constraint constraint2 = 2.0 * v + v * 2.5 >= 400;
  ASSERT_EQ(constraint2.expression().terms().size(), 2);
  ASSERT_EQ(constraint2.expression().constant(), -400);
  ASSERT_EQ(constraint2.relation(),
            rl::layout::Constraint::Relation::GreaterThanOrEqualTo);
  ASSERT_EQ(constraint2.priority(), rl::layout::priority::Required);

  rl::layout::Constraint constraint3 = 2.0 * v + v * 2.5 <= -400;
  ASSERT_EQ(constraint3.expression().terms().size(), 2);
  ASSERT_EQ(constraint3.expression().constant(), 400);
  ASSERT_EQ(constraint3.relation(),
            rl::layout::Constraint::Relation::LessThanOrEqualTo);
  ASSERT_EQ(constraint3.priority(), rl::layout::priority::Required);

  rl::layout::Constraint constraint4 =
      2.0 * v + v * 2.5 == 2 * (-400 + 5.0 * v);
  ASSERT_EQ(constraint4.expression().terms().size(), 3);
  ASSERT_EQ(constraint4.expression().constant(), 800);
  ASSERT_EQ(constraint4.relation(), rl::layout::Constraint::Relation::EqualTo);
  ASSERT_EQ(constraint4.priority(), rl::layout::priority::Required);
  ASSERT_EQ(constraint4.expression().terms()[2].coefficient(), -10);
}

TEST(LayoutTest, ConstraintCreationAtPriority) {
  rl::layout::Variable v(1);

  rl::layout::Constraint constraint =
      2.0 * v + 35 == 0 | rl::layout::priority::Strong;
  ASSERT_EQ(constraint.expression().terms().size(), 1);
  ASSERT_EQ(constraint.expression().constant(), 35);
  ASSERT_EQ(constraint.relation(), rl::layout::Constraint::Relation::EqualTo);
  ASSERT_EQ(constraint.priority(), rl::layout::priority::Strong);

  rl::layout::Constraint constraint2 =
      rl::layout::priority::Strong | 2.0 * v + 35 == 0;
  ASSERT_EQ(constraint2.expression().terms().size(), 1);
  ASSERT_EQ(constraint2.expression().constant(), 35);
  ASSERT_EQ(constraint2.relation(), rl::layout::Constraint::Relation::EqualTo);
  ASSERT_EQ(constraint2.priority(), rl::layout::priority::Strong);
}

TEST(LayoutTest, SerializeDeserializeConstraint) {
  rl::layout::Variable v(1);
  auto constraint =
      2.0 * v + v * 2.5 <= 2 * (-400 + 5.0 * v) | rl::layout::priority::Strong;

  rl::core::Message message;
  ASSERT_EQ(message.encode(constraint), true);
  ASSERT_NE(message.size(), 0);

  auto sizeWritten = message.size();
  message.rewindRead();

  rl::layout::Constraint decoded;
  ASSERT_EQ(message.decode(decoded), true);
  ASSERT_EQ(message.sizeRead(), sizeWritten);

  ASSERT_EQ(decoded.expression().terms().size(), 3);
  ASSERT_EQ(decoded.priority(), rl::layout::priority::Strong);
  ASSERT_EQ(decoded.relation(),
            rl::layout::Constraint::Relation::LessThanOrEqualTo);
  ASSERT_EQ(decoded.expression().terms()[2].coefficient(), -10.0);
}

TEST(LayoutTest, ConstraintsAdd) {
  rl::layout::Variable p1(1), p2(2), p3(3);

  rl::layout::Variable left = p1;
  rl::layout::Variable mid = p2;
  rl::layout::Variable right = p3;

  rl::layout::Solver solver;

  ASSERT_EQ(solver.addConstraint(right + left == 2.0 * mid),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(right - left >= 100),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(left >= 0), rl::layout::Result::Success);

  std::map<rl::interface::Entity::Identifier, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
      });

  ASSERT_EQ(updates[1], 0.0);
  ASSERT_EQ(updates[2], 50.0);
  ASSERT_EQ(updates[3], 100.0);
}

TEST(LayoutTest, ParameterHoisting) {
  rl::layout::Variable p1(100);
  rl::layout::Constraint constraint = p1 >= 10;

  ASSERT_EQ(constraint.expression().terms().size(), 1);
  ASSERT_EQ(constraint.expression().constant(), -10.0);
}

TEST(LayoutTest, ConstraintsAddParameterConst) {
  rl::layout::Variable left(1), mid(2), right(3);

  rl::layout::Solver solver;

  ASSERT_EQ(solver.addConstraint(right + left == 2.0 * mid),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(right - left >= 100),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(left >= 0), rl::layout::Result::Success);

  std::map<rl::interface::Entity::Identifier, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
      });

  ASSERT_EQ(updates[1], 0.0);
  ASSERT_EQ(updates[2], 50.0);
  ASSERT_EQ(updates[3], 100.0);
}

TEST(LayoutTest, UpdatesInSolver) {
  rl::layout::Solver solver;

  rl::layout::Variable left(1);
  rl::layout::Variable right(2);

  auto c1 = right - left >= 200.0;
  auto c2 = right >= right;

  ASSERT_EQ(solver.addConstraint(c1), rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(c1), rl::layout::Result::DuplicateConstraint);
  ASSERT_EQ(solver.removeConstraint(c2), rl::layout::Result::UnknownConstraint);
  ASSERT_EQ(solver.removeConstraint(c1), rl::layout::Result::Success);
  ASSERT_EQ(solver.removeConstraint(c1), rl::layout::Result::UnknownConstraint);
}

TEST(LayoutTest, EditUpdates) {
  rl::layout::Solver solver;

  rl::layout::Variable left(1);
  rl::layout::Variable right(2);
  rl::layout::Variable mid(3);

  auto c = left + right >= 2.0 * mid;
  ASSERT_EQ(solver.addConstraint(c), rl::layout::Result::Success);
  ASSERT_EQ(solver.addEditVariable(mid, 999), rl::layout::Result::Success);
  ASSERT_EQ(solver.addEditVariable(mid, 999),
            rl::layout::Result::DuplicateEditVariable);
  ASSERT_EQ(solver.removeEditVariable(mid), rl::layout::Result::Success);
  ASSERT_EQ(solver.removeEditVariable(mid),
            rl::layout::Result::UnknownEditVariable);
}

TEST(LayoutTest, EditConstraintFlush) {
  rl::layout::Variable left(1), right(2), mid(3);

  rl::layout::Solver solver;

  auto res = solver.addConstraints({
      right + left == 2.0 * mid,  //
      right - left >= 100.0,      //
      left >= 0.0,                //
  });

  ASSERT_EQ(res, rl::layout::Result::Success);

  ASSERT_EQ(solver.addEditVariable(mid, rl::layout::priority::Strong),
            rl::layout::Result::Success);

  ASSERT_EQ(solver.applySuggestions({{mid, 300}}), rl::layout::Result::Success);

  std::map<rl::interface::Entity::Identifier, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
      });

  ASSERT_EQ(updates[1], 0.0);
  ASSERT_EQ(updates[3], 300.0);
  ASSERT_EQ(updates[2], 600.0);
}

TEST(LayoutTest, SolverSolutionWithOptimize) {
  rl::layout::Variable p1(1), p2(2), p3(3), container(4);

  rl::layout::Solver solver;

  auto res = solver.addEditVariable(container, rl::layout::priority::Strong);

  ASSERT_EQ(res, rl::layout::Result::Success);

  res = solver.applySuggestions({{container, 100.0}});

  ASSERT_EQ(res, rl::layout::Result::Success);

  res = solver.addConstraints({
      p1 >= 30.0 | rl::layout::priority::Strong,  //
      p1 == p3 | rl::layout::priority::Medium,    //
      p2 == 2.0 * p1,                             //
      container == p1 + p2 + p3                   //
  });

  ASSERT_EQ(res, rl::layout::Result::Success);

  std::map<rl::interface::Entity::Identifier, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
      });

  ASSERT_EQ(updates[4], 100.0);
  ASSERT_EQ(updates[1], 30.0);
  ASSERT_EQ(updates[2], 60.0);
  ASSERT_EQ(updates[3], 10.0);
}
