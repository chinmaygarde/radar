/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <string>

namespace rl {
namespace toolbox {
namespace string {

using String = std::string;

extern bool HasPrefix(const String& string, const String& prefix);

extern bool HasSuffix(const String& string, const String& suffix);

extern bool Contains(const String& string, const String& contains);

}  // namespace string
}  // namespace toolbox
}  // namespace rl
