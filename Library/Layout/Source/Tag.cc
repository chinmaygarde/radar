/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Layout/Tag.h>

namespace rl {
namespace layout {

Tag::Tag(const Symbol& marker, const Symbol& other)
    : _marker(marker), _other(other) {}

void Tag::setMarker(const Symbol& marker) {
  _marker = marker;
}

const Symbol& Tag::marker() const {
  return _marker;
}

void Tag::setOther(const Symbol& other) {
  _other = other;
}

const Symbol& Tag::other() const {
  return _other;
}

}  // namespace layout
}  // namespace rl
