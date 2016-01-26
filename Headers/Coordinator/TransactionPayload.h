// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_
#define RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_

#include <Coordinator/TransferEntity.h>
#include <Core/Core.h>
#include <Interface/Action.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>

#include <map>

namespace rl {
namespace coordinator {

class TransactionPayload : public core::ArchiveSerializable,
                           public core::MessageSerializable {
 public:
  using EntityMap = std::map<core::Name, std::unique_ptr<TransferEntity>>;

  using ActionCallback = std::function<void(interface::Action&)>;
  using TransferRecordCallback = std::function<
      void(interface::Action&, TransferEntity&, const core::ClockPoint&)>;
  using ConstraintsCallback =
      std::function<void(std::vector<layout::Constraint>&&)>;
  using SuggestionsCallback =
      std::function<void(std::vector<layout::Suggestion>&&)>;

  explicit TransactionPayload(interface::Action&& action,
                              EntityMap&& entities,
                              std::vector<layout::Constraint>&& constraints,
                              std::vector<layout::Suggestion>&& suggestions);

  explicit TransactionPayload(const core::ClockPoint& commitTime,
                              ActionCallback actionCallback,
                              TransferRecordCallback transferRecordCallback,
                              ConstraintsCallback constraintsCallback,
                              SuggestionsCallback suggestionsCallback);

  /*
   *  Message Serialization
   */
  bool serialize(core::Message& message) const override;
  bool deserialize(core::Message& message, core::Namespace* ns) override;

  /*
   *  Archive Serialization
   */
  ArchiveName archiveName() const override;
  bool serialize(core::ArchiveItem& item) const override;
  bool deserialize(core::ArchiveItem& item) override;

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
  core::ClockPoint _commitTime;
  ActionCallback _actionCallback;
  TransferRecordCallback _transferRecordCallback;
  ConstraintsCallback _constraintsCallback;
  SuggestionsCallback _suggestionsCallback;

  RL_DISALLOW_COPY_AND_ASSIGN(TransactionPayload);
};

}  // namespace coordinator
}  // namespace rl

enum TransactionArchiveKey {
  Action,
  Constraints,
  Suggestions,
  Entities,
};

template <>
class rl::core::ArchiveDef<rl::coordinator::TransactionPayload> {
 public:
  std::string className() const { return "TransactionPayload"; }

  ArchiveSerializable::Members members() const {
    return {
        TransactionArchiveKey::Action,       //
        TransactionArchiveKey::Constraints,  //
        TransactionArchiveKey::Suggestions,  //
        TransactionArchiveKey::Entities,     //
    };
  }
};

#endif  // RADARLOVE_COORDINATOR_TRANSACTIONPAYLOAD_H_
