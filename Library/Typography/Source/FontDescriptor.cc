/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Typography/FontDescriptor.h>

namespace rl {
namespace type {

FontDescriptor::FontDescriptor() : FontDescriptor("Helvetica", 14.0) {}

FontDescriptor::FontDescriptor(std::string postscriptName, double pointSize)
    : _postscriptName(std::move(postscriptName)), _pointSize(pointSize) {}

FontDescriptor::~FontDescriptor() = default;

const std::string& FontDescriptor::postscriptName() const {
  return _postscriptName;
}

double FontDescriptor::pointSize() const {
  return _pointSize;
}

bool FontDescriptor::operator==(const FontDescriptor& other) const {
  return _pointSize == other._pointSize &&
         _postscriptName == other._postscriptName;
}

}  // namespace type
}  // namespace rl
