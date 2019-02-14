/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Interface/InterfaceDelegate.h>

namespace rl {
namespace interface {

InterfaceDelegate::~InterfaceDelegate() = default;

DefaultInterfaceDelegate::DefaultInterfaceDelegate() = default;

void DefaultInterfaceDelegate::setInterfaceDidFinishLaunching(
    InterfaceCallback callback) {
  _didFinishLaunchingCallback = callback;
}

void DefaultInterfaceDelegate::setInterfaceDidBecomeActive(
    InterfaceCallback callback) {
  _didBecomeActiveCallback = callback;
}

void DefaultInterfaceDelegate::setInterfaceDidEnterBackground(
    InterfaceCallback callback) {
  _didEnterBackgroundCallback = callback;
}

void DefaultInterfaceDelegate::setInterfaceDidTerminate(
    InterfaceCallback callback) {
  _didTerminateCallback = callback;
}

void DefaultInterfaceDelegate::setInterfaceDidBecomeInactive(
    InterfaceCallback callback) {
  _didBecomeInactiveCallback = callback;
}

void DefaultInterfaceDelegate::didFinishLaunching(Interface& interface) {
  if (_didFinishLaunchingCallback) {
    _didFinishLaunchingCallback(interface);
  }
}
void DefaultInterfaceDelegate::didBecomeActive(Interface& interface) {
  if (_didBecomeActiveCallback) {
    _didBecomeActiveCallback(interface);
  }
}
void DefaultInterfaceDelegate::didEnterBackground(Interface& interface) {
  if (_didEnterBackgroundCallback) {
    _didEnterBackgroundCallback(interface);
  }
}
void DefaultInterfaceDelegate::didTerminate(Interface& interface) {
  if (_didTerminateCallback) {
    _didTerminateCallback(interface);
  }
}
void DefaultInterfaceDelegate::didBecomeInactive(Interface& interface) {
  if (_didBecomeInactiveCallback) {
    _didBecomeInactiveCallback(interface);
  }
}

}  // namespace interface
}  // namespace rl
