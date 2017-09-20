/*
 * VPARR.c
 *
 *  Created on: 18 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include "VPARR.h"

#include <errno.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

/* private macros ------------------------------------------------------------- */

#define VPARR_ITERATE(i, ibase, arr) \
		void *(ibase) = (arr)->base; \
		for(size_t (i) = (arr)->nmemb; (i); --(i), (ibase)+=(arr)->size)

#define VPARR_ITERATE_FROM_SECOND(i, ibase, arr) \
		void *(ibase) = (arr)->base + (arr)->size; \
		for(size_t (i) = (arr)->nmemb-1; (i); --(i), (ibase)+=(arr)->size)

#define VPARR_ITERATE_TWO(i,ibase0,ibase1,arr) \
		void *ibase0 = arr->base; \
		void *ibase1 = arr->base+arr->size; \
		for(size_t i = arr->nmemb-1; i; --i, ibase0+=arr->size, ibase1+=arr->size)

#ifndef __GNUC__
# define VPARR_PRE_ACT_VAL(var, val, act, reverseact) \
	( VPARR_PRE_ACT_VAL(var,val,act) , (var) reverseact (val) )
#else
# define VPARR_PRE_ACT_VAL(var, val, act, reverseact) \
	({ const __typeof__ (var) __save_var_ ## var = (var); \
		VPARR_POST_ACT_VAL(var, val, act); \
		( __save_var_ ## var ); })
#endif
#define VPARR_POST_ACT_VAL(var, val, act) \
	( (var) act ## = (val) )

#define VPARR_POST_ACT(ibase, arr, action) \
	VPARR_POST_ACT_VAL((ibase), (arr)->size, action)
#define VPARR_PRE_ACT(ibase, arr, action, reverseact) \
	VPARR_PRE_ACT_VAL((ibase), (arr)->size, action, reverseact)

/* exported functions ----------------------------------------------------------- */

/* ****** dynamic memory functions */

int VPARR_realloc(VPARR *arr, size_t nmemb)
{
	assert(arr && arr->size);
	if ( arr->size > INT_MAX / nmemb ) { // nmemb * arr->size will overflow
		return -EINVAL;
	}
	void *pnt = realloc(arr->base, nmemb * arr->size);
	if ( nmemb && !pnt ) {
		assert(errno == -ENOMEM);
		return -ENOMEM;
	}
	arr->base = pnt;

	// initialize new values to zero
	if ( nmemb > arr->nmemb ) {
		memset(arr->base + arr->nmemb * arr->size, 0, (arr->nmemb - nmemb) * arr->size);
	}

	arr->nmemb = nmemb;
	return 0;
}

/* ****** static memory functions */

void VPARR_for_each(VPARR *arr, void (*f)(void*))
{
	VPARR_ITERATE(i, base, arr) {
		f(base);
	}
}

void VPARR_copy(VPARR *dest, VPARR *src)
{
	if ( dest->size != src->size ) {
		return;
	}
	VPARR_memcopy(dest, src->base, src->nmemb);
}

void VPARR_memcopy(VPARR *dest, void *src, size_t src_nmemb)
{
	if ( dest->nmemb < src_nmemb ) {
		return;
	}
	memcpy(dest->base, src, src_nmemb * dest->size);
}

int VPARR_cmp(VPARR *s1, VPARR *s2)
{
	if ( s1->size != s2->size ) {
		return INT_MIN;
	}
	return VPARR_memcmp(s1, s2->base, s2->nmemb);
}

int VPARR_memcmp(VPARR *s1, void *s2, size_t s2_nmemb)
{
	if ( s1->nmemb != s2_nmemb ) {
		return INT_MIN;
	}
	return memcmp(s1->base, s2, s2_nmemb*s1->size);
}


size_t VPARR_count(VPARR *arr, void *val)
{
	size_t ret = 0;
	VPARR_ITERATE(i, base, arr) {
		if ( !memcmp(base, val, arr->size) ) {
			++ret;
		}
	}
	return ret;
}

bool VPARR_equal(VPARR *arr1, VPARR *arr2)
{
	if ( arr1->nmemb != arr2->nmemb || arr1->size != arr2->size ) {
		return false;
	}
	return !memcmp(arr1->base, arr2->base, arr1->nmemb*arr1->size) ? true : false;
}

void VPARR_fill(VPARR *arr, void *elem)
{
	VPARR_ITERATE(i, base, arr) {
		memcpy(base, elem, arr->size);
	}
}

void *VPARR_find(VPARR *arr, void *elem)
{
	VPARR_ITERATE(i, base, arr) {
		if ( !memcmp(base, elem, arr->size) ) {
			return base;
		}
	}
	return NULL;
}

void *VPARR_find_if(VPARR *arr, bool(*compar)(void*))
{
	VPARR_ITERATE(i, base, arr) {
		if ( compar(base) ) {
			return base;
		}
	}
	return NULL;
}

void *VPARR_find_if_not(VPARR *arr, bool(*compar)(void*))
{
	VPARR_ITERATE(i, base, arr) {
		if ( !compar(base) ) {
			return base;
		}
	}
	return NULL;
}

bool VPARR_is_sorted(VPARR *arr, int (*compar)(const void *, const void *))
{
	if ( arr->nmemb == 1 ) {
		return true;
	}
	VPARR_ITERATE_TWO(i, base0, base1, arr) {
		if ( compar(base0, base1) < 0 ) {
			return false;
		}
	}
	return true;
}

void VPARR_replace(VPARR *arr, void *old_value, void *new_value)
{
	VPARR_ITERATE(i, base, arr) {
		if ( !memcmp(base, old_value, arr->size) ) {
			memcpy(base, new_value, arr->size);
		}
	}
}

#include <stdio.h>
int VPARR_merge(VPARR *arr1, VPARR *arr2, VPARR *result, int (*compar)(const void *, const void *))
{
	if ( arr1->size != arr2->size || arr1->size != result->size || result->nmemb < arr1->nmemb + arr2->nmemb ) {
		return -EINVAL;
	}

	const size_t size = result->size;
	for(void
			*base1  = arr1->base,
			*endbase1 = arr1->base + arr1->nmemb * size,
			*base2 = arr2->base,
			*endbase2 = arr2->base + arr2->nmemb * size,
			*dest = result->base;
			;
			dest+=size) {

		if ( base1 >= endbase1 ) {
			memcpy(dest, base2, endbase2 - base2);
			break;
		}

		if ( base2 >= endbase2 ) {
			memcpy(dest, base1, endbase1 - base1);
			break;
		}

		if ( compar(base1,base2) > 0 ) {
			memcpy(dest, base2, size);
			base2+=size;
		} else {
			memcpy(dest, base1, size);
			base1+=size;
		}

	}
	return arr1->nmemb + arr2->nmemb;
}

void VPARR_shuffle(VPARR *arr)
{
	const size_t size = arr->size;
	const size_t n = arr->nmemb;
    char * const base = arr->base;
    char tmp[size];
    size_t stride = size * sizeof(char);

    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; ++i) {
            size_t rnd = (size_t) rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, base + j * stride, size);
            memcpy(base + j * stride, base + i * stride, size);
            memcpy(base + i * stride, tmp, size);
        }
    }
}

void *VPARR_max_element(VPARR *arr, int (*compar)(const void *, const void *))
{
	void *ret = arr->base;
	if ( arr->nmemb == 1 ) {
		return ret;
	}
	VPARR_ITERATE(i, base, arr) {
		if (compar(ret, base)) {
			ret = base;
		}
	}
	return ret;
}

void *VPARR_min_element(VPARR *arr, int (*compar)(const void *, const void *))
{
	void *ret = arr->base;
	if ( arr->nmemb == 1 ) {
		return ret;
	}
	VPARR_ITERATE_FROM_SECOND(i, base, arr) {
		if (compar(base, ret)) {
			ret = base;
		}
	}
	return ret;
}

struct VPARR_minmax_s VPARR_minmax_element(VPARR *arr, int (*compar)(const void *, const void *))
{
	// second version from http://en.cppreference.com/w/cpp/algorithm/minmax_element
	struct VPARR_minmax_s ret = {arr->base,arr->base};
	if ( arr->nmemb == 1 ) {
		return ret;
	}
	void *first = (arr)->base;
	void *last = (arr)->base + (arr)->nmemb * (arr)->size;
	while(first != last) {
		first += arr->size;
		void *i = first;
		first += arr->size;
        if (i == last) {
            if (compar(i, ret.min)) ret.min = i;
            else if (!(compar(i, ret.max))) ret.max = i;
            break;
        } else {
            if (compar(first, i)) {
                if (compar(first, ret.min)) ret.min = first;
                if (!(compar(i, ret.max))) ret.max = i;
            } else {
                if (compar(i, ret.min)) ret.min = i;
                if (!(compar(first, ret.max))) ret.max = first;
            }
        }
    }
    return ret;
}

