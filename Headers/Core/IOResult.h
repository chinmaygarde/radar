// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_IORESULT_H_
#define RADARLOVE_CORE_IORESULT_H_

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

#endif  // RADARLOVE_CORE_IORESULT_H_
