/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/AttributedStringBuilder.h>

namespace rl {
namespace type {

AttributedStringBuilder::AttributedStringBuilder() {
  FontDescriptor defaultFontDescriptor;
  _fontDescriptorsStack.push(defaultFontDescriptor);
  _fontDescriptorsMap[0] = defaultFontDescriptor;
}

AttributedStringBuilder::~AttributedStringBuilder() = default;

AttributedStringBuilder& AttributedStringBuilder::appendText(
    const std::string& stringBuffer) {
  String string(stringBuffer);
  if (string.lengthOfCharacters() == 0) {
    return *this;
  }
  _currentStringIndex += string.size();
  _stringBuilder.append(string);
  return *this;
}

AttributedStringBuilder& AttributedStringBuilder::pushFontDescriptor(
    FontDescriptor descriptor) {
  RL_ASSERT(_fontDescriptorsStack.size() > 0);
  if (_fontDescriptorsStack.top() == descriptor) {
    return *this;
  }
  _fontDescriptorsMap[_currentStringIndex] = descriptor;
  _fontDescriptorsStack.push(descriptor);
  return *this;
}

AttributedStringBuilder& AttributedStringBuilder::popFontDescriptor() {
  if (_fontDescriptorsStack.size() > 1) {
    _fontDescriptorsStack.pop();
  }
  _fontDescriptorsMap[_currentStringIndex] = _fontDescriptorsStack.top();
  return *this;
}

AttributedString AttributedStringBuilder::attributedString() const {
  if (_stringBuilder.lengthOfCharacters() == 0) {
    return {};
  }

  /*
   *  Remove items that were added without a corresponding string.
   */
  auto fontDescriptors = _fontDescriptorsMap;
  fontDescriptors.erase(_currentStringIndex);
  return {_stringBuilder, std::move(fontDescriptors)};
}

}  // namespace type
}  // namespace rl
