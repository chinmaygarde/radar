// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Channel.h>
#include <Core/Macros.h>
#include <string>

namespace rl {
namespace core {

class BootstrapClientProvider {
 public:
  virtual IOResult doAdvertise(const std::string& name,
                               std::shared_ptr<Channel> channel) = 0;

  virtual std::shared_ptr<Channel> doAcquire(const std::string& name) = 0;
};

}  // namespace core
}  // namespace rl
