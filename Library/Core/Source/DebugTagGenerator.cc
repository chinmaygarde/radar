/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/DebugTagGenerator.h>
#include <sstream>

namespace rl {
namespace core {

DebugTagGenerator::DebugTagGenerator(const std::string& prefix)
    : _prefix(prefix), _last(0) {}

std::string DebugTagGenerator::createSpecific(const std::string& tag) {
  return _prefix + tag;
}

std::string DebugTagGenerator::acquire() {
  static const char* dictionary[] = {"tinker",     "tailor",   "soldier",
                                     "sailor",     "rich_man", "poor_man",
                                     "begger_man", "thief"};
  const auto index = _last++ % (sizeof(dictionary) / sizeof(char*));
  std::stringstream stream;
  stream << _prefix << "." << dictionary[index] << "." << _last;
  return stream.str();
}

}  // namespace core
}  // namespace rl
