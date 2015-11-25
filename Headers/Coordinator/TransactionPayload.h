// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_
#define RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_

#include <Core/Core.h>
#include <Interface/Action.h>
#include <Coordinator/TransferEntity.h>
#include <Coordinator/TransferRecord.h>
#include <Recognition/GestureRecognizer.h>
#include <Layout/Constraint.h>

#include <map>

namespace rl {
namespace coordinator {

class TransactionPayload : public core::Serializable {
 public:
  using EntityMap =
      std::map<interface::Entity::Identifier, std::unique_ptr<TransferEntity>>;

  using ActionCallback = std::function<void(interface::Action&)>;
  using TransferRecordCallback = std::function<
      void(interface::Action&, TransferRecord&, const core::ClockPoint&)>;
  using RecognizerCallback =
      std::function<void(recognition::GestureRecognizer::Collection&&)>;
  using ConstraintsCallback =
      std::function<void(std::vector<layout::Constraint>&&)>;

  explicit TransactionPayload(
      interface::Action&& action,
      EntityMap&& entities,
      recognition::GestureRecognizer::Collection&& recognizers,
      std::vector<layout::Constraint>&& constraints);

  explicit TransactionPayload(const core::ClockPoint& commitTime,
                              ActionCallback actionCallback,
                              TransferRecordCallback transferRecordCallback,
                              RecognizerCallback recognizerCallback,
                              ConstraintsCallback constraintsCallback);

  bool serialize(core::Message& message) const override;
  bool deserialize(core::Message& message) override;

 private:
  /*
   *  Used when writing
   */
  interface::Action _action;
  EntityMap _entities;
  recognition::GestureRecognizer::Collection _recognizers;
  std::vector<layout::Constraint> _constraints;

  /*
   *  Used when reading
   */
  core::ClockPoint _commitTime;
  ActionCallback _actionCallback;
  TransferRecordCallback _transferRecordCallback;
  RecognizerCallback _recognizerCallback;
  ConstraintsCallback _constraintsCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(TransactionPayload);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_
