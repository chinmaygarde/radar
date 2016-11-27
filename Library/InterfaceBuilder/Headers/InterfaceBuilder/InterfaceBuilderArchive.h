// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
#define RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_

#include <Core/Macros.h>
#include <Core/FileHandle.h>
#include <Interface/Interface.h>
#include <memory>

namespace rl {
namespace ib {

class InterfaceBuilderArchive {
 public:
  static std::unique_ptr<InterfaceBuilderArchive> Make(
      const core::FileHandle& handle);

  virtual ~InterfaceBuilderArchive();

  virtual bool isValid() const = 0;

  virtual bool inflate(interface::Interface& interface) const = 0;

 protected:
  InterfaceBuilderArchive();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceBuilderArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
