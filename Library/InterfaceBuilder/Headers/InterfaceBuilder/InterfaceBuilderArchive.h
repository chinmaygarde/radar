// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
#define RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_

#include <Core/FileHandle.h>
#include <Core/Macros.h>
#include <Interface/Interface.h>
#include <memory>

namespace rl {
namespace ib {

class InterfaceBuilderArchive {
 public:
  static std::unique_ptr<InterfaceBuilderArchive> Make(
      const core::FileHandle& handle);

  static std::unique_ptr<InterfaceBuilderArchive> Make(
      const core::Allocation& allocation);

  static std::unique_ptr<InterfaceBuilderArchive> Make(const uint8_t* data,
                                                       size_t size);

  virtual ~InterfaceBuilderArchive();

  virtual bool isValid() const = 0;

  using EntityMap =
      std::unordered_map<std::string, interface::ModelEntity::Ref>;

  virtual interface::ModelEntity::Ref inflate(interface::Interface& interface,
                                              EntityMap& map) const = 0;

 protected:
  InterfaceBuilderArchive();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceBuilderArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
