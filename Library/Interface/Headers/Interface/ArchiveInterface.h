/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
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
