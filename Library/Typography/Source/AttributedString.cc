/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/AttributedString.h>

namespace rl {
namespace type {

AttributedString::AttributedString() = default;

AttributedString::AttributedString(String string, FontDescriptorsMap map)
    : _string(std::move(string)), _fontDescriptors(std::move(map)) {}

AttributedString::~AttributedString() = default;

bool AttributedString::isValid() const {
  return _string.lengthOfCharacters() > 0 && _fontDescriptors.size() > 0;
}

const String& AttributedString::string() const {
  return _string;
}

const AttributedString::FontDescriptorsMap&
AttributedString::fontDescriptorsMap() const {
  return _fontDescriptors;
}

FontDescriptor AttributedString::descriptorForIndex(size_t index) const {
  if (_fontDescriptors.size() == 0) {
    return {};
  }

  return _fontDescriptors.lower_bound(index)->second;
}

}  // namespace type
}  // namespace rl
