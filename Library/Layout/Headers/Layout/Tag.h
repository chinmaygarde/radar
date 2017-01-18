// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_LAYOUT_TAG_H_
#define RADAR_LAYOUT_TAG_H_

#include <Core/Core.h>
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

#endif  // RADAR_LAYOUT_TAG_H_
