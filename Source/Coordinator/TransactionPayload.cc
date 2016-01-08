// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/PresentationEntity.h>
#include <Coordinator/TransactionPayload.h>
#include <Instrumentation/TraceEvent.h>

namespace rl {
namespace coordinator {

TransactionPayload::TransactionPayload(
    interface::Action&& action,
    EntityMap&& entities,
    std::vector<layout::Constraint>&& constraints,
    std::vector<layout::Suggestion>&& suggestions)
    : _localNS(nullptr),
      _action(std::move(action)),
      _entities(std::move(entities)),
      _constraints(std::move(constraints)),
      _suggestions(std::move(suggestions)) {}

TransactionPayload::TransactionPayload(
    core::Namespace& localNS,
    const core::ClockPoint& commitTime,
    ActionCallback actionCallback,
    TransferRecordCallback transferRecordCallback,
    ConstraintsCallback constraintsCallback,
    SuggestionsCallback suggestionsCallback)
    : _localNS(&localNS),
      _commitTime(commitTime),
      _actionCallback(actionCallback),
      _transferRecordCallback(transferRecordCallback),
      _constraintsCallback(constraintsCallback),
      _suggestionsCallback(suggestionsCallback) {}

bool TransactionPayload::serialize(core::Message& message) const {
  bool result = true;

  /*
   *  Step 1: Encode the Action, Constraints and Suggestions
   */
  result &= message.encode(_action);

  result &= message.encode(_constraints);

  result &= message.encode(_suggestions);

  /*
   *  Step 2: Encode the transfer record count
   */
  result &= message.encode(_entities.size());

  /*
   *  Step 3: Encode the transfer records
   */
  size_t transferRecordsEncoded = 0;
  for (const auto& pair : _entities) {
    transferRecordsEncoded += (*(pair.second)).serialize(message);
  }

  return result;
}

bool TransactionPayload::deserialize(core::Message& message) {
  bool result = true;
  /*
   *  Step 1: Read the action
   */
  interface::Action action;
  result = action.deserialize(message);

  if (!result) {
    return false;
  }

  _actionCallback(action);

  std::vector<layout::Constraint> constraints;
  result = message.decode(constraints);

  if (!result) {
    return false;
  }

  std::vector<layout::Suggestion> suggestions;
  result = message.decode(suggestions);

  if (!result) {
    return false;
  }

  /*
   *  Step 2: Read the transfer record count
   */
  size_t transferRecords = 0;
  result = message.decode(transferRecords);
  if (!result) {
    return false;
  }

  /*
   *  Step 3: Read the transfer records
   */
  {
    RL_TRACE_AUTO("TransferRecordsCommit");

    TransferEntity entity(core::DeadName);

    for (size_t i = 0; i < transferRecords; i++) {
      if (message.decode(entity)) {
        _transferRecordCallback(action, entity, _commitTime);
      } else {
        break;
      }
    }
  }

  if (constraints.size() > 0) {
    _constraintsCallback(std::move(constraints));
  }

  if (suggestions.size() > 0) {
    _suggestionsCallback(std::move(suggestions));
  }

  return true;
}

}  // namespace coordinator
}  // namespace rl
