// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RadarLove_Core_DebugTagGenerator_h_
#define RadarLove_Core_DebugTagGenerator_h_

#include <Core/Macros.h>
#include <atomic>
#include <string>

namespace rl {
namespace core {

class DebugTagGenerator {
 public:
  DebugTagGenerator(const std::string& prefix);

  std::string createSpecific(const std::string& tag);

  std::string acquire();

 private:
  std::string _prefix;
  std::atomic<uint32_t> _last;

  RL_DISALLOW_COPY_AND_ASSIGN(DebugTagGenerator);
};

}  // namespace core
}  // namespace rl

#endif  // RadarLove_Core_DebugTagGenerator_h_
