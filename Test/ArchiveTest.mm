// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RadarTest.h"

#include <Core/Archive.h>

#include <thread>
#include <cstdio>

class Sample : public rl::core::Archivable {
 public:
  Sample(uint64_t count = 42) : _someData(count) {}

  uint64_t someData() const { return _someData; };

  bool writeToArchive(rl::core::ArchiveItem& item) const override {
    return item.encode(999, _someData);
  };

  bool readFromArchive(rl::core::ArchiveItem& item) override { return false; };

 private:
  uint64_t _someData;

  RL_DISALLOW_COPY_AND_ASSIGN(Sample);
};

template <>
class rl::core::ArchiveDef<Sample> {
 public:
  std::string className() { return "sample"; }

  rl::core::Archivable::Members members() { return {999}; }
};

class FooBar {};

template <>
class rl::core::ArchiveDef<FooBar> {
 public:
  std::string className() { return "foo"; }

  rl::core::Archivable::Members members() { return {1, 2, 3}; }
};

RL_DECLARE_TEST_START(ArchiveTest)

TEST(ArchiveTest, SimpleInitialization) {
  auto name = "/tmp/sample.db";
  {
    rl::core::Archive archive(name);
    ASSERT_TRUE(archive.isReady());
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST(ArchiveTest, AddStorageClass) {
  auto name = "/tmp/sample2.db";
  {
    rl::core::Archive archive(name);
    ASSERT_TRUE(archive.isReady());

    auto res = archive.registerDefinition<FooBar>();

    ASSERT_TRUE(res);
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST(ArchiveTest, AddData) {
  auto name = "/tmp/sample3.db";
  {
    rl::core::Archive archive(name);
    ASSERT_TRUE(archive.isReady());

    auto res = archive.registerDefinition<Sample>();

    ASSERT_TRUE(res);

    Sample sample;
    ASSERT_TRUE(archive.archive(sample));
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST(ArchiveTest, AddDataMultiple) {
  auto name = "/tmp/sample4.db";
  {
    rl::core::Archive archive(name);
    ASSERT_TRUE(archive.isReady());

    auto res = archive.registerDefinition<Sample>();

    ASSERT_TRUE(res);

    for (auto i = 0; i < 100; i++) {
      Sample sample(i + 1);
      ASSERT_TRUE(archive.archive(sample));
    }
  }
  ASSERT_TRUE(::remove(name) == 0);
}

RL_DECLARE_TEST_END
