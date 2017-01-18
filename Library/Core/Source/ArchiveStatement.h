// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_ARCHIVESTATEMENT_H_
#define RADAR_CORE_ARCHIVESTATEMENT_H_

#include <Core/Allocation.h>
#include <Core/Macros.h>

namespace rl {
namespace core {

class ArchiveStatement {
 public:
  ~ArchiveStatement();

  ArchiveStatement(ArchiveStatement&& message);

  bool isReady() const;

  bool reset();

  bool bind(size_t index, const std::string& item);

  template <class T, class = only_if<std::is_integral<T>::value>>
  bool bind(size_t index, T item) {
    return bindIntegral(index, static_cast<int64_t>(item));
  }

  bool bind(size_t index, double item);

  bool bind(size_t index, const Allocation& item);

  template <class T, class = only_if<std::is_integral<T>::value>>
  bool column(size_t index, T& item) {
    return columnIntegral(index, item);
  }

  bool column(size_t index, double& item);

  bool column(size_t index, std::string& item);

  bool column(size_t index, Allocation& item);

  size_t columnCount();

  enum class Result {
    Done,
    Row,
    Failure,
  };

  Result run();

 private:
  void* _statement;
  bool _ready;

  friend class ArchiveDatabase;

  ArchiveStatement(void* db, const std::string& statememt);
  bool bindIntegral(size_t index, int64_t item);
  bool columnIntegral(size_t index, int64_t& item);

  RL_DISALLOW_COPY_AND_ASSIGN(ArchiveStatement);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_ARCHIVESTATEMENT_H_
