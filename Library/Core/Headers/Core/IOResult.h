/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Message.h>

namespace rl {
namespace core {

enum class IOResult {
  /**
   *  The IO operation in question was successfully completed within the
   *  (optional) timeout
   */
  Success,
  /**
   *  The IO operation in question was an unconditional failure
   */
  Failure,
  /**
   *  The IO operation in question failed due to a timeout
   */
  Timeout,
};

/**
 *  The result of the read message. Returns the actual message as the second
 *  argument if the result was `IOResult::Success`. If not, an empty message
 *  is present instead.
 */
using IOReadResult = std::pair<IOResult, Message>;

}  // namespace core
}  // namespace rl
