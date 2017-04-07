// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Channel.h>
#include <Core/IOResult.h>
#include <Core/Macros.h>
#include "BootstrapClientProvider.h"

namespace rl {
namespace core {

class BootstrapClient {
 public:
  BootstrapClient();

  ~BootstrapClient();

  IOResult advertise(const std::string& name, std::shared_ptr<Channel> channel);

  std::shared_ptr<Channel> acquire(const std::string& name);

 private:
  std::unique_ptr<BootstrapClientProvider> _provider;

  RL_DISALLOW_COPY_AND_ASSIGN(BootstrapClient);
};

}  // namespace core
}  // namespace rl
