// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/TransactionPayload.h>
#include <Compositor/PresentationEntity.h>

namespace rl {

TransactionPayload::TransactionPayload(Action&& action,
                                       EntityMap&& entities,
                                       RecognizerCollection&& recognizers)
    : _action(std::move(action)),
      _entities(std::move(entities)),
      _recognizers(std::move(recognizers)) {
}

TransactionPayload::TransactionPayload(
    const std::chrono::nanoseconds& commitTime,
    ActionCallback actionCallback,
    TransferRecordCallback transferRecordCallback,
    RecognizerCallback recognizerCallback)
    : _commitTime(commitTime),
      _actionCallback(actionCallback),
      _transferRecordCallback(transferRecordCallback),
      _recognizerCallback(recognizerCallback) {
}

bool TransactionPayload::serialize(Message& message) const {
  bool result = true;

  /*
   *  Step 1: Encode the Action
   */
  result &= _action.serialize(message);

  /*
   *  Step 2: Encode the transfer record count
   *          This is a bit weird since we dont know how many records the entity
   *          will decide to encode. So we just allocate space for the count and
   *          come back to it later.
   */
  auto countOffset = message.encodeRawOffsetUnsafe(sizeof(size_t));
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

bool TransactionPayload::deserialize(Message& message) {
  bool result = true;
  /*
   *  Step 1: Read the action
   */
  Action action;
  result = action.deserialize(message);

  if (!result) {
    return false;
  }

  _actionCallback(action);

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
  for (auto i = 0; i < transferRecords; i++) {
    _transferRecordCallback(action, TransferRecord::NextInMessage(message),
                            _commitTime);
  }

  return true;
}

}  // namespace rl
