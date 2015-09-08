// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/Core.h>
#include <Recognition/Equation.h>

#include <thread>

TEST(RecognitionTest, SimpleInitialization) {
  /*
   *  -3x^2 + 6
   */
  rl::Variable variable;
  rl::Equation::Term term1(-3,
                           {rl::Equation::Term::VariableDegree(variable, 2)});
  rl::Equation::Term term2(6);
  rl::Equation equation({term1, term2});
  ASSERT_TRUE(equation.terms().size() == 2);
  ASSERT_TRUE(equation.terms()[0].coefficient() == -3);
  ASSERT_TRUE(equation.terms()[1].coefficient() == 6);
  ASSERT_TRUE(equation.terms()[0].variables().size() == 1);
  ASSERT_TRUE(equation.terms()[0].variables()[0].second == 2);
}
