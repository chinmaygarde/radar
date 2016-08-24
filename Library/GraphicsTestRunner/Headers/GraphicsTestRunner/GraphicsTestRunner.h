// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_TEST_RUNNER_H_
#define RL_TEST_RUNNER_H_

#include <gtest/gtest.h>

#define TEST_SLOW(className, testName) TEST(className, RLSlowTest##testName)

#endif  // RL_TEST_RUNNER_H_
