// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/Core.h>
#include <Entity/Entity.h>
#include <Layout/ConstraintCreation.h>
#include <Layout/Solver.h>

TEST(LayoutTest, SimpleOperatorOverloadedConstruction) {
  rl::layout::Expression expr({}, 1.0);
  rl::layout::Expression expr2({}, 2.0);
  auto expr3 = expr + expr2;
  ASSERT_EQ(expr3.constant(), 3.0);
  ASSERT_EQ(expr3.terms().size(), 0);
}

TEST(LayoutTest, TermConstruction) {
  rl::core::Namespace ns;

  rl::layout::Variable v(rl::core::Name{ns});
  rl::layout::Term term(v, 1.0, false);
  rl::layout::Term term2(v, 2.0, false);

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
  rl::core::Namespace ns;

  rl::layout::Variable v(rl::core::Name{ns});
  rl::layout::Term term2(v, 2.0, false);

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
  rl::core::Namespace ns;

  rl::layout::Variable v(rl::core::Name{ns});
  rl::layout::Term term2(v, 2.0, false);

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
  rl::core::Namespace ns;

  rl::layout::Variable v1(rl::core::Name{ns});
  rl::layout::Variable v2(rl::core::Name{ns});
  rl::layout::Variable v3(rl::core::Name{ns});
  rl::layout::Variable v4(rl::core::Name{ns});
  rl::layout::Variable v5(rl::core::Name{ns});
  rl::layout::Variable v6(rl::core::Name{ns});

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
  rl::core::Namespace ns;

  rl::layout::Variable v(rl::core::Name{ns});

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
  rl::core::Namespace ns;

  rl::layout::Variable v(rl::core::Name{ns});

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
  rl::core::Namespace ns;

  rl::layout::Variable v(rl::core::Name{ns});
  auto constraint =
      2.0 * v + v * 2.5 <= 2 * (-400 + 5.0 * v) | rl::layout::priority::Strong;

  rl::core::Message message;
  ASSERT_EQ(message.encode(constraint), true);
  ASSERT_NE(message.size(), 0);

  auto sizeWritten = message.size();

  rl::core::Channel channel;

  rl::core::Messages messages;
  messages.emplace_back(std::move(message));

  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  auto readMessages =
      channel.drainPendingMessages(rl::core::ClockDurationNano{0});

  ASSERT_EQ(readMessages.size(), 1);

  auto& readMessage = readMessages[0];

  rl::layout::Constraint decoded;
  ASSERT_EQ(readMessage.decode(decoded, &ns), true);
  ASSERT_EQ(readMessage.sizeRead(), sizeWritten);

  ASSERT_EQ(decoded.expression().terms().size(), 3);
  ASSERT_EQ(decoded.priority(), rl::layout::priority::Strong);
  ASSERT_EQ(decoded.relation(),
            rl::layout::Constraint::Relation::LessThanOrEqualTo);
  ASSERT_EQ(decoded.expression().terms()[2].coefficient(), -10.0);
}

TEST(LayoutTest, ConstraintsAdd) {
  rl::core::Namespace ns;

  rl::core::Name one(rl::core::Name{ns}), two(rl::core::Name{ns}),
      three(rl::core::Name{ns});

  rl::layout::Variable p1(one), p2(two), p3(three);

  rl::layout::Variable left = p1;
  rl::layout::Variable mid = p2;
  rl::layout::Variable right = p3;

  rl::layout::Solver solver(ns);

  ASSERT_EQ(solver.addConstraint(right + left == 2.0 * mid),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(right - left >= 100),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(left >= 0), rl::layout::Result::Success);

  std::map<rl::core::Name, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
        return rl::layout::Solver::FlushResult::Updated;
      });

  ASSERT_EQ(updates[one], 0.0);
  ASSERT_EQ(updates[two], 50.0);
  ASSERT_EQ(updates[three], 100.0);
}

TEST(LayoutTest, ParameterHoisting) {
  rl::core::Namespace ns;
  rl::layout::Variable p1(rl::core::Name{ns});
  rl::layout::Constraint constraint = p1 >= 10;

  ASSERT_EQ(constraint.expression().terms().size(), 1);
  ASSERT_EQ(constraint.expression().constant(), -10.0);
}

TEST(LayoutTest, ConstraintsAddParameterConst) {
  rl::core::Namespace ns;

  rl::core::Name one(rl::core::Name{ns}), two(rl::core::Name{ns}),
      three(rl::core::Name{ns});

  rl::layout::Variable left(one), mid(two), right(three);

  rl::layout::Solver solver(ns);

  ASSERT_EQ(solver.addConstraint(right + left == 2.0 * mid),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(right - left >= 100),
            rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(left >= 0), rl::layout::Result::Success);

  std::map<rl::core::Name, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
        return rl::layout::Solver::FlushResult::Updated;
      });

  ASSERT_EQ(updates[one], 0.0);
  ASSERT_EQ(updates[two], 50.0);
  ASSERT_EQ(updates[three], 100.0);
}

TEST(LayoutTest, UpdatesInSolver) {
  rl::core::Namespace ns;

  rl::layout::Solver solver(ns);

  rl::layout::Variable left(rl::core::Name{ns});
  rl::layout::Variable right(rl::core::Name{ns});

  auto c1 = right - left >= 200.0;
  auto c2 = right >= right;

  ASSERT_EQ(solver.addConstraint(c1), rl::layout::Result::Success);
  ASSERT_EQ(solver.addConstraint(c1), rl::layout::Result::DuplicateConstraint);
  ASSERT_EQ(solver.removeConstraint(c2), rl::layout::Result::UnknownConstraint);
  ASSERT_EQ(solver.removeConstraint(c1), rl::layout::Result::Success);
  ASSERT_EQ(solver.removeConstraint(c1), rl::layout::Result::UnknownConstraint);
}

TEST(LayoutTest, EditUpdates) {
  rl::core::Namespace ns;
  rl::layout::Solver solver(ns);

  rl::layout::Variable left(rl::core::Name{ns});
  rl::layout::Variable right(rl::core::Name{ns});
  rl::layout::Variable mid(rl::core::Name{ns});

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
  rl::core::Namespace ns;

  rl::core::Name one(rl::core::Name{ns}), two(rl::core::Name{ns}),
      three(rl::core::Name{ns});

  rl::layout::Variable left(one), right(two), mid(three);

  rl::layout::Solver solver(ns);

  auto res = solver.addConstraints({
      right + left == 2.0 * mid,  //
      right - left >= 100.0,      //
      left >= 0.0,                //
  });

  ASSERT_EQ(res, rl::layout::Result::Success);

  ASSERT_EQ(solver.addEditVariable(mid, rl::layout::priority::Strong),
            rl::layout::Result::Success);

  ASSERT_EQ(solver.applySuggestion({mid, 300, rl::layout::priority::Strong}),
            rl::layout::Result::Success);

  std::map<rl::core::Name, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
        return rl::layout::Solver::FlushResult::Updated;
      });

  ASSERT_EQ(updates[one], 0.0);
  ASSERT_EQ(updates[two], 600.0);
  ASSERT_EQ(updates[three], 300.0);
}

TEST(LayoutTest, SolverSolutionWithOptimize) {
  rl::core::Namespace ns;

  rl::core::Name one(rl::core::Name{ns}), two(rl::core::Name{ns}),
      three(rl::core::Name{ns}), four(rl::core::Name{ns});

  rl::layout::Variable p1(one), p2(two), p3(three), container(four);

  rl::layout::Solver solver(ns);

  auto res = solver.addEditVariable(container, rl::layout::priority::Strong);

  ASSERT_EQ(res, rl::layout::Result::Success);

  res =
      solver.applySuggestion({container, 100.0, rl::layout::priority::Strong});

  ASSERT_EQ(res, rl::layout::Result::Success);

  res = solver.addConstraints({
      p1 >= 30.0 | rl::layout::priority::Strong,  //
      p1 == p3 | rl::layout::priority::Medium,    //
      p2 == 2.0 * p1,                             //
      container == p1 + p2 + p3                   //
  });

  ASSERT_EQ(res, rl::layout::Result::Success);

  std::map<rl::core::Name, double> updates;
  solver.flushUpdates(
      [&updates](const rl::layout::Variable& var, double value) {
        updates[var.identifier()] = value;
        return rl::layout::Solver::FlushResult::Updated;
      });

  ASSERT_EQ(updates[four], 100.0);
  ASSERT_EQ(updates[one], 30.0);
  ASSERT_EQ(updates[two], 60.0);
  ASSERT_EQ(updates[three], 10.0);
}

TEST(LayoutTest, VariableCreationViaOverloading) {
  rl::core::Namespace ns;

  rl::entity::Entity entity(rl::core::Name{ns}, nullptr);
  auto variable = entity | rl::layout::Variable::Property::BoundsOriginX;
  ASSERT_EQ(variable.property(), rl::layout::Variable::Property::BoundsOriginX);
  ASSERT_EQ(variable.identifier(), entity.identifier());
}

TEST(LayoutTest, MakeConstantHoistableVariants) {
  rl::core::Namespace ns;

  auto expr1 = rl::layout::MakeConst(10.0);
  ASSERT_EQ(expr1.constant(), 10.0);

  rl::layout::Variable var(rl::core::Name{ns});
  auto expr2 = rl::layout::MakeConst(var);
  ASSERT_EQ(expr2.terms().size(), 1);

  rl::layout::Term term(var, 1.0, false);
  auto expr3 = rl::layout::MakeConst(term);
  ASSERT_EQ(expr3.terms().size(), 1);
  ASSERT_EQ(expr3.terms()[0].isConstant(), true);

  auto expr4 = rl::layout::MakeConst(expr3);
  ASSERT_EQ(expr4.terms().size(), 1);
  ASSERT_EQ(expr4.terms()[0].isConstant(), true);
}
