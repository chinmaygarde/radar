/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Typography/FontDescriptor.h>
#include <Typography/String.h>
#include <map>

namespace rl {
namespace type {

class AtrributedStringBuilder;

class AttributedString {
 public:
  using FontDescriptorsMap =
      std::map<size_t, FontDescriptor, std::greater<size_t>>;

  ~AttributedString();

  bool isValid() const;

  const String& string() const;

  const FontDescriptorsMap& fontDescriptorsMap() const;

  FontDescriptor descriptorForIndex(size_t index) const;

 private:
  friend class AttributedStringBuilder;

  String _string;
  FontDescriptorsMap _fontDescriptors;

  AttributedString();

  AttributedString(String string, FontDescriptorsMap map);
};

}  // namespace type
}  // namespace rl
