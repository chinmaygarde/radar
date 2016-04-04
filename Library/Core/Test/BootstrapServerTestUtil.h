// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_TEST_BOOTSTRAPSERVERTESTUTIL_H_
#define RADARLOVE_TEST_BOOTSTRAPSERVERTESTUTIL_H_

#include <Core/Macros.h>
#include <Core/BootstrapServer.h>

namespace rl {
namespace test {

class AutoBootstrapServer {
 public:
  AutoBootstrapServer() {  //
    core::BootstrapServerSetup();
  }

  ~AutoBootstrapServer() {  //
    core::BootstrapServerTeardown();
  }

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AutoBootstrapServer);
};

}  // namespace test
}  // namespace rl

#endif  // RADARLOVE_TEST_BOOTSTRAPSERVERTESTUTIL_H_
