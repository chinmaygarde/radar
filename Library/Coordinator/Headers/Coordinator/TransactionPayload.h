/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Action.h>
#include <Coordinator/TransferEntity.h>
#include <Core/Macros.h>
#include <Layout/Constraint.h>
#include <Layout/Suggestion.h>
#include <map>

namespace rl {
namespace coordinator {

class TransactionPayload final : public core::ArchiveSerializable,
                                 public core::MessageSerializable {
 public:
  using EntityMap = std::map<core::Name, std::unique_ptr<TransferEntity>>;

  using ActionCallback = std::function<void(animation::Action&)>;
  using TransferRecordCallback = std::function<
      void(animation::Action&, TransferEntity&, const core::ClockPoint&)>;
  using ConstraintsCallback =
      std::function<void(std::vector<layout::Constraint>&&)>;
  using SuggestionsCallback =
      std::function<void(std::vector<layout::Suggestion>&&)>;

  TransactionPayload();

  TransactionPayload(TransactionPayload&& payload);

  TransactionPayload(animation::Action&& action,
                     EntityMap&& entities,
                     std::vector<layout::Constraint>&& constraints,
                     std::vector<layout::Suggestion>&& suggestions);

  TransactionPayload(const core::ClockPoint& commitTime,
                     ActionCallback actionCallback,
                     TransferRecordCallback transferRecordCallback,
                     ConstraintsCallback constraintsCallback,
                     SuggestionsCallback suggestionsCallback);

  bool serialize(core::Message& message) const override;

  bool deserialize(core::Message& message, core::Namespace* ns) override;

  static const core::ArchiveDef ArchiveDefinition;

  core::ArchiveSerializable::ArchiveName archiveName() const override;

  bool serialize(core::ArchiveItem& item) const override;

  bool deserialize(core::ArchiveItem& item, core::Namespace* ns) override;

 private:
  /*
   *  Used when writing
   */
  animation::Action _action;
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
