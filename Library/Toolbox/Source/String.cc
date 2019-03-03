/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Toolbox/String.h>
#include <cstring>

namespace rl {
namespace toolbox {
namespace string {

bool HasPrefix(const String& string, const String& prefix) {
  if (prefix.size() > string.size() || string.size() == 0 ||
      prefix.size() == 0) {
    return false;
  }

  auto found = std::strstr(string.c_str(), prefix.c_str());

  if (found == nullptr) {
    return false;
  }

  if (found != string.c_str()) {
    return false;
  }

  return true;
}

bool HasSuffix(const String& string, const String& suffix) {
  if (suffix.size() > string.size() || string.size() == 0 ||
      suffix.size() == 0) {
    return false;
  }

  auto found = std::strstr(string.c_str(), suffix.c_str());

  if (found == nullptr) {
    return false;
  }

  auto compare = string.c_str() + (string.size() - suffix.length());

  if (found != compare) {
    return false;
  }

  return true;
}

bool Contains(const String& string, const String& contains) {
  if (contains.size() > string.size() || string.size() == 0 ||
      contains.size() == 0) {
    return false;
  }

  return std::strstr(string.c_str(), contains.c_str()) != nullptr;
}

}  // namespace string
}  // namespace toolbox
}  // namespace rl
