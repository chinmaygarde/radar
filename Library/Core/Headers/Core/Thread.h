// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_THREAD_H_
#define RADAR_CORE_THREAD_H_

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

#endif  // RADAR_CORE_THREAD_H_
