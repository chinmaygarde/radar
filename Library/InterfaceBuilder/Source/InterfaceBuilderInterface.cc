/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/FileMapping.h>
#include <InterfaceBuilder/InterfaceBuilderInterface.h>

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
