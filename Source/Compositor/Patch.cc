// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Patch.h>
#include <Interface/Layer.h>

namespace rl {

Patch::Patch() : _marks() {
}

void Patch::mark(PatchChunk chunk) {
  _marks.emplace_back(chunk);
}

bool Patch::hasMessage() const {
  return _marks.size() > 0;
}

Message Patch::flatten() const {
  /*
   *  TODO:
   *  This form of flattening is extremely inefficient due the the presence of
   *  union. Since we already know the sizes of the data arguments based on
   *  command, it is easy to optimize this routine to not waste space.
   */
  Message message(_marks.size() * sizeof(PatchChunk));

  for (const auto& chunk : _marks) {
    message.encode(chunk);
  }

  return message;
}

}  // namespace rl
