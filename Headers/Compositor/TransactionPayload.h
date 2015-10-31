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

class TransactionPayload : public core::Serializable {
 public:
  using EntityMap =
      std::map<interface::Entity::Identifier, std::unique_ptr<TransferEntity>>;

  using ActionCallback = std::function<void(interface::Action&)>;
  using TransferRecordCallback = std::function<
      void(interface::Action&, TransferRecord&, const core::ClockPoint&)>;
  using RecognizerCallback =
      std::function<void(recognition::GestureRecognizer::Collection&&)>;

  explicit TransactionPayload(
      interface::Action&& action,
      EntityMap&& entities,
      recognition::GestureRecognizer::Collection&& recognizers);

  explicit TransactionPayload(const core::ClockPoint& commitTime,
                              ActionCallback actionCallback,
                              TransferRecordCallback transferRecordCallback,
                              RecognizerCallback recognizerCallback);

  bool serialize(core::Message& message) const override;
  bool deserialize(core::Message& message) override;

 private:
  /*
   *  Used when writing
   */
  interface::Action _action;
  EntityMap _entities;
  recognition::GestureRecognizer::Collection _recognizers;

  /*
   *  Used when reading
   */
  core::ClockPoint _commitTime;
  ActionCallback _actionCallback;
  TransferRecordCallback _transferRecordCallback;
  RecognizerCallback _recognizerCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(TransactionPayload);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_TRANSACTIONPAYLOAD_H_
