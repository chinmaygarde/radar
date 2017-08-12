/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/String.h>
#include <unicode/unistr.h>

namespace rl {
namespace type {

String::String() : _string(std::make_unique<icu::UnicodeString>()) {}

String::String(const std::string& string)
    : _string(std::make_unique<icu::UnicodeString>(
          icu::UnicodeString::fromUTF8(string))) {}

String::String(const String& other)
    : _string(std::make_unique<icu::UnicodeString>(*other._string.get())) {}

String::~String() = default;

void String::append(const String& other) {
  _string->append(*other._string.get());
}

size_t String::size() const {
  return _string->length();
}

size_t String::lengthOfCharacters() const {
  return _string->countChar32();
}

const icu::UnicodeString& String::unicodeString() const {
  return *_string;
}

}  // namespace type
}  // namespace rl
