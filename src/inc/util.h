#ifndef UTIL_H
#define UTIL_H 1

#include <stddef.h>

#include "slib/util.h"
#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MIN
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef MAX
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#endif

void *xmalloc(size_t) MALLOC_LIKE;
void *xcalloc(size_t, size_t) MALLOC_LIKE;
void *xzalloc(size_t) MALLOC_LIKE;
void *xrealloc(void *, size_t);
void *x2nrealloc(void *p, size_t *n, size_t s);

#ifdef __cplusplus
}
#endif

#endif /* util.h */