// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <pugixml.hpp>
#include <string>

namespace rl {
namespace ib {

template <class T>
T Decode(const pugi::xml_node& node, const char* name, bool* present = nullptr);

}  // namespace ib
}  // namespace rl
