/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_SHMEM == RL_SHMEM_POSIX

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include "SharedMemoryHandle.h"

namespace rl {
namespace core {

static const int SharedMemoryTempHandleMaxRetries = 25;

static std::string SharedMemory_RandomFileName() {
  static uint32_t kSeed = 4;
  auto random = rand_r(&kSeed) % RAND_MAX;

  std::stringstream stream;

  stream << "/rl_SharedMemory" << random;

  return stream.str();
}

SharedMemory::Handle SharedMemoryHandleCreate(size_t size) {
  SharedMemory::Handle newHandle = -1;

  auto tempFile = SharedMemory_RandomFileName();

  int tries = 0;

  while (tries++ < SharedMemoryTempHandleMaxRetries) {
    newHandle = ::shm_open(tempFile.c_str(), O_RDWR | O_CREAT | O_EXCL,
                           S_IRUSR | S_IWUSR | O_CLOEXEC);

    if (newHandle == -1 && errno == EEXIST) {
      /*
       *  The current handle already exists (the O_CREAT | O_EXCL
       *  check is atomic). Try a new file name.
       */
      tempFile = SharedMemory_RandomFileName();
      continue;
    }

    break;
  }

  /*
   *  We already have a file reference, unlink the
   *  reference by name.
   */
  if (newHandle != -1) {
    RL_CHECK(::shm_unlink(tempFile.c_str()));
  }

/*
 *  Set the size of the shared memory
 */
#if RL_OS_MAC || RL_OS_BSD
  if (RL_TEMP_FAILURE_RETRY(::ftruncate(newHandle, size)) == -1) {
    RL_CHECK(::close(newHandle));
    return -1;
  }
#else
  int result = ::posix_fallocate(newHandle, 0, size);
  if (result != 0) {
    /*
     *  `posix_fallocate` does not set errno
     */
    RL_LOG("Could not resize shared memory handle: %s (%d)", strerror(result),
           result);
    RL_CHECK(::close(newHandle));
    return -1;
  }
#endif

  return newHandle;
}

}  // namespace core
}  // namespace rl

#endif  // RL_SHMEM == RL_SHMEM_POSIX
