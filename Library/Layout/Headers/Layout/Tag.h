/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Layout/Symbol.h>

namespace rl {
namespace layout {

class Tag {
 public:
  Tag(const Symbol& maker, const Symbol& other);

  void setMarker(const Symbol& marker);

  const Symbol& marker() const;

  void setOther(const Symbol& other);

  const Symbol& other() const;

 private:
  Symbol _marker;
  Symbol _other;
};

}  // namespace layout
}  // namespace rl
