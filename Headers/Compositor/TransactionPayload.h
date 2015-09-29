// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H_
#define RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H_

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

  using ActionCallback = std::function<void(Action&)>;
  using TransferRecordCallback =
      std::function<void(Action&, TransferRecord&, const ClockPoint&)>;
  using RecognizerCallback =
      std::function<void(GestureRecognizer::Collection&&)>;

  explicit TransactionPayload(Action&& action,
                              EntityMap&& entities,
                              GestureRecognizer::Collection&& recognizers);

  explicit TransactionPayload(const ClockPoint& commitTime,
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
  GestureRecognizer::Collection _recognizers;

  /*
   *  Used when reading
   */
  ClockPoint _commitTime;
  ActionCallback _actionCallback;
  TransferRecordCallback _transferRecordCallback;
  RecognizerCallback _recognizerCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(TransactionPayload);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H_
