// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Utilities.h>

#if RL_OS_MAC
#include <Foundation/Foundation.h>
#endif

#include <sstream>

namespace rl {
namespace core {

int ToUnixTimeoutMS(ClockDurationNano nano) {
  if (nano.count() >= std::numeric_limits<int>::max()) {
    return -1;
  }
  return static_cast<int>(nano.count() / 1000000);
}

URI GetExecutablePath() {
  std::stringstream uriString;

  uriString << "file://";

#if RL_OS_MAC
  uriString << [[NSBundle mainBundle] executablePath].UTF8String;
#else
#error Unknown Platform
#endif

  URI uri(uriString.str());

  uri.normalize();

  return uri;
}

}  // namespace core
}  // namespace rl
