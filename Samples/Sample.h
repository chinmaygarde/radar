// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_SAMPLE_H__
#define __RADARLOVE_SAMPLE_H__

#include <Interface/Interface.h>

namespace sample {
class SampleApplication : public rl::InterfaceDelegate {
 public:
  SampleApplication();

  virtual void didFinishLaunching(rl::Interface& interface) override;
  virtual void didBecomeActive(rl::Interface& interface) override;
  virtual void didEnterBackground(rl::Interface& interface) override;
  virtual void didTerminate(rl::Interface& interface) override;
  virtual void didBecomeInactive(rl::Interface& interface) override;
  virtual void didUpdateSize(rl::Interface& interface) override;

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(SampleApplication);
};

}  // namespace sample

#endif /* __RADARLOVE_SAMPLE_H__ */
