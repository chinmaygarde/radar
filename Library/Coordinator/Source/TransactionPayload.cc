// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/PresentationEntity.h>
#include <Coordinator/TransactionPayload.h>
#include <Core/TraceEvent.h>

namespace rl {
namespace coordinator {

TransactionPayload::TransactionPayload() {}

TransactionPayload::TransactionPayload(TransactionPayload&& payload) = default;

TransactionPayload::TransactionPayload(
    animation::Action&& action,
    EntityMap&& entities,
    std::vector<layout::Constraint>&& constraints,
    std::vector<layout::Suggestion>&& suggestions)
    : _action(std::move(action)),
      _entities(std::move(entities)),
      _constraints(std::move(constraints)),
      _suggestions(std::move(suggestions)) {}

TransactionPayload::TransactionPayload(
    const core::ClockPoint& commitTime,
    ActionCallback actionCallback,
    TransferRecordCallback transferRecordCallback,
    ConstraintsCallback constraintsCallback,
    SuggestionsCallback suggestionsCallback)
    : _commitTime(commitTime),
      _actionCallback(actionCallback),
      _transferRecordCallback(transferRecordCallback),
      _constraintsCallback(constraintsCallback),
      _suggestionsCallback(suggestionsCallback) {}

bool TransactionPayload::serialize(core::Message& message) const {
  /*
   *  Step 1: Encode the Action, Constraints and Suggestions
   */
  RL_RETURN_IF_FALSE(message.encode(_action));

  RL_RETURN_IF_FALSE(message.encode(_constraints));

  RL_RETURN_IF_FALSE(message.encode(_suggestions));

  /*
   *  Step 2: Encode the transfer record count
   */
  RL_RETURN_IF_FALSE(message.encode(_entities.size()));

  /*
   *  Step 3: Encode the transfer records
   */
  for (const auto& pair : _entities) {
    RL_RETURN_IF_FALSE((*(pair.second)).serialize(message));
  }

  return true;
}

bool TransactionPayload::deserialize(core::Message& message,
                                     core::Namespace* ns) {
  /*
   *  Step 1: Read the action.
   */
  animation::Action action;
  RL_RETURN_IF_FALSE(action.deserialize(message, ns));
  _actionCallback(action);

  /*
   *  Step 2: Read the constraint set.
   */
  std::vector<layout::Constraint> constraints;
  RL_RETURN_IF_FALSE(message.decode(constraints, ns));

  /*
   *  Step 3: Read constraint suggestions.
   */
  std::vector<layout::Suggestion> suggestions;
  RL_RETURN_IF_FALSE(message.decode(suggestions, ns));

  /*
   *  Step 4.1: Read the transfer record count
   */
  size_t transferRecords = 0;
  RL_RETURN_IF_FALSE(message.decode(transferRecords, ns));

  /*
   *  Step 4.2: Read the transfer records
   */
  {
    RL_TRACE_AUTO("TransferRecordsCommit");

    for (size_t i = 0; i < transferRecords; i++) {
      TransferEntity entity;
      RL_RETURN_IF_FALSE(message.decode(entity, ns));
      _transferRecordCallback(action, entity, _commitTime);
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

enum TransactionArchiveKey {
  Action,
  Constraints,
  Suggestions,
  Entities,
};

const core::ArchiveDef TransactionPayload::ArchiveDefinition = {
    /* .superClass = */ nullptr,
    /* .className = */ "TransactionPayload",
    /* .autoAssignName = */ true,
    /* .members = */ {
        TransactionArchiveKey::Action,       //
        TransactionArchiveKey::Constraints,  //
        TransactionArchiveKey::Suggestions,  //
        TransactionArchiveKey::Entities,     //
    }};

TransactionPayload::ArchiveName TransactionPayload::archiveName() const {
  return core::ArchiveNameAuto;
}

bool TransactionPayload::serialize(core::ArchiveItem& item) const {
  RL_RETURN_IF_FALSE(
      item.encodeArchivable(TransactionArchiveKey::Action, _action));
  RL_RETURN_IF_FALSE(
      item.encode(TransactionArchiveKey::Constraints, _constraints));
  RL_RETURN_IF_FALSE(
      item.encode(TransactionArchiveKey::Suggestions, _suggestions));

  std::vector<TransferEntity> entities;
  for (const auto& entity : _entities) {
    entities.push_back(*(entity.second));
  }
  RL_RETURN_IF_FALSE(item.encode(TransactionArchiveKey::Entities, entities));

  return true;
}

bool TransactionPayload::deserialize(core::ArchiveItem& item,
                                     core::Namespace* ns) {
  RL_RETURN_IF_FALSE(
      item.decodeArchivable(TransactionArchiveKey::Action, _action, ns));
  RL_RETURN_IF_FALSE(
      item.decode(TransactionArchiveKey::Constraints, _constraints, ns));
  RL_RETURN_IF_FALSE(
      item.decode(TransactionArchiveKey::Suggestions, _suggestions, ns));
  std::vector<TransferEntity> entities;
  RL_RETURN_IF_FALSE(item.decode(TransactionArchiveKey::Entities, entities, ns))

  for (auto& entity : entities) {
    _entities.emplace(entity.identifier(),
                      core::make_unique<TransferEntity>(std::move(entity)));
  }

  return true;
}

}  // namespace coordinator
}  // namespace rl
