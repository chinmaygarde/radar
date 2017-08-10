/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Typography/FontDescriptor.h>
#include <map>

namespace rl {
namespace type {

class AtrributedStringBuilder;

class AttributedString {
 public:
  using FontDescriptorsMap =
      std::map<size_t, FontDescriptor, std::less<size_t>>;

  ~AttributedString();

  AttributedString(AttributedString&&);

  bool isValid() const;

  const std::string& string() const;

  const FontDescriptorsMap& fontDescriptorsMap() const;

 private:
  friend class AttributedStringBuilder;

  std::string _string;
  FontDescriptorsMap _fontDescriptors;

  AttributedString();

  AttributedString(std::string string, FontDescriptorsMap map);

  RL_DISALLOW_COPY_AND_ASSIGN(AttributedString);
};

}  // namespace type
}  // namespace rl
