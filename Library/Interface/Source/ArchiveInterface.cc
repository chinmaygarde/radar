// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransactionPayload.h>
#include <Interface/ArchiveInterface.h>

namespace rl {
namespace interface {

ArchiveInterface::ArchiveInterface(std::shared_ptr<InterfaceDelegate> delegate,
                                   std::unique_ptr<core::Archive>&& archive)
    : Interface(delegate, nullptr /* splice archive */),
      _archive(std::move(archive)) {
  RL_ASSERT_MSG(_archive && _archive->isReady(), "The archive must be ready");
}

void ArchiveInterface::didBecomeReady() {
  _archive->unarchive<coordinator::TransactionPayload>(
      [&](core::ArchiveItem& item) {
        coordinator::TransactionPayload payload;
        if (payload.deserialize(item, &ns())) {
          transaction().mark(std::move(payload));
          return true;
        } else {
          return false;
        }
      });
}

}  // namespace interface
}  // namespace rl
