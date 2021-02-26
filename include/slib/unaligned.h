#ifndef SLIB_UNALIGNED_H
#define SLIB_UNALIGNED_H 1

/* uint64_t get_unaligned_u64(uint64_t *p);
 *
 * Returns the value of the possibly misaligned uint64_t at 'p'.  'p' may
 * actually be any type that points to a 64-bit integer.  That is, on Unix-like
 * 32-bit ABIs, it may point to an "unsigned long long int", and on Unix-like
 * 64-bit ABIs, it may point to an "unsigned long int" or an "unsigned long
 * long int".
 *
 * This is special-cased because on some Linux targets, the kernel __u64 is
 * unsigned long long int and the userspace uint64_t is unsigned long int, so
 * that any single function prototype would fail to accept one or the other.
 *
 * Below, "sizeof (*(P) % 1)" verifies that *P has an integer type, since
 * operands to % must be integers.
 */
#define get_unaligned_u64(P)                                \
    (BUILD_ASSERT(sizeof *(P) == 8),                        \
     BUILD_ASSERT_GCCONLY(!TYPE_IS_SIGNED(typeof(*(P)))),   \
     (void) sizeof (*(P) % 1),                              \
     get_unaligned_u64__((const uint64_t *) (P)))

#ifdef __GNUC__
/* GCC implementations. */
#define GCC_UNALIGNED_ACCESSORS(TYPE, ABBREV)   \
struct unaligned_##ABBREV {                     \
    TYPE x __attribute__((__packed__));         \
};                                              \
static inline struct unaligned_##ABBREV *       \
unaligned_##ABBREV(const TYPE *p)               \
{                                               \
    return (struct unaligned_##ABBREV *) p;     \
}                                               \
                                                \
static inline TYPE                              \
get_unaligned_##ABBREV(const TYPE *p)           \
{                                               \
    return unaligned_##ABBREV(p)->x;            \
}                                               \
                                                \
static inline void                              \
put_unaligned_##ABBREV(TYPE *p, TYPE x)         \
{                                               \
    unaligned_##ABBREV(p)->x = x;               \
}

GCC_UNALIGNED_ACCESSORS(uint16_t, u16);
GCC_UNALIGNED_ACCESSORS(uint32_t, u32);
GCC_UNALIGNED_ACCESSORS(uint64_t, u64__); /* Special case: see below. */

#else
/* Generic implementations. */

static inline uint16_t get_unaligned_u16(const uint16_t *p_)
{
    const uint8_t *p = (const uint8_t *) p_;
    return ntohs((p[0] << 8) | p[1]);
}

static inline void put_unaligned_u16(uint16_t *p_, uint16_t x_)
{
    uint8_t *p = (uint8_t *) p_;
    uint16_t x = ntohs(x_);

    p[0] = x >> 8;
    p[1] = x;
}

static inline uint32_t get_unaligned_u32(const uint32_t *p_)
{
    const uint8_t *p = (const uint8_t *) p_;
    return ntohl((p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3]);
}

static inline void put_unaligned_u32(uint32_t *p_, uint32_t x_)
{
    uint8_t *p = (uint8_t *) p_;
    uint32_t x = ntohl(x_);

    p[0] = x >> 24;
    p[1] = x >> 16;
    p[2] = x >> 8;
    p[3] = x;
}

static inline uint64_t get_unaligned_u64__(const uint64_t *p_)
{
    const uint8_t *p = (const uint8_t *) p_;
    return ntohll(((uint64_t) p[0] << 56)
                  | ((uint64_t) p[1] << 48)
                  | ((uint64_t) p[2] << 40)
                  | ((uint64_t) p[3] << 32)
                  | (p[4] << 24)
                  | (p[5] << 16)
                  | (p[6] << 8)
                  | p[7]);
}

static inline void put_unaligned_u64__(uint64_t *p_, uint64_t x_)
{
    uint8_t *p = (uint8_t *) p_;
    uint64_t x = ntohll(x_);

    p[0] = x >> 56;
    p[1] = x >> 48;
    p[2] = x >> 40;
    p[3] = x >> 32;
    p[4] = x >> 24;
    p[5] = x >> 16;
    p[6] = x >> 8;
    p[7] = x;
}

/* Only sparse cares about the difference between uint<N>_t and ovs_be<N>, and
 * that takes the GCC branch, so there's no point in working too hard on these
 * accessors. */
#define get_unaligned_be16 get_unaligned_u16
#define get_unaligned_be32 get_unaligned_u32
#define put_unaligned_be16 put_unaligned_u16
#define put_unaligned_be32 put_unaligned_u32
#define put_unaligned_be64 put_unaligned_u64

/* We do not #define get_unaligned_be64 as for the other be<N> functions above,
 * because such a definition would mean that get_unaligned_be64() would have a
 * different interface in each branch of the #if: with GCC it would take a
 * "ovs_be64 *", with other compilers any pointer-to-64-bit-type (but not void
 * *).  The latter means code like "get_unaligned_be64(ofpbuf_data(b))" would
 * work with GCC but not with other compilers, which is surprising and
 * undesirable.  Hence this wrapper function. */
static inline ovs_be64
get_unaligned_be64(const ovs_be64 *p)
{
    return get_unaligned_u64(p);
}
#endif

#endif /* unaligned.h */