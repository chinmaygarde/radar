// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_TURQUOISE_H__
#define __RADARLOVE_TURQUOISE_H__

#include <Interface/Interface.h>

namespace tq {
class TurquoiseApplication : public rl::InterfaceDelegate {
 public:
  TurquoiseApplication();

  virtual void didFinishLaunching(rl::Interface& interface) override;
  virtual void didBecomeActive(rl::Interface& interface) override;
  virtual void didEnterBackground(rl::Interface& interface) override;
  virtual void didTerminate(rl::Interface& interface) override;
  virtual void didBecomeInactive(rl::Interface& interface) override;
  virtual void didUpdateSize(rl::Interface& interface) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(TurquoiseApplication);
};
}

#endif /* __RADARLOVE_TURQUOISE_H__ */
