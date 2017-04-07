// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Bundle.h>
#include <Interface/Interface.h>

namespace sample {

class SampleApplication : public rl::interface::InterfaceDelegate {
 public:
  SampleApplication();

  virtual void didFinishLaunching(rl::interface::Interface& interface) override;
  virtual void didBecomeActive(rl::interface::Interface& interface) override;
  virtual void didEnterBackground(rl::interface::Interface& interface) override;
  virtual void didTerminate(rl::interface::Interface& interface) override;
  virtual void didBecomeInactive(rl::interface::Interface& interface) override;

 private:
  rl::core::Bundle _bundle;

  void AddEntityWithImage(rl::interface::Interface& interface);
  void AddImageWithRoundedRect(rl::interface::Interface& interface);

  RL_DISALLOW_COPY_AND_ASSIGN(SampleApplication);
};

}  // namespace sample
