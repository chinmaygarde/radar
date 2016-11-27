// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
#define RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_

#include <Core/Macros.h>

namespace rl {
namespace ib {

class InterfaceBuilderArchive {
 public:
  InterfaceBuilderArchive();

  ~InterfaceBuilderArchive();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InterfaceBuilderArchive);
};

}  // namespace ib
}  // namespace rl

#endif  // RADAR_INTERFACEBUILDER_INTERFACEBUILDERARCHIVE_H_
