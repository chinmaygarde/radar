// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Utilities.h>

#if RL_OS_MAC
#include <Foundation/Foundation.h>
#elif RL_OS_LINUX
#include <unistd.h>
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
  uriString << [[[NSBundle mainBundle] executablePath]
                   stringByAddingPercentEncodingWithAllowedCharacters:
                       [NSCharacterSet URLQueryAllowedCharacterSet]]
                   .UTF8String;
#elif RL_OS_LINUX
  const int pathSize = 256;
  char path[pathSize] = {0};
  auto readSize = ::readlink("/proc/self/exe", path, pathSize);
  if (readSize != -1) {
    uriString << std::string(path, readSize);
  }
#else
#error Unknown Platform
#endif

  URI uri(uriString.str());

  if (!uri.normalize()) {
    return URI{};
  }

  return uri;
}

URI GetResourcesPath() {
#if RL_OS_MAC
  std::stringstream uriString;

  uriString << "file://";

  uriString << [[[NSBundle mainBundle] resourcePath]
                   stringByAddingPercentEncodingWithAllowedCharacters:
                       [NSCharacterSet URLQueryAllowedCharacterSet]]
                   .UTF8String;
  uriString << "/Resources";

  URI uri(uriString.str());

  if (!uri.normalize()) {
    return URI{};
  }

  return uri;
#elif RL_OS_LINUX
  return GetExecutablePath().append(URI{".."});
#else
#error Unknown Platform
#endif
}

}  // namespace core
}  // namespace rl
