// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__

#include "Core/Channel.h"

namespace rl {
class MachPortChannel : public Channel {
 public:
  MachPortChannel(const std::string& name);

  static ConnectedPair CreateConnectedPair();

  virtual ~MachPortChannel() override;

  virtual std::shared_ptr<LooperSource> source() override;

  virtual Message createMessage(size_t reserved) const override;

 private:
  using Handle = uint32_t;

  Handle _handle;
  std::shared_ptr<LooperSource> _source;

  virtual Result WriteMessage(Message& message) override;
  virtual ReadResult ReadMessages() override;
  virtual bool doConnect(const std::string& endpoint) override;
  virtual bool doTerminate() override;

  DISALLOW_COPY_AND_ASSIGN(MachPortChannel);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__) */
