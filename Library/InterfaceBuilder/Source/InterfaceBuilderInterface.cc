// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <InterfaceBuilder/InterfaceBuilderInterface.h>
#include <Core/FileMapping.h>

namespace rl {
namespace ib {

InterfaceBuilderInterface::InterfaceBuilderInterface(
    std::unique_ptr<InterfaceBuilderArchive> archive)
    : _archive(std::move(archive)) {}

InterfaceBuilderInterface::~InterfaceBuilderInterface() = default;

bool InterfaceBuilderInterface::isValid() const {
  return _archive != nullptr && _archive->isValid();
}

void InterfaceBuilderInterface::didFinishLaunching(
    interface::Interface& interface) {
  if (!isValid()) {
    return;
  }
  ib::InterfaceBuilderArchive::EntityMap map;
  interface.rootEntity().addChild(_archive->inflate(interface, map));
}

void InterfaceBuilderInterface::didBecomeActive(
    interface::Interface& interface) {
  //
}

void InterfaceBuilderInterface::didEnterBackground(
    interface::Interface& interface) {
  //
}

void InterfaceBuilderInterface::didTerminate(interface::Interface& interface) {
  //
}

void InterfaceBuilderInterface::didBecomeInactive(
    interface::Interface& interface) {
  //
}

}  // namespace ib
}  // namespace rl
