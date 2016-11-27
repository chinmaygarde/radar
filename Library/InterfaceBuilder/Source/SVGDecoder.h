// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_SVGDECODER_H_
#define RADAR_INTERFACEBUILDER_SVGDECODER_H_

#include <Core/Macros.h>
#include <pugixml.hpp>

namespace rl {
namespace ib {

template <class T>
T Decode(const pugi::xml_node& node, const char* name);

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_SVGDECODER_H_
