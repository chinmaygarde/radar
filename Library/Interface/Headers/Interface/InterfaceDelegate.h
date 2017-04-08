/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <functional>

namespace rl {
namespace interface {

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
};

class DefaultInterfaceDelegate : public InterfaceDelegate {
 public:
  using InterfaceCallback = std::function<void(Interface&)>;

  DefaultInterfaceDelegate();

  void setInterfaceDidFinishLaunching(InterfaceCallback callback);

  void setInterfaceDidBecomeActive(InterfaceCallback callback);

  void setInterfaceDidEnterBackground(InterfaceCallback callback);

  void setInterfaceDidTerminate(InterfaceCallback callback);

  void setInterfaceDidBecomeInactive(InterfaceCallback callback);

 private:
  InterfaceCallback _didFinishLaunchingCallback;
  InterfaceCallback _didBecomeActiveCallback;
  InterfaceCallback _didEnterBackgroundCallback;
  InterfaceCallback _didTerminateCallback;
  InterfaceCallback _didBecomeInactiveCallback;

  void didFinishLaunching(Interface& interface) override;
  void didBecomeActive(Interface& interface) override;
  void didEnterBackground(Interface& interface) override;
  void didTerminate(Interface& interface) override;
  void didBecomeInactive(Interface& interface) override;

  RL_DISALLOW_COPY_AND_ASSIGN(DefaultInterfaceDelegate);
};

}  // namespace interface
}  // namespace rl
