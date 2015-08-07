// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__UTILITIES__
#define __RADARLOVE__UTILITIES__

#include <memory>

namespace rl {

/**
 *  std::make_unique replacement for C++11
 *
 *  @param args the arguments to make_unique (forwarded)
 *
 *  @return the unique pointer
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}

#endif /* defined(__RADARLOVE__UTILITIES__) */
