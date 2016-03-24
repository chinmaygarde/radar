// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RL_TEST_RUNNER_H_
#define RL_TEST_RUNNER_H_

#if RL_USE_XCODE_TEST_RUNNER

#include <XCTest/XCTest.h>

/**
 *  Used to declare the start of a test body
 *
 *  @param name the name of the test
 */
#define RL_DECLARE_TEST_START(name) \
  @interface name : XCTestCase      \
  @end                              \
  @implementation name

/**
 *  Used to declare the end of a test body
 */
#define RL_DECLARE_TEST_END @end

/**
 *  Used to declare a new test case
 *
 *  @param className the test class name. This must be the same as the argument
 *                   passed to `RL_DECLARE_TEST_START`
 *  @param testName  the test case name
 */
#define TEST(className, testName) -(void)test##testName

/**
 *  Denote that this is a slow test
 *
 *  @param className the test class name. This must be the same as the argument
 *                   passed to `RL_DECLARE_TEST_START`
 *  @param testName  the test case name
 */
#define TEST_SLOW(className, testName) TEST(className, testName)

/*
 *  Assertions
 */

#define ASSERT_TRUE(case) XCTAssertTrue((case))
#define ASSERT_FALSE(case) XCTAssertFalse((case))

#define ASSERT_EQ(something, other) XCTAssertEqual((something), (other))
#define ASSERT_NE(something, other) XCTAssertNotEqual((something), (other))

#define ASSERT_GE(something, other) \
  XCTAssertGreaterThanOrEqual((something), (other))

#else  // RL_USE_XCODE_TEST_RUNNER

#define RL_DECLARE_TEST_START(name)
#define RL_DECLARE_TEST_END

#include <gtest/gtest.h>

#define TEST_SLOW(className, testName) TEST(className, RLSlowTest##testName)

#endif  // RL_USE_XCODE_TEST_RUNNER

#endif  // RL_TEST_RUNNER_H_
