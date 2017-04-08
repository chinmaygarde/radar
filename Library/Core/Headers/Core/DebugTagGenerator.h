/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
