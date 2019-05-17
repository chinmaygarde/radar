/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>

#include "MachPort.h"

namespace rl {
namespace core {
namespace testing {

#if RL_CHANNELS == RL_CHANNELS_MACH

TEST(MachPortTest, CanCreatePorts) {
  MachPort port(MachPort::Type::None);
  ASSERT_FALSE(port.isValid());
}

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

}  // namespace testing
}  // namespace core
}  // namespace rl
