// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/Core.h>
#include <Recognition/Equation.h>
#include <Recognition/GestureRecognizer.h>

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
