#ifndef SLIB_COMPILER_H
#define SLIB_COMPILER_H 1

#ifndef __has_feature
  #define __has_feature(x) 0
#endif
#ifndef __has_extension
  #define __has_extension(x) 0
#endif

#if __GNUC__ && !__CHECKER__
#define PRINTF_FORMAT(FMT, ARG1) __attribute__((__format__(printf, FMT, ARG1)))
#else
#define PRINTF_FORMAT(FMT, ARG1)
#endif

#endif /* compiler.h */