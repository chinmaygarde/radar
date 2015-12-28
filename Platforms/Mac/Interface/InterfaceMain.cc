// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Interface.h>
#include "Sample.h"

int main(int argc, const char* argv[]) {
  auto application = std::make_shared<sample::SampleApplication>();
  rl::interface::Interface interface(application);
  interface.run();
  return EXIT_SUCCESS;
}
