// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_
#define RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_

#include <Coordinator/TransferEntity.h>
#include <Coordinator/TransferRecord.h>
#include <Core/Core.h>
#include <Interface/Action.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>

#include <map>

namespace rl {
namespace coordinator {

class TransactionPayload : public core::Serializable {
 public:
  using EntityMap = std::map<interface::Identifier,
                             std::unique_ptr<TransferEntity>,
                             interface::Identifier::Compare>;

  using ActionCallback = std::function<void(interface::Action&)>;
  using TransferRecordCallback = std::function<
      void(interface::Action&, TransferRecord&, const core::ClockPoint&)>;
  using ConstraintsCallback =
      std::function<void(std::vector<layout::Constraint>&&)>;
  using SuggestionsCallback =
      std::function<void(std::vector<layout::Suggestion>&&)>;

  explicit TransactionPayload(interface::Action&& action,
                              EntityMap&& entities,
                              std::vector<layout::Constraint>&& constraints,
                              std::vector<layout::Suggestion>&& suggestions);

  explicit TransactionPayload(interface::Identifier::LocalID localID,
                              const core::ClockPoint& commitTime,
                              ActionCallback actionCallback,
                              TransferRecordCallback transferRecordCallback,
                              ConstraintsCallback constraintsCallback,
                              SuggestionsCallback suggestionsCallback);

  bool serialize(core::Message& message) const override;
  bool deserialize(core::Message& message) override;

 private:
  /*
   *  Used when writing
   */
  interface::Action _action;
  EntityMap _entities;
  std::vector<layout::Constraint> _constraints;
  std::vector<layout::Suggestion> _suggestions;

  /*
   *  Used when reading
   */
  interface::Identifier::LocalID _localID;
  core::ClockPoint _commitTime;
  ActionCallback _actionCallback;
  TransferRecordCallback _transferRecordCallback;
  ConstraintsCallback _constraintsCallback;
  SuggestionsCallback _suggestionsCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(TransactionPayload);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_
