// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_ARCHIVEINTERFACE_H_
#define RADARLOVE_INTERFACE_ARCHIVEINTERFACE_H_

#include <Core/Core.h>
#include <Interface/Interface.h>

namespace rl {
namespace interface {

class ArchiveInterface : public Interface {
 public:
  ArchiveInterface(std::shared_ptr<InterfaceDelegate> delegate,
                   std::unique_ptr<core::Archive>&& archive);

 private:
  std::unique_ptr<core::Archive> _archive;

  void didBecomeReady() override;

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveInterface);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_ARCHIVEINTERFACE_H_
