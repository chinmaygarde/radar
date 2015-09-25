// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_INTERFACEDELEGATE_
#define RADARLOVE_INTERFACE_INTERFACEDELEGATE_

namespace rl {

class Interface;
class InterfaceDelegate {
 public:
  /**
   *  The interface has finished launching and and may become active soon
   *
   *  @param interface the actual interface
   */
  virtual void didFinishLaunching(Interface& interface) = 0;

  /**
   *  The interface has become active
   *
   *  @param interface the actual interface
   */
  virtual void didBecomeActive(Interface& interface) = 0;

  /**
   *  The interface has entered background execution state
   *
   *  @param interface the actual interface
   */
  virtual void didEnterBackground(Interface& interface) = 0;

  /**
   *  The interface has terminated
   *
   *  @param interface the actual interface
   */
  virtual void didTerminate(Interface& interface) = 0;

  /**
   *  The interface has become inactive
   *
   *  @param interface the actual interface
   */
  virtual void didBecomeInactive(Interface& interface) = 0;

  /**
   *  The interface size was updated. The new size may be accessed via the
   *  size() property
   *
   *  @param interface the interface whose size was updated
   */
  virtual void didUpdateSize(Interface& interface) = 0;
};
}

#endif  // RADARLOVE_INTERFACE_INTERFACEDELEGATE_
