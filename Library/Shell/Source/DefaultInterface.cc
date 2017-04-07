// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <Interface/Interface.h>
#include <Interface/InterfaceDelegate.h>
#include <Shell/DefaultInterface.h>

namespace rl {
namespace shell {

std::unique_ptr<interface::InterfaceDelegate> CreateDefaultInterface() {
  /*
   *  The default interface just creates a blank surface with a dark background.
   */
  auto interface = std::make_unique<interface::DefaultInterfaceDelegate>();
  interface->setInterfaceDidFinishLaunching(
      [](interface::Interface& interface) -> void {
        auto& root = interface.rootEntity();
        root.setBackgroundColor({0.2, 0.2, 0.2, 1.0});
      });
  return std::move(interface);
}

}  // namespace shell
}  // namespace rl
