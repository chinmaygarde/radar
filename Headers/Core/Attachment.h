// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__ATTACHMENT__
#define __RADARLOVE__ATTACHMENT__

#include "Base.h"

namespace rl {
class Attachment {
 public:
  typedef int Handle;

  Attachment(Handle handle);
  ~Attachment();

  Handle handle() const { return _handle; }

 private:
  Handle _handle;
};
}

#endif /* defined(__RADARLOVE__ATTACHMENT__) */
