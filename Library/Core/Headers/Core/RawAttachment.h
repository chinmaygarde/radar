// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_RAWATTACHMENT_H_
#define RADAR_CORE_RAWATTACHMENT_H_

#include <Core/Macros.h>
#include <Core/Attachment.h>

namespace rl {
namespace core {

class RawAttachment : public Attachment {
 public:
  RawAttachment();

  RawAttachment(Handle handle);

  RawAttachment(RawAttachment&&);

  ~RawAttachment();

  bool isValid() const override;

  RL_WARN_UNUSED_RESULT
  Handle takeAttachmentHandle() override;

  Handle attachmentHandle() const override;

 private:
  Attachment::Handle _handle;

  friend class Message;

  void setHandle(Handle _handle);

  RL_DISALLOW_COPY_AND_ASSIGN(RawAttachment);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_RAWATTACHMENT_H_
