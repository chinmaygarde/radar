// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H__
#define __RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H__

#include <Core/Core.h>
#include <Interface/Action.h>
#include <Compositor/TransferEntity.h>
#include <Compositor/TransferRecord.h>
#include <Recognition/GestureRecognizer.h>

#include <map>

namespace rl {

class TransactionPayload : public Serializable {
 public:
  using EntityMap =
      std::map<Entity::Identifier, std::unique_ptr<TransferEntity>>;
  using RecognizerCollection = std::vector<GestureRecognizer>;

  using ActionCallback = std::function<void(Action&)>;
  using TransferRecordCallback = std::function<
      void(Action&, TransferRecord&, const std::chrono::nanoseconds&)>;
  using RecognizerCallback = std::function<void(RecognizerCollection&)>;

  explicit TransactionPayload(Action&& action,
                              EntityMap&& entities,
                              RecognizerCollection&& recognizers);

  explicit TransactionPayload(const std::chrono::nanoseconds& commitTime,
                              ActionCallback actionCallback,
                              TransferRecordCallback transferRecordCallback,
                              RecognizerCallback recognizerCallback);

  bool serialize(Message& message) const override;
  bool deserialize(Message& message) override;

 private:
  /*
   *  Used when writing
   */
  Action _action;
  EntityMap _entities;
  RecognizerCollection _recognizers;

  /*
   *  Used when reading
   */
  std::chrono::nanoseconds _commitTime;
  ActionCallback _actionCallback;
  TransferRecordCallback _transferRecordCallback;
  RecognizerCallback _recognizerCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(TransactionPayload);
};

}  // namespace rl

#endif  // __RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H__
