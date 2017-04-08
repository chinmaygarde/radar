/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Base64.h>
#include <base64/base64.h>

namespace rl {
namespace core {

Allocation Base64Decode(const uint8_t* data) {
  auto length = ::Base64decode_len(reinterpret_cast<const char*>(data));

  if (length <= 0) {
    return {};
  }

  Allocation decoded;

  if (!decoded.resize(length)) {
    return {};
  }

  auto newLength = ::Base64decode(reinterpret_cast<char*>(decoded.data()),
                                  reinterpret_cast<const char*>(data));

  if (newLength <= 0) {
    return {};
  }

  if (newLength != length) {
    if (!decoded.resize(newLength)) {
      return {};
    }
  }

  return decoded;
}

}  // namespace core
}  // namespace rl
