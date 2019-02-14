/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Attachment.h>
#include <Core/Macros.h>
#include <functional>

namespace rl {
namespace core {

class RawAttachment final : public Attachment {
 public:
  using Collector = std::function<void(Attachment::Handle handle)>;

  RawAttachment();

  RawAttachment(Handle handle, Collector collector);

  RawAttachment(RawAttachment&& other);

  RawAttachment& operator=(RawAttachment&& other);

  ~RawAttachment();

  Handle handle() const override;

  RL_WARN_UNUSED_RESULT
  Handle takeHandle();

 private:
  Attachment::Handle _handle;
  Collector _collector;

  RL_DISALLOW_COPY_AND_ASSIGN(RawAttachment);
};

}  // namespace core
}  // namespace rl
