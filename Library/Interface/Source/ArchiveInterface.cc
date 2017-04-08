/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

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
