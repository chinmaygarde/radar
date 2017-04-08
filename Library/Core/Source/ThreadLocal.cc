/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/ThreadLocal.h>

namespace rl {
namespace core {

#if RL_THREAD_LOCAL_PTHREADS

void ThreadLocal::ThreadLocalDestroy(Box* value) {
  RL_ASSERT(value != nullptr);
  value->destroyValue();
  delete value;
}

#endif

}  // namespace core
}  // namespace rl
