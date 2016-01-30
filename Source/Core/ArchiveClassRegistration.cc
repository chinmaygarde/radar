// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/ArchiveClassRegistration.h>

namespace rl {
namespace core {

ArchiveClassRegistration::ArchiveClassRegistration(ArchiveDef definition)
    : _tableName(definition.className), _columnCount(0) {
  /*
   *  Each class in the archive class hierarchy is assigned an entry in the
   *  class map.
   */
  const ArchiveDef* current = &definition;
  size_t currentMember = 1;
  while (current != nullptr) {
    auto membersInCurrent = current->members.size();
    _columnCount += membersInCurrent;
    MemberColumnMap map;
    for (const auto& member : current->members) {
      map[member] = currentMember++;
    }
    _classMap[current->className] = map;
    current = current->superClass;
  }
}

const std::string& ArchiveClassRegistration::tableName() const {
  return _tableName;
}

size_t ArchiveClassRegistration::columnCount() const {
  return _columnCount;
}

ArchiveClassRegistration::ColumnResult ArchiveClassRegistration::findColumn(
    const std::string& className,
    ArchiveSerializable::Member member) const {
  auto found = _classMap.find(className);

  if (found == _classMap.end()) {
    return {0, false};
  }

  const auto& memberToColumns = found->second;

  auto foundColumn = memberToColumns.find(member);

  if (foundColumn == memberToColumns.end()) {
    return {0, false};
  }

  return {foundColumn->second, true};
}

}  // namespace core
}  // namespace rl
