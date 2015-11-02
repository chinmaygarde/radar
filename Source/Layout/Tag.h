// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_LAYOUT_TAG_H_
#define RADARLOVE_LAYOUT_TAG_H_

#include <Core/Core.h>
#include <Layout/Symbol.h>

namespace rl {
namespace layout {

class Tag {
 public:
  Tag(const Symbol& maker, const Symbol& other);

  const Symbol& marker() const;

  const Symbol& other() const;

 private:
  const Symbol _marker;
  const Symbol _other;

  RL_DISALLOW_COPY_AND_ASSIGN(Tag);
};

}  // namespace layout
}  // namespace rl

#endif  // RADARLOVE_LAYOUT_TAG_H_
