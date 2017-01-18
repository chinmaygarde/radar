// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_INTERFACEBUILDERINTERFACE_H_
#define RADAR_INTERFACEBUILDER_INTERFACEBUILDERINTERFACE_H_

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

#endif  // RADAR_INTERFACEBUILDER_INTERFACEBUILDERINTERFACE_H_
