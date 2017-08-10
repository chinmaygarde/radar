/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Typography/AttributedString.h>
#include <Typography/FontDescriptor.h>
#include <Typography/Types.h>
#include <sstream>
#include <stack>
#include <string>

namespace rl {
namespace type {

class AttributedStringBuilder {
 public:
  AttributedStringBuilder();

  ~AttributedStringBuilder();

  AttributedStringBuilder& appendText(const std::string& text);

  AttributedStringBuilder& pushFontDescriptor(FontDescriptor descriptor);

  AttributedStringBuilder& popFontDescriptor();

  AttributedString attributedString() const;

 private:
  std::stringstream _stringBuilder;
  AttributedString::FontDescriptorsMap _fontDescriptorsMap;
  std::stack<FontDescriptor> _fontDescriptorsStack;
  size_t _currentStringIndex = 0;

  RL_DISALLOW_COPY_AND_ASSIGN(AttributedStringBuilder);
};

}  // namespace type
}  // namespace rl
