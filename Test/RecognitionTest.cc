// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/Core.h>
#include <Recognition/Polynomial.h>
#include <Recognition/GestureRecognizer.h>

#if 0

TEST(RecognitionTest, SimpleInitialization) {
  /*
   *  Simple Pan: y = mx + c
   *  y(entity.position) = m(1) * x(touch.position) + c(something)
   */

  rl::Entity entity(false);

  rl::Variable y(entity, rl::Entity::Position);
  rl::Variable x(rl::Variable::Proxy::AllTouches, rl::Entity::Position);

  rl::Term term(1.0, {{x, 1.0}});
  rl::Equation equation({term}, 0.0);

  rl::GestureRecognizer recognizer(std::move(y), std::move(equation));

  ASSERT_TRUE(recognizer.equation().terms().size() == 1);
  auto firstTerm = recognizer.equation().terms()[0];

  ASSERT_TRUE(firstTerm.degree() == 1);
  ASSERT_TRUE(recognizer.equation().degree() == 1);
  ASSERT_TRUE(firstTerm.degree() == recognizer.equation().degree());
}

TEST(RecognitionTest, ComplexInitialization) {
  rl::Entity entity(false);
  rl::Entity entity2(false);
  rl::Entity entity3(false);

  rl::Variable y(entity, rl::Entity::Position);
  rl::Variable x1(rl::Variable::Proxy::AllTouches, rl::Entity::Position);
  rl::Variable x2(entity2, rl::Entity::Position);
  rl::Variable x3(entity3, rl::Entity::Position);

  rl::Term term(1.0, {{x1, 1.0}});
  rl::Term term2(2.0, {{x2, 4.0}, {x1, 1.0}});
  rl::Term term3(4.0, {{x2, 2.0}, {x1, 1.0}, {x3, 7.0}});
  rl::Equation equation({term, term2, term3}, 4.0);

  rl::GestureRecognizer recognizer(std::move(y), std::move(equation));

  ASSERT_TRUE(recognizer.equation().terms().size() == 3);

  auto firstTerm = recognizer.equation().terms()[0];
  auto secondTerm = recognizer.equation().terms()[1];
  auto thirdTerm = recognizer.equation().terms()[2];

  ASSERT_TRUE(firstTerm.degree() == 1);
  ASSERT_TRUE(secondTerm.degree() == 5);
  ASSERT_TRUE(thirdTerm.degree() == 10);

  ASSERT_TRUE(recognizer.equation().degree() == 10);
  ASSERT_TRUE(recognizer.equation().constant() == 4.0);
}

TEST(RecognitionTest, OperatorOverload1) {
  rl::Entity entity(false);

  rl::Variable x(entity, rl::Entity::Position);

  auto variableDegree = x ^ 2;
  RL_ASSERT(variableDegree.second == 2);
}

#if 0

TEST(RecognitionTest, ComplexInitializationOperatedOverloaded) {
  rl::Entity entity(false);
  rl::Entity entity2(false);
  rl::Entity entity3(false);

  rl::Variable y(entity, rl::Entity::Position);
  rl::Variable x1(rl::Variable::Proxy::AllTouches, rl::Entity::Position);
  rl::Variable x2(entity2, rl::Entity::Position);
  rl::Variable x3(entity3, rl::Entity::Position);

  /**
   *  (1 * (x1 ^ 1)) + (2 * (x2 ^ 4) * (x1 ^ 1)) + (4 * (x2 ^ 2) * (x1 ^ 1) *
   *  (x3 ^ 7)) + 4.0
   */

  rl::Equation equation((1 * (x1 ^ 1)) + (2 * (x2 ^ 4) * (x1 ^ 1)) +
                        (4 * (x2 ^ 2) * (x1 ^ 1) * (x3 ^ 7)) + 4.0);

  rl::GestureRecognizer recognizer(std::move(y), std::move(equation));

  ASSERT_TRUE(recognizer.equation().terms().size() == 3);

  auto firstTerm = recognizer.equation().terms()[0];
  auto secondTerm = recognizer.equation().terms()[1];
  auto thirdTerm = recognizer.equation().terms()[2];

  ASSERT_TRUE(firstTerm.degree() == 1);
  ASSERT_TRUE(secondTerm.degree() == 5);
  ASSERT_TRUE(thirdTerm.degree() == 10);

  ASSERT_TRUE(recognizer.equation().degree() == 10);
  ASSERT_TRUE(recognizer.equation().constant() == 4.0);
}

#endif

#endif
