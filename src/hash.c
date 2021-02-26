#include "slib/hash.h"

#include <string.h>

#include "slib/unaligned.h"
#include "compiler.h"

/* Returns the hash of 'a', 'b', and 'c'. */
uint32_t
hash_3words(uint32_t a, uint32_t b, uint32_t c)
{
    return hash_finish(hash_add(hash_add(hash_add(a, 0), b), c), 12);
}

/* Returns the hash of the 'n' bytes at 'p', starting from 'basis'. */
uint32_t
hash_bytes(const void *p_, size_t n, uint32_t basis)
{
    const uint32_t *p = p_;
    size_t orig_n = n;
    uint32_t hash;

    hash = basis;
    while (n >= 4) {
        hash = hash_add(hash, get_unaligned_u32(p));
        n -= 4;
        p += 1;
    }

    if (n) {
        uint32_t tmp = 0;

        memcpy(&tmp, p, n);
        hash = hash_add(hash, tmp);
    }

    return hash_finish(hash, orig_n);
}

uint32_t
hash_double(double x, uint32_t basis)
{
    uint32_t value[2];
    BUILD_ASSERT_DECL(sizeof x == sizeof value);

    memcpy(value, &x, sizeof value);
    return hash_3words(value[0], value[1], basis);
}

uint32_t
hash_words__(const uint32_t p[], size_t n_words, uint32_t basis)
{
    return hash_words_inline(p, n_words, basis);
}

uint32_t
hash_words64__(const uint64_t p[], size_t n_words, uint32_t basis)
{
    return hash_words64_inline(p, n_words, basis);
}

