// fcntl_.cc

// clang-format off
#include "mycpp/myerror.h"  // for IOError
// clang-format on

#include "fcntl__leaky.h"

#include <errno.h>
#include <fcntl.h>  // the system header

namespace fcntl_ {

int fcntl(int fd, int cmd) {
  int result = ::fcntl(fd, cmd);
  if (result < 0) {
    throw new IOError(errno);
  }
  return result;
}

int fcntl(int fd, int cmd, int arg) {
  int result = ::fcntl(fd, cmd, arg);
  if (result < 0) {
    throw new IOError(errno);
  }
  return result;
}

}  // namespace fcntl_
