// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/Core.h>
#include <Layout/Solver.h>
#include <Layout/Parameter.h>
#include <Layout/ConstraintCreation.h>

TEST(LayoutTest, SimpleOperatorOverloadedConstruction) {
  rl::layout::Expression expr({}, 1.0);
  rl::layout::Expression expr2({}, 2.0);
  auto expr3 = expr + expr2;
  ASSERT_EQ(expr3.constant(), 3.0);
  ASSERT_EQ(expr3.terms().size(), 0);
}

TEST(LayoutTest, TermConstruction) {
  rl::layout::Variable v(0);
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
  rl::layout::Variable v(reinterpret_cast<rl::layout::Parameter*>(1));
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
  rl::layout::Variable v(reinterpret_cast<rl::layout::Parameter*>(1));
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
  rl::layout::Variable v1(reinterpret_cast<rl::layout::Parameter*>(1));
  rl::layout::Variable v2(reinterpret_cast<rl::layout::Parameter*>(2));
  rl::layout::Variable v3(reinterpret_cast<rl::layout::Parameter*>(3));
  rl::layout::Variable v4(reinterpret_cast<rl::layout::Parameter*>(4));
  rl::layout::Variable v5(reinterpret_cast<rl::layout::Parameter*>(5));
  rl::layout::Variable v6(reinterpret_cast<rl::layout::Parameter*>(6));

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
  rl::layout::Variable v(reinterpret_cast<rl::layout::Parameter*>(1));

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
  rl::layout::Variable v(reinterpret_cast<rl::layout::Parameter*>(1));

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
  rl::layout::Variable v(reinterpret_cast<rl::layout::Parameter*>(1));
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
  rl::layout::Parameter p1, p2, p3;

  rl::layout::Variable left = p1.asVariable();
  rl::layout::Variable mid = p2.asVariable();
  rl::layout::Variable right = p3.asVariable();

  rl::layout::Solver solver;

  ASSERT_EQ(solver.addConstraint(right + left == 2.0 * mid).isError(), false);
  ASSERT_EQ(solver.addConstraint(right - left >= 100).isError(), false);
  ASSERT_EQ(solver.addConstraint(left >= 0).isError(), false);

  solver.flushUpdates();

  ASSERT_EQ(p1.value(), 0.0);
  ASSERT_EQ(p2.value(), 50.0);
  ASSERT_EQ(p3.value(), 100.0);
}

TEST(LayoutTest, ParameterHoisting) {
  rl::layout::Parameter p1(100.0);
  rl::layout::Constraint constraint = p1 >= 10;

  ASSERT_EQ(constraint.expression().terms().size(), 1);
  ASSERT_EQ(constraint.expression().constant(), -10.0);
}

TEST(LayoutTest, ConstraintsAddParameterConst) {
  rl::layout::Parameter left, mid, right;

  rl::layout::Solver solver;

  ASSERT_EQ(solver.addConstraint(right + left == 2.0 * mid).isError(), false);
  ASSERT_EQ(solver.addConstraint(right - left >= 100).isError(), false);
  ASSERT_EQ(solver.addConstraint(left >= 0).isError(), false);

  solver.flushUpdates();

  ASSERT_EQ(left.value(), 0.0);
  ASSERT_EQ(mid.value(), 50.0);
  ASSERT_EQ(right.value(), 100.0);
}
