// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
#define RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_

#include <Core/Macros.h>
#include <Core/FileHandle.h>
#include <Entity/Entity.h>
#include <memory>

namespace rl {
namespace ib {

class InterfaceBuilderArchive {
 public:
  static std::unique_ptr<InterfaceBuilderArchive> Make(
      const core::FileHandle& handle);

  virtual ~InterfaceBuilderArchive();

  bool isValid() const;

  std::unique_ptr<entity::Entity> inflate() const;

 protected:
  InterfaceBuilderArchive();

  virtual bool isArchiveReadable() const = 0;

  virtual std::unique_ptr<entity::Entity> onInflate() const = 0;

 private:
  std::unique_ptr<InterfaceBuilderArchive> _impl;

  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceBuilderArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
