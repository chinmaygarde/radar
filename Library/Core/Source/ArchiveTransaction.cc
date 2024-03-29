/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ArchiveTransaction.h"
#include "ArchiveStatement.h"

namespace rl {
namespace core {

ArchiveTransaction::ArchiveTransaction(int64_t& transactionCount,
                                       ArchiveStatement& beginStatement,
                                       ArchiveStatement& endStatement,
                                       ArchiveStatement& rollbackStatement)
    : _endStatement(endStatement),
      _rollbackStatement(rollbackStatement),
      _transactionCount(transactionCount),
      _cleanup(false),
      _successful(false),
      _abandoned(false) {
  if (_transactionCount == 0) {
    _cleanup = beginStatement.run() == ArchiveStatement::Result::Done;
  }
  _transactionCount++;
}

ArchiveTransaction::ArchiveTransaction(ArchiveTransaction&& other)
    : _endStatement(other._endStatement),
      _rollbackStatement(other._rollbackStatement),
      _transactionCount(other._transactionCount),
      _cleanup(other._cleanup),
      _successful(other._successful),
      _abandoned(false) {
  other._abandoned = true;
}

ArchiveTransaction::~ArchiveTransaction() {
  if (_abandoned) {
    return;
  }

  RL_ASSERT(_transactionCount != 0);
  if (_transactionCount == 1 && _cleanup) {
    auto res = _successful ? _endStatement.run() : _rollbackStatement.run();
    RL_ASSERT_MSG(res == ArchiveStatement::Result::Done,
                  "Must be able to commit the nested transaction");
  }
  _transactionCount--;
}

void ArchiveTransaction::markWritesSuccessful() {
  _successful = true;
}

}  // namespace core
}  // namespace rl
