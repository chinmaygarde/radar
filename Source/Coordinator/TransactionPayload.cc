// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TransactionPayload.h>
#include <Coordinator/PresentationEntity.h>

namespace rl {
namespace coordinator {

TransactionPayload::TransactionPayload(
    interface::Action&& action,
    EntityMap&& entities,
    recognition::GestureRecognizer::Collection&& recognizers)
    : _action(std::move(action)),
      _entities(std::move(entities)),
      _recognizers(std::move(recognizers)) {
}

TransactionPayload::TransactionPayload(
    const core::ClockPoint& commitTime,
    ActionCallback actionCallback,
    TransferRecordCallback transferRecordCallback,
    RecognizerCallback recognizerCallback)
    : _commitTime(commitTime),
      _actionCallback(actionCallback),
      _transferRecordCallback(transferRecordCallback),
      _recognizerCallback(recognizerCallback) {
}

bool TransactionPayload::serialize(core::Message& message) const {
  bool result = true;

  /*
   *  Step 1: Encode the Action
   */
  result &= message.encode(_action);

  result &= message.encode(_recognizers);

  /*
   *  Step 2: Encode the transfer record count
   *          This is a bit weird since we dont know how many records the entity
   *          will decide to encode. So we just allocate space for the count and
   *          come back to it later.
   */
  auto countOffset = message.encodeOffsetRawUnsafe(sizeof(size_t));
  result &= (countOffset != std::numeric_limits<size_t>::max());

  if (!result) {
    return false;
  }

  /*
   *  Step 3: Encode the transfer records
   */
  size_t transferRecordsEncoded = 0;
  for (const auto& pair : _entities) {
    transferRecordsEncoded += (*(pair.second)).serialize(message);
  }

  /*
   *  Step 2.1: See step 2
   */
  if (auto count = reinterpret_cast<size_t*>(message[countOffset])) {
    memcpy(count, &transferRecordsEncoded, sizeof(size_t));
  } else {
    return false;
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

  recognition::GestureRecognizer::Collection recognizers;
  result = message.decode(recognizers);

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
  for (size_t i = 0; i < transferRecords; i++) {
    _transferRecordCallback(action, TransferRecord::NextInMessage(message),
                            _commitTime);
  }

  if (recognizers.size() > 0) {
    _recognizerCallback(std::move(recognizers));
  }

  return true;
}

}  // namespace coordinator
}  // namespace rl
