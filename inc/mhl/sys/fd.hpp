#pragma once
#include <unistd.h>

namespace mhl::sys
{
    class fd
    {
    public:
      fd(const fd &) = delete;
      fd(fd &&) = default;
      fd &operator=(const fd &) = delete;
      fd &operator=(fd &&) = default;
      inline explicit fd(const int file_descriptor) : _fd{file_descriptor} {}

      virtual ~fd()
      {
          if (_fd != -1)
              ::close(_fd);
      }

      [[nodiscard]] inline int get() const { return _fd; }
    private:
        int _fd;
    };
}
