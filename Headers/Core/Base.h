// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_BASE_H__
#define __RADARLOVE_BASE_H__

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(TypeName&) = delete;            \
  void operator=(TypeName) = delete;

#include <memory>

namespace rl {
namespace Utils {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}
}

#endif /* defined(__RADARLOVE_BASE_H__) */
