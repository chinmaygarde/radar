// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/Core.h>

#include <thread>

TEST(ThreadTest, GetSetName) {
  std::thread thread([] {
    const char name[] = "DeRp";
    rl::core::thread::SetName(name);
    ::strncmp(name, rl::core::thread::GetName().c_str(), sizeof(name));
  });
  thread.join();
}
