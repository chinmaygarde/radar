// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INSTRUMENTATION_TRACE_H_
#define RADARLOVE_INSTRUMENTATION_TRACE_H_

#include <Core/Core.h>

namespace rl {
namespace instrumentation {

class Trace {
 public:
  explicit Trace() {}

  void begin(const char* name);
  void end();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(Trace);
};

class AutoTrace : public Trace {
 public:
  AutoTrace(const char* name) { begin(name); }
  ~AutoTrace() { end(); }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AutoTrace);
};

}  // namespace instrumentation
}  // namespace rl

#endif  // RADARLOVE_INSTRUMENTATION_TRACE_H_
