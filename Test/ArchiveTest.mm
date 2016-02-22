// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RadarTest.h"

#include <Core/Archive.h>

#include <cstdio>
#include <thread>

static rl::core::ArchiveSerializable::ArchiveName LastSample = 0;

class Sample : public rl::core::ArchiveSerializable {
 public:
  Sample(uint64_t count = 42) : _someData(count), _name(++LastSample) {}

  uint64_t someData() const { return _someData; };

  ArchiveName archiveName() const override { return _name; }

  bool serialize(rl::core::ArchiveItem& item) const override {
    return item.encode(999, _someData);
  };

  bool deserialize(rl::core::ArchiveItem& item,
                   rl::core::Namespace* ns) override {
    _name = item.name();
    return item.decode(999, _someData);
  };

  static const rl::core::ArchiveDef ArchiveDefinition;

 private:
  uint64_t _someData;
  ArchiveName _name;

  RL_DISALLOW_COPY_AND_ASSIGN(Sample);
};

const rl::core::ArchiveDef Sample::ArchiveDefinition = {
    .superClass = nullptr,
    .className = "Sample",
    .autoAssignName = false,
    .members = {999},
};

RL_DECLARE_TEST_START(ArchiveTest)

TEST(ArchiveTest, SimpleInitialization) {
  auto name = "/tmp/sample.db";
  {
    rl::core::Archive archive(name, true);
    ASSERT_TRUE(archive.isReady());
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST(ArchiveTest, AddStorageClass) {
  auto name = "/tmp/sample2.db";
  {
    rl::core::Archive archive(name, true);
    ASSERT_TRUE(archive.isReady());
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST(ArchiveTest, AddData) {
  auto name = "/tmp/sample3.db";
  {
    rl::core::Archive archive(name, true);
    ASSERT_TRUE(archive.isReady());
    Sample sample;
    ASSERT_TRUE(archive.archive(sample));
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST_SLOW(ArchiveTest, AddDataMultiple) {
  auto name = "/tmp/sample4.db";
  {
    rl::core::Archive archive(name, true);
    ASSERT_TRUE(archive.isReady());

    for (auto i = 0; i < 100; i++) {
      Sample sample(i + 1);
      ASSERT_TRUE(archive.archive(sample));
    }
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST_SLOW(ArchiveTest, ReadData) {
  auto name = "/tmp/sample5.db";
  {
    rl::core::Archive archive(name, true);
    ASSERT_TRUE(archive.isReady());

    size_t count = 50;

    std::vector<rl::core::ArchiveSerializable::ArchiveName> keys;
    std::vector<uint64_t> values;

    keys.reserve(count);
    values.reserve(count);

    for (auto i = 0; i < count; i++) {
      Sample sample(i + 1);
      keys.push_back(sample.archiveName());
      values.push_back(sample.someData());
      ASSERT_TRUE(archive.archive(sample));
    }

    for (auto i = 0; i < count; i++) {
      Sample sample;
      ASSERT_TRUE(archive.unarchive(keys[i], sample, nullptr));
      ASSERT_EQ(values[i], sample.someData());
    }
  }
  ASSERT_TRUE(::remove(name) == 0);
}

TEST(ArchiveTest, ReadDataWithNames) {
  auto name = "/tmp/sample6.db";
  {
    rl::core::Archive archive(name, true);
    ASSERT_TRUE(archive.isReady());

    size_t count = 8;

    std::vector<rl::core::ArchiveSerializable::ArchiveName> keys;
    std::vector<uint64_t> values;

    keys.reserve(count);
    values.reserve(count);

    for (auto i = 0; i < count; i++) {
      Sample sample(i + 1);
      keys.push_back(sample.archiveName());
      values.push_back(sample.someData());
      ASSERT_TRUE(archive.archive(sample));
    }

    for (auto i = 0; i < count; i++) {
      Sample sample;
      ASSERT_TRUE(archive.unarchive(keys[i], sample, nullptr));
      ASSERT_EQ(values[i], sample.someData());
      ASSERT_EQ(keys[i], sample.archiveName());
    }
  }
  ASSERT_TRUE(::remove(name) == 0);
}

RL_DECLARE_TEST_END
