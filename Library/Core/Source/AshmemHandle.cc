/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_SHMEM == RL_SHMEM_ASHMEM

#include <fcntl.h>
#include <linux/ashmem.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "SharedMemoryHandle.h"

namespace rl {
namespace core {

#define ASHMEM_DEVICE "/dev/ashmem"

/**
*  Creates a new ashmem region and returns the file descriptor, or < 0 on error
*
*  @param name an optional label to give the region (visible in /proc/pid/maps)
*  @param size size of the region, in page-aligned bytes
*
*  @return file descriptor on success, < 0 on error
*/
int AshmemCreateRegion(const char* name, size_t size) {
  int fd = 0;
  int ret = 0;

  fd = open(ASHMEM_DEVICE, O_RDWR);
  if (fd < 0) {
    return fd;
  }

  if (name) {
    char buf[ASHMEM_NAME_LEN] = {0};
    strlcpy(buf, name, sizeof(buf));

    ret = ioctl(fd, ASHMEM_SET_NAME, buf);

    if (ret < 0) {
      goto error;
    }
  }

  ret = ioctl(fd, ASHMEM_SET_SIZE, size);

  if (ret < 0) {
    goto error;
  }

  return fd;

error:
  RL_CHECK(::close(fd));
  return ret;
}

int AshmemSetProtRegion(int fd, int prot) {
  return ioctl(fd, ASHMEM_SET_PROT_MASK, prot);
}

int AshmemPinRegion(int fd, size_t offset, size_t len) {
  struct ashmem_pin pin = {offset, len};
  return ioctl(fd, ASHMEM_PIN, &pin);
}

int AshmemUnpinRegion(int fd, size_t offset, size_t len) {
  struct ashmem_pin pin = {offset, len};
  return ioctl(fd, ASHMEM_UNPIN, &pin);
}

int AshmemGetSizeRegion(int fd) {
  return ioctl(fd, ASHMEM_GET_SIZE, NULL);
}

SharedMemory::Handle SharedMemoryHandleCreate(size_t size) {
  auto res = AshmemCreateRegion("radarlove", size);
  return res < 0 ? -1 : res;
}

}  // namespace core
}  // namespace rl

#endif  // RL_SHMEM == RL_SHMEM_ASHMEM
