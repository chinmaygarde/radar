/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
