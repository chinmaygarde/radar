// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <thread>
#include <Core/Core.h>
#include <Coordinator/EntityLease.h>

#if 0

TEST(InterfaceLeaseTest, SimpleSwap) {
  rl::EntityLease lease(256);

  // Get the write pointer
  auto write = lease.swapWriteAndNotify();
  memcpy(write, "Hello", 5);

  // Submit the write
  write = lease.swapWriteAndNotify();
  memcpy(write, "World", 5);

  // Get the read pointer
  auto read = lease.swapRead();

  ASSERT_TRUE(write != read);
  ASSERT_TRUE(memcmp(read, "Hello", 5) == 0);

  write = lease.swapWriteAndNotify();

  read = lease.swapRead();
  ASSERT_TRUE(write != read);
  ASSERT_TRUE(memcmp(read, "World", 5) == 0);
}

TEST(InterfaceLeaseTest, WritersCanBeFaster) {
  rl::InterfaceLease lease(256);

  // Get the write pointer
  auto write = lease.swapWriteAndNotify();
  memcpy(write, "Hello", 5);

  // Submit the write
  write = lease.swapWriteAndNotify();
  memcpy(write, "World", 5);

  // Get the read pointer
  auto read = lease.swapRead();

  ASSERT_TRUE(write != read);
  ASSERT_TRUE(memcmp(read, "Hello", 5) == 0);

  write = lease.swapWriteAndNotify();
  memcpy(write, "World", 5);

  ASSERT_TRUE(memcmp(read, "Hello", 5) == 0);

  write = lease.swapWriteAndNotify();
  memcpy(write, "ABCDE", 5);

  ASSERT_TRUE(memcmp(read, "Hello", 5) == 0);

  // Just because
  read = lease.swapRead();
  ASSERT_TRUE(memcmp(read, "World", 5) == 0);
}

#endif
