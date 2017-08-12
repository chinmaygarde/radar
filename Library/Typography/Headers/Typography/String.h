/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Typography/Types.h>
#include <memory>
#include <string>

namespace rl {
namespace type {

class String {
 public:
  String();

  String(const std::string& string);

  String(const String&);

  ~String();

  void append(const String& other);

  size_t size() const;

  size_t lengthOfCharacters() const;

  const icu::UnicodeString& unicodeString() const;

 private:
  std::unique_ptr<icu::UnicodeString> _string;
};

}  // namespace type
}  // namespace rl
