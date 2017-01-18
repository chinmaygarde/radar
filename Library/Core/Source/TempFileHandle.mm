// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Platform.h>
#include <Core/TempFileHandle.h>
#include <stdlib.h>
#include <sstream>

#if RL_OS_MAC
#import <Foundation/Foundation.h>
#else  // RL_OS_MAC
#error Unsupported Platform.
#endif  // RL_OS_MAC

namespace rl {
namespace core {

core::URI TemporaryFileDirectory() {
#if RL_OS_MAC
  return {NSTemporaryDirectory().UTF8String};
#else
#error Unsupported Platform.
#endif
}

core::FileHandle TemporaryFileCreate(bool permanent) {
  std::stringstream stream;

  stream << TemporaryFileDirectory().filesystemRepresentation();
  stream << "rl.XXXXXX";

  auto fileTemplateString = stream.str();

  /*
   *  The const cast is fine because the template is replaced in place.
   */
  auto fileTemplate = const_cast<char*>(fileTemplateString.c_str());

  int handle = ::mkstemp(fileTemplate);

  if (handle == -1) {
    return -1;
  }

  if (!permanent) {
    RL_CHECK(::unlink(fileTemplate))
  }

  return {handle};
}

}  // namespace core
}  // namespace rl
