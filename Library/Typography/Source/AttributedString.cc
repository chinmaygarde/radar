/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/AttributedString.h>

namespace rl {
namespace type {

AttributedString::AttributedString() = default;

AttributedString::AttributedString(std::string string, FontDescriptorsMap map)
    : _string(std::move(string)), _fontDescriptors(std::move(map)) {}

AttributedString::~AttributedString() = default;

AttributedString::AttributedString(AttributedString&&) = default;

bool AttributedString::isValid() const {
  return _string.size() > 0 && _fontDescriptors.size() > 0;
}

const std::string& AttributedString::string() const {
  return _string;
}

const AttributedString::FontDescriptorsMap&
AttributedString::fontDescriptorsMap() const {
  return _fontDescriptors;
}

}  // namespace type
}  // namespace rl
