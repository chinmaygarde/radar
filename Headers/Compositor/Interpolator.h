// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_INTERPOLATOR__
#define __RADARLOVE_COMPOSITOR_INTERPOLATOR__

#include <Core/Core.h>
#include <Interface/Action.h>

namespace rl {
template <typename Type>
class Interpolator {
 public:
  Interpolator(const Action& action, const Type& from, const Type& to)
      : _action(action), _from(from), _to(to){};

 private:
  const Action _action;
  const Type _from;
  const Type _to;

  DISALLOW_ASSIGN(Interpolator);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_INTERPOLATOR__) */
