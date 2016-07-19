// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_TYPOGRAPHY_TYPEFRAME_H_
#define RADAR_TYPOGRAPHY_TYPEFRAME_H_

#include <Core/Macros.h>

#include <string>

namespace rl {
namespace type {

class TypeFrame {
 public:
  TypeFrame(const std::string& string);

  ~TypeFrame();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(TypeFrame);
};

}  // namespace type
}  // namespace rl

#endif  // RADAR_TYPOGRAPHY_TYPEFRAME_H_
