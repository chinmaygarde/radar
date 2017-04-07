// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <Core/URI.h>

namespace rl {
namespace type {

class Font {
 public:
  Font(core::URI uri);

  ~Font();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Font);
};

}  // namespace type
}  // namespace rl
