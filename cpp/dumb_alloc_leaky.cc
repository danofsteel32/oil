// dumb_alloc.cc: Test this C++ mechanism as a lower bound on performance.

#include "dumb_alloc_leaky.h"

#include <stdio.h>

// 400 MiB of memory
char kMem[400 << 20];

int gMemPos = 0;
int gNumNew = 0;
int gNumDelete = 0;

// I'm not sure why this matters but we get crashes when aligning to 8 bytes.
// That is annoying.
// Example: we get a crash in cpp/frontend_flag_spec.cc
// auto out = new flag_spec::_FlagSpecAndMore();
//
// https://stackoverflow.com/questions/52531695/int128-alignment-segment-fault-with-gcc-o-sse-optimize
constexpr int kMask = alignof(max_align_t) - 1;  // e.g. 15 or 7

// Align returned pointers to the worst case of 8 bytes (64-bit pointers)
inline size_t aligned(size_t n) {
  // https://stackoverflow.com/questions/2022179/c-quick-calculation-of-next-multiple-of-4
  // return (n + 7) & ~7;

  return (n + kMask) & ~kMask;
}

// This global interface is silly ...

#ifdef DUMB_ALLOC
void* operator new(size_t size) {
  char* p = &(kMem[gMemPos]);
#ifdef ALLOC_LOG
  printf("new %zu\n", size);
#endif
  gMemPos += aligned(size);
  ++gNumNew;
  return p;
}

// noexcept fixes Clang warning
void operator delete(void* p) noexcept {
  // fprintf(stderr, "\tdelete %p\n", p);
  ++gNumDelete;
}
#endif

char kMem2[400 << 20];
int gMemPos2 = 0;
int gNumMalloc = 0;
int gNumFree = 0;

void* dumb_malloc(size_t size) noexcept {
  char* p = &(kMem2[gMemPos2]);
#ifdef ALLOC_LOG
  printf("malloc %zu\n", size);
#endif
  gMemPos2 += aligned(size);
  ++gNumMalloc;
  return p;
}

void dumb_free(void* p) noexcept {
  // fprintf(stderr, "free\n");
  ++gNumFree;
}

namespace dumb_alloc {

void Summarize() {
#ifdef DUMB_ALLOC_VERBOSE
  fprintf(stderr, "\n");
  fprintf(stderr, "dumb_alloc:\n");
  fprintf(stderr, "\tgNumNew = %d\n", gNumNew);
  fprintf(stderr, "\tgNumDelete = %d\n", gNumDelete);
  fprintf(stderr, "\tgMemPos = %d\n", gMemPos);
  fprintf(stderr, "\n");
  fprintf(stderr, "\tgNumMalloc = %d\n", gNumMalloc);
  fprintf(stderr, "\tgNumFree = %d\n", gNumFree);
  fprintf(stderr, "\tgMemPos2 = %d\n", gMemPos2);
#endif
}

}  // namespace dumb_alloc
