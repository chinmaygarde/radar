/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
