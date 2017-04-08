/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/FileHandle.h>
#include <Core/Macros.h>
#include <Interface/InterfaceDelegate.h>
#include <InterfaceBuilder/InterfaceBuilderArchive.h>

namespace rl {
namespace ib {

class InterfaceBuilderInterface : public interface::InterfaceDelegate {
 public:
  InterfaceBuilderInterface(std::unique_ptr<InterfaceBuilderArchive> archive);

  ~InterfaceBuilderInterface();

  bool isValid() const;

 private:
  std::unique_ptr<InterfaceBuilderArchive> _archive;

  void didFinishLaunching(interface::Interface& interface) override;

  void didBecomeActive(interface::Interface& interface) override;

  void didEnterBackground(interface::Interface& interface) override;

  void didTerminate(interface::Interface& interface) override;

  void didBecomeInactive(interface::Interface& interface) override;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceBuilderInterface);
};

}  // namespace ib
}  // namespace rl
