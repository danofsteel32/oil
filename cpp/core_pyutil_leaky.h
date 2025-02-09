// core_pyutil_leaky.h

#ifndef CORE_PYUTIL_H
#define CORE_PYUTIL_H

#include "mycpp/mylib_leaky.h"

class _OSError;  // declaration from mycpp/myerror.h

namespace pyutil {

bool IsValidCharEscape(int c);
Str* ChArrayToString(List<int>* ch_array);

class _ResourceLoader {
 public:
  virtual Str* Get(Str* path);
};

_ResourceLoader* GetResourceLoader();

void CopyFile(Str* in_path, Str* out_path);

Str* GetVersion(_ResourceLoader* loader);

Str* ShowAppVersion(Str* app_name, _ResourceLoader* loader);

Str* strerror(_OSError* e);

inline Str* BackslashEscape(Str* s, Str* meta_chars) {
  int upper_bound = s->len_ * 2;
  char* buf = static_cast<char*>(malloc(upper_bound));
  char* p = buf;

  for (int i = 0; i < s->len_; ++i) {
    char c = s->data_[i];
    if (memchr(meta_chars->data_, c, meta_chars->len_)) {
      *p++ = '\\';
    }
    *p++ = c;
  }
  int len = p - buf;
  return new Str(buf, len);
}

}  // namespace pyutil

#endif  // CORE_PYUTIL_H
