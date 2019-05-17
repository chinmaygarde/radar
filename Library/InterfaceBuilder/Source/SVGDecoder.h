/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>

#include <string>

#include <pugixml.hpp>

namespace rl {
namespace ib {

template <class T>
T Decode(const pugi::xml_node& node, const char* name, bool* present = nullptr);

}  // namespace ib
}  // namespace rl
