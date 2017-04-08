/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <string>

namespace rl {
namespace core {
namespace thread {

/**
 *  Set the name of current thread of execution
 *
 *  @param name the name to set
 */
void SetName(const char* name);

/**
 *  Get the name of the current thread of execution
 *
 *  @return the name of the thread
 */
std::string GetName();

}  // namespace thread
}  // namespace core
}  // namespace rl
