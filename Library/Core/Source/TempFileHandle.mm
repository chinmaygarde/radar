/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Platform.h>
#include <Core/TempFileHandle.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#if RL_OS_MAC
#import <Foundation/Foundation.h>
#elif RL_OS_LINUX
// Nothing.
#else  // RL_OS_MAC
#error Unsupported Platform.
#endif  // RL_OS_MAC

namespace rl {
namespace core {

core::URI TemporaryFileDirectory() {
#if RL_OS_MAC
  return {NSTemporaryDirectory().UTF8String};
#elif RL_OS_LINUX
  return {"/tmp"};
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
