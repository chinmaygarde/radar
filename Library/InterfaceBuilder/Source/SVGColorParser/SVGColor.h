// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_SVGCOLOR_H_
#define RADAR_INTERFACEBUILDER_SVGCOLOR_H_

#include <Core/Macros.h>
#include <Entity/Color.h>

namespace rl {
namespace ib {

entity::Color ColorFromString(const char* color);

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_SVGCOLOR_H_