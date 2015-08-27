// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INSTRUMENTATION_TRACE_H__
#define __RADARLOVE_INSTRUMENTATION_TRACE_H__

#include <Core/Core.h>

namespace rl {

class Trace {
 public:
  explicit Trace() {}

  void begin(const char* name);
  void end();

 private:
  DISALLOW_COPY_AND_ASSIGN(Trace);
};

class AutoTrace : public Trace {
 public:
  AutoTrace(const char* name) { begin(name); }
  ~AutoTrace() { end(); };

 private:
  DISALLOW_COPY_AND_ASSIGN(AutoTrace);
};

}  // namespace rl

#endif  // __RADARLOVE_INSTRUMENTATION_TRACE_H__
