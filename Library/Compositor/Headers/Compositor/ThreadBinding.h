// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_COMPOSITOR_THREADBINDING_H_
#define RADAR_COMPOSITOR_THREADBINDING_H_

#include <Core/Macros.h>

#include <thread>

namespace rl {
namespace compositor {

/**
 *  Asserts that the lifetime of a resource is tied to a particular thread.
 *  If the resource is not collected on the same thread it was bound, an
 *  assertion is triggered. This is purely a debugging aid and will result in
 *  a no-op in release builds (eventually).
 */
class ThreadBinding {
 public:
  ThreadBinding(bool bind = true);

  ~ThreadBinding();

  void bind();

  void unbind();

  bool isBound() const;

 private:
  std::thread::id _threadID;
  bool _bound;

  RL_DISALLOW_COPY_AND_ASSIGN(ThreadBinding);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADAR_COMPOSITOR_THREADBINDING_H_
