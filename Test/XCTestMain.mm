// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef RL_USE_XCODE_TEST_RUNNER

#import <XCTest/XCTest.h>
#include <gtest/gtest.h>

@interface RadarLoveTest : XCTestCase

@end

@implementation RadarLoveTest

- (void)testAll {
  auto arguments = [[NSProcessInfo processInfo] arguments];

  auto argc = static_cast<int>(arguments.count);
  char* argv[argc];

  for (auto i = 0; i < argc; i++) {
    argv[i] = strdup(arguments[i].UTF8String);
  }

  ::testing::InitGoogleTest(&argc, argv);
  XCTAssertEqual(RUN_ALL_TESTS(), 0,
                 @"The GTest harness must be run the tests successfully");

  for (auto i = 0; i < argc; i++) {
    free(argv[i]);
  }
}

@end

#endif  // RL_USE_XCODE_TEST_RUNNER