
#ifndef SLIB_LIST_H
#define SLIB_LIST_H 1

#include <stddef.h>
#include <assert.h>

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct list_head_s list_head_t;

/* Doubly linked list head or element. */
struct list_head_s {
    list_head_t *prev;
    list_head_t *next;
};

#define LIST_HEAD_INITIALIZER(LIST) { LIST, LIST }

static inline void
list_init(list_head_t *list)
{
    list->next = list->prev = list;
}

static inline void
list_insert_before(list_head_t *before, list_head_t *element)
{
    element->prev = before->prev;
    element->next = before;
    before->prev->next = element;
    before->prev = element;
}

/* Remove elements 'first' though 'last' (exclusive) from their current list,
 * then inserts them just before 'before'. */
static inline void
list_splice(list_head_t *before, list_head_t *first, list_head_t *last)
{
    if (first == last) {
        return;
    }
    last = last->prev;

    /* Cleanly remove 'first'...'last' from its current list. */
    first->prev->next = last->next;
    last->next->prev = first->prev;

    /* Splice 'first'...'last' into new list. */
    first->prev = before->prev;
    last->next = before;
    before->prev->next = first;
    before->prev = last;
}

static inline void
list_push_front(list_head_t *list, list_head_t *element)
{
    list_insert_before(list->next, element);
}

static inline void
list_push_back(list_head_t *list, list_head_t *element)
{
    list_insert_before(list, element);
}

/* Puts 'element' in the position currently occupied by 'position'.
 * Afterward, 'position' is not part of the list. */
static inline void
list_replace(list_head_t *element, const list_head_t *position)
{
    element->next = position->next;
    element->next->prev = element;
    element->prev = position->prev;
    element->prev->next = element;
}

static inline void
list_moved__(list_head_t *list, const list_head_t *origin)
{
    if (list->next == origin) {
        list_init(list);
    } else {
        list->prev->next = list->next->prev = list;
    }
}

/* Initializes 'dst' with the contents of 'src', compensating for moving it
 * around in memory. */
static inline void
list_move(list_head_t *dst, list_head_t *src)
{
    *dst = *src;
    list_moved__(dst, src);
}

/* Remove 'element' from its list and returns the element that followed it.
 * Undefined behavior if 'element' is not in a list. */
static inline list_head_t *
list_remove(list_head_t *element)
{
    element->prev->next = element->next;
    element->next->prev = element->prev;
    return element->next;
}

/* Removes the front element from 'list' and returns it.
 * Undefined behavior if 'list' s empty before removal. */
static inline list_head_t *
list_pop_front(list_head_t *list)
{
    list_head_t *front = list->next;

    list_remove(front);
    return front;
}

/* Removes the back element from 'list' and returns it.
 * Undefined behavior if 'list' is empty before removal. */
static inline list_head_t *
list_pop_back(list_head_t *list)
{
    list_head_t *back = list->prev;

    list_remove(back);
    return back;
}

/* Return the number of elements in 'list'. */
static inline size_t
list_size(const list_head_t *list)
{
    const list_head_t *e;
    size_t cnt = 0;

    for (e = list->next; e != list; e = e->next) {
        cnt++;
    }

    return cnt;
}

static inline bool
list_is_empty(const list_head_t *list)
{
    return list->next == list;
}

/* Returns true if 'list' has 0 or 1 elements, false otherwise. */
static inline bool
list_is_short(const list_head_t *list)
{
    return list->next == list->prev;
}

static inline bool
list_is_singleton(const list_head_t *list)
{
    return list_is_short(list) && !list_is_empty(list);
}

/* Returns the front element in 'list'.
 * Undefined behavior if 'list' is empty. */
static inline list_head_t *
list_front(const list_head_t *list)
{
    list_head_t *list_ = CONST_CAST(list_head_t *, list);

    assert(!list_is_empty(list_));

    return list->next;
}

/* Return the back element in 'list'.
 * Undefined behavior if 'list' is empty. */
static inline list_head_t *
list_back(const list_head_t *list)
{
    list_head_t *list_ = CONST_CAST(list_head_t *, list);

    assert(!list_is_empty(list_));

    return list->prev;
}

/* Transplant a list into another, and resets the origin list */
static inline void
list_push_back_all(list_head_t *dst, list_head_t *src)
{
    list_splice(dst, src->next, src);
}

#define list_entry(ptr, type, member) \
    SLIB_CONTAINER_OF(ptr, type, member)

#define list_first_entry(head, type, member) \
    list_entry((head)->next, type, member)

#define list_for_each(ptr, head, member) \
    for (ptr = list_entry((head)->next, __typeof__(*ptr), member); \
        &(ptr)->member != (head); \
        (ptr) = list_entry((ptr)->member.next, __typeof__(*(ptr)), member))

#define list_for_each_safe(ptr, ptr_next, head, member) \
    for (ptr = list_entry((head)->next, __typeof__(*ptr), member), \
        ptr_next = list_entry((ptr)->member.next, __typeof__(*ptr), member); \
        &(ptr)->member != (head); \
        ptr = ptr_next, \
        ptr_next = list_entry((ptr_next)->member.next, __typeof__(*ptr_next), \
            member))

#define list_for_each_continue(ptr, head, member) \
    for (ptr = list_entry((ptr)->member.next, __typeof__(*(ptr)), member); \
        &(ptr)->member != (head); \
        (ptr) = list_entry((ptr)->member.next, __typeof__(*(ptr)), member))

#define list_for_each_reverse(ptr, head, member) \
    for (ptr = list_entr((head)->prev, __typeof__(*(ptr)), member); \
        &(ptr)->member != (head); \
        (ptr) = list_entry((ptr)->member.prev, __typeof__(*(ptr)), member))

#define list_for_each_reverse_continue(ptr, head, member) \
    for (ptr = list_entry((ptr)->member.prev, __typeof__(*(ptr)), member); \
        &(ptr)->member != (head); \
        (ptr) = list_entry((ptr)->member.prev, __typeof__(*(ptr)), member))

#ifdef __cplusplus
}
#endif

#endif /* list.h */