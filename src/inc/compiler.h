#ifndef COMPILER_H
#define COMPILER_H 1

#include "slib/compiler.h"

#if __GNUC__ && !__CHECKER__
#define MALLOC_LIKE __attribute__((__malloc__))
#else
#define MALLOC_LIKE
#endif

/* Build assertions.
 *
 * Use BUILD_ASSERT_DECL as a declaration or a statement, or BUILD_ASSERT as
 * part of an expression. */
#ifdef __CHECKER__
#define BUILD_ASSERT(EXPR) ((void) 0)
#define BUILD_ASSERT_DECL(EXPR) extern int (*build_assert(void))[1]
#elif defined(__cplusplus) && __cplusplus >= 201103L
#define BUILD_ASSERT(EXPR) static_assert(EXPR, "assertion failed")
#define BUILD_ASSERT_DECL(EXPR) static_assert(EXPR, "assertion failed")
#elif defined(__cplusplus) && __cplusplus < 201103L
#include <boost/static_assert.hpp>
#define BUILD_ASSERT BOOST_STATIC_ASSERT
#define BUILD_ASSERT_DECL BOOST_STATIC_ASSERT
#elif (__GNUC__ * 256 + __GNUC_MINOR__ >= 0x403 \
       || __has_extension(c_static_assert))
#define BUILD_ASSERT_DECL(EXPR) _Static_assert(EXPR, #EXPR)
#define BUILD_ASSERT(EXPR) (void) ({ _Static_assert(EXPR, #EXPR); })
#else
#define BUILD_ASSERT__(EXPR) \
        sizeof(struct { unsigned int build_assert_failed : (EXPR) ? 1 : -1; })
#define BUILD_ASSERT(EXPR) (void) BUILD_ASSERT__(EXPR)
#define BUILD_ASSERT_DECL(EXPR) \
        extern int (*build_assert(void))[BUILD_ASSERT__(EXPR)]
#endif

#ifdef __GNUC__
#define BUILD_ASSERT_GCCONLY(EXPR) BUILD_ASSERT(EXPR)
#define BUILD_ASSERT_DECL_GCCONLY(EXPR) BUILD_ASSERT_DECL(EXPR)
#else
#define BUILD_ASSERT_GCCONLY(EXPR) ((void) 0)
#define BUILD_ASSERT_DECL_GCCONLY(EXPR) ((void) 0)
#endif

#endif