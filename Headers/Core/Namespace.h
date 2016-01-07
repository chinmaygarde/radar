// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>

#include <atomic>
#include <mutex>
#include <unordered_map>

namespace rl {
namespace core {

using Name = uint32_t;

static const Name DeadName = 0;

class Namespace {
 public:
  Namespace();

  Name create(Name counterpart = DeadName);

  void destroy(Name name);

 private:
  using NameMap = std::unordered_map<Name, Name>;

  std::atomic<Name> _last;
  std::mutex _lock;

  NameMap _localToCounterpartMap;
  NameMap _counterpartToLocalMap;

  RL_DISALLOW_COPY_AND_ASSIGN(Namespace);
};

}  // namespace core
}  // namespace rl
