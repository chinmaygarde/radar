/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/FileHandle.h>
#include <Core/Macros.h>

namespace rl {
namespace core {

core::URI TemporaryFileDirectory();

core::FileHandle TemporaryFileCreate(bool permanent = false);

}  // namespace core
}  // namespace rl
