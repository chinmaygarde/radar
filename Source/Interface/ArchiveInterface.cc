// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/ArchiveInterface.h>

namespace rl {
namespace interface {

ArchiveInterface::ArchiveInterface(std::weak_ptr<InterfaceDelegate> delegate,
                                   std::unique_ptr<core::Archive>&& archive)
    : Interface(delegate), _archive(std::move(archive)) {
  RL_ASSERT_MSG(archive && archive->isReady(), "The archive must be ready");
}

void ArchiveInterface::didBecomeReady() {}

}  // namespace interface
}  // namespace rl
