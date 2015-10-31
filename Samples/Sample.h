// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_SAMPLE_H__
#define __RADARLOVE_SAMPLE_H__

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
  virtual void didUpdateSize(rl::interface::Interface& interface) override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(SampleApplication);
};

}  // namespace sample

#endif /* __RADARLOVE_SAMPLE_H__ */
