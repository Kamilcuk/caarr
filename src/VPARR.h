/*
 * VPARR.h
 *
 *  Created on: 18 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 *
 * VPARR - Void Pointer ARRay
 * Library for using and manipulating void pointer arrays.
 * You can easily rename whole library using sed 's/VPARR/something/g'
 */
#ifndef SRC_VPARR_H_
#define SRC_VPARR_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

/* public macros ----------------------------------------------------------- */

/**
 * \defgroup declare an VPARR variable with initialization
 * @{
 */
/**
 * Declare array "base" of type "type" of elements "nmemb".
 * Declare "arr" of type "VPARR" pointing to variable "base".
 */
#define VPARR_DECLARE_ON_STACK(type, arr, base, nmemb) \
	(type) (base) [(nmemb)] ; \
	VPARR (arr) = VPARR_INIT_FROM_STATIC((base))
/**
 * @}
 */

/**
 * \defgroup VPARR variable initialization
 * @{
 */
#define VPARR_INIT_ALL(base0,nmemb0,size0) (VPARR){.base=(base0),.nmemb=(nmemb0),.size=(size0)}
#define VPARR_INIT_SIZE(size)              VPARR_INIT_ALL(NULL,0,size)
#define VPARR_INIT_TYPE(type)              VPARR_INIT_SIZE(sizeof(type))

/**
 * Initialize VPARR variable to point to array of type "type"
 */
#define VPARR_INIT(type)                   VPARR_INIT_TYPE(type)

/**
 * Initialize VPARR variable to point to a on stack allocated variable "base"
 */
#define VPARR_INIT_FROM_STATIC(base)       VPARR_INIT_ALL(base,sizeof(base)/sizeof(*base),sizeof(*base))
/*
 * @}
 */

/**
 * @defgroup Iterate over VPARR variable
 * @{
/**
 * Iterate over VPARR "arr" using a range based approach.
 * @pre  0 <= start <= stop <= arr->nmemb
 */
#define VPARR_FOR_RANGE_BASED_START_STOP_ENDBASE(ibase, start, endbase, stop, arr) \
		for(const void *(ibase) = (arr)->base + (start) * (arr)->size, \
			* const (endbase) = (arr)->base + (stop) * (arr)->size; \
			(ibase) < (endbase); \
			(ibase) += (arr)->size)
/**
 * Iterate over array "arr" using a range based approach.
 */
#define VPARR_FOR_RANGE_BASED(ibase, arr) \
		VPARR_FOR_RANGE_BASED_START_STOP_ENDBASE((ibase), 0, (__endbase_ ## ibase), (arr)->nmemb, (arr))
/**
 * Iterate over VPARR "arr" using a range based approach in reverse.
 * @pre 0 <= stop <= start <= arr->nmemb-1
 */
#define VPARR_FOR_RANGE_BASED_REVERSE_START_STOP_ENDBASE(ibase, start, endbase, stop, arr) \
		for(const void *(ibase) = (arr)->base + (start) * (arr)->size, \
			* const (endbase) = (arr)->base + (stop) * (arr)->size; \
			(ibase) >= (endbase); \
			(ibase) -= (arr)->size)
/**
 * Iterate over array "arr" using a range based approach in reverse.
 */
#define VPARR_FOR_RANGE_BASED_REVERSE(ibase, arr) \
		VPARR_FOR_RANGE_BASED_REVERSE_START_STOP_ENDBASE((ibase), (arr)->nmemb, (__endbase_ ## ibase), 0, (arr))
/**
 * Iterate over stack, but don't care about iterator value.
 * Teoretically faster then VPARR_FOR_BEGIN, practically same.
 */
#define VPARR_FOR_REVERSE_BEGIN(i, ibase, arr) \
	do { \
		void *(ibase) = (arr)->base; \
		for(size_t (i) = (arr)->nmemb; (i); --(i), (ibase)+=(arr)->size)
/**
 * Open VPARR for block
 * Iterate over array "arr", using iterator "i" and pointer "ibase"
 */
#define VPARR_FOR_BEGIN(i, ibase, arr) \
	do { \
		void *(ibase) = (arr)->base; \
		for(size_t (i) = 0; (i) < (arr)->nmemb; ++(i), (ibase)+=(arr)->size)
/**
 * Close VPARR for block
 */
#define VPARR_FOR_END \
	}while(0);
/**
 * @}
 */

#define VPARR_GETSIZE(arr)  ((arr)->size)
#define VPARR_GETNMEMB(arr) ((arr)->nmemb)
#define VPARR_GETBASE(arr)  ((arr)->base)

/* public types ------------------------------------------------------------ */

/**
 * Represents an Void Pointer ARRay
 */
typedef struct VPARR_s {
	/**
	 * Count of array members
	 */
	size_t nmemb;
	/**
	 * Size of one member in array
	 */
	const size_t size;
	/**
	 * Pointer to the initial member of array
	 */
	void *base;
} VPARR;

/* exported functions ----------------------------------------------------------- */

/**
 * @defgroup memory dynamic functions
 * @{
 */
int VPARR_realloc(VPARR *arr, size_t nmemb);
#define VPARR_ALLOC_TYPE(type, nmemb) VPARR_alloc(nmemb, sizeof(type))
static inline VPARR VPARR_alloc(size_t nmemb, size_t size)
{
	VPARR ret = VPARR_INIT_SIZE(size);
	if ( VPARR_realloc(&ret, nmemb) < 0 ) {
		assert(false);
	}
	return ret;
}
static inline void VPARR_destruct(VPARR *arr) {
	free(arr->base);
}
/**
 * @}
 */

/* ------------------------------------------------------------------------------------
 * @defgroup VPARR vector member functions
 * member functions from http://en.cppreference.com/w/cpp/container/vector ---
 * @{
 */
static inline void *VPARR_begin(VPARR *arr)
{
	return arr->base;
}

static inline void *VPARR_end(VPARR *arr)
{
	return arr->base + arr->nmemb * arr->size;
}

static inline void *VPARR_at(VPARR *arr, size_t pos)
{
	if ( pos > arr->nmemb ) {
		if ( VPARR_realloc(arr, pos) < 0 ) {
			return NULL;
		}
	}
	return arr->base + pos * arr->size;
}

static inline void *VPARR_front(VPARR *arr, size_t pos)
{
	return arr->base;
}

static inline void *VPARR_back(VPARR *arr, size_t pos)
{
	return arr->base + (arr->nmemb-1) * arr->size;
}

static inline bool VPARR_empty(VPARR *arr)
{
	return arr->nmemb == 0;
}

static inline size_t VPARR_size(VPARR *arr)
{
	return arr->nmemb;
}

static inline size_t VPARR_max_size(VPARR *arr)
{
	return UINT16_MAX;
}

static inline int VPARR_reserve(VPARR *arr, size_t count)
{
	if ( count >= arr->nmemb ) {
		return VPARR_realloc(arr, count);
	}
	return 0;
}

static inline int VPARR_memsetcmp(const void *ptr0, int value0, size_t len)
{
	const char *ptr = ptr0;
	unsigned char value = value0;
	while(len--) {
		if ( *ptr++ != value ) {
			return 1;
		}
	}
	return 0;
}

static inline int VPARR_shrink_to_fit(VPARR *arr, size_t count)
{
	size_t i = arr->nmemb;
	VPARR_FOR_RANGE_BASED_REVERSE_START_STOP_ENDBASE(last, arr->nmemb-1, first, 0, arr) {
		if (VPARR_memsetcmp(last, 0, arr->size)) {
			break;
		}
		--i;
	}
	return VPARR_realloc(arr, i);
}

static inline void VPARR_clear(VPARR *arr)
{
	free(arr->base);
	arr->base = NULL;
	arr->nmemb = 0;
}

static inline void *VPARR_insert_single_element(VPARR *arr, size_t pos, void *value)
{
	assert(pos <= arr->nmemb);
	if ( VPARR_realloc(arr, arr->nmemb + 1) < 0 ) {
		return NULL;
	}
	void * const i = arr->base + pos*arr->size;
	memmove(i+arr->size, i, (arr->nmemb - pos)*arr->size);
	memcpy(i, value, arr->size);
	return i;
}

static inline int VPARR_insert_fill(VPARR *arr, size_t pos, size_t n, void *value)
{
	assert(pos <= arr->nmemb);
	if ( VPARR_realloc(arr, arr->nmemb + n) < 0 ) {
		return -ENOMEM;
	}
	void * const i = arr->base + pos * arr->size;
	memmove(i + n * arr->size, i, (arr->nmemb - pos) * arr->size);
	VPARR_FOR_RANGE_BASED(first, arr) {
		memcpy((void*)first, value, arr->size);
	}
	return 0;
}


static inline void *VPARR_insert_range(VPARR *arr, size_t pos, VPARR *arr2)
{
	assert(pos <= arr->nmemb && arr->size == arr2->size);
	if ( VPARR_realloc(arr, arr->nmemb + arr2->nmemb) < 0 ) {
		return NULL;
	}
	void * const i = arr->base + pos * arr->size;
	memmove(i + arr2->nmemb*arr->size, i, arr2->nmemb*arr->size);
	memcpy(i, arr2->base, arr2->nmemb*arr2->size);
	return i;
}

static inline int VPARR_erase(VPARR *arr, size_t pos)
{
	assert(pos < arr->nmemb);
	memmove(arr->base + (pos+1)*arr->size, arr->base + pos*arr->size, (arr->nmemb - pos - 1)*arr->size);
	if ( VPARR_realloc(arr, arr->nmemb-1) < 0 ) {
		return -ENOMEM;
	}
	return 0;
}

static inline void *VPARR_push_back(VPARR *arr, void *value)
{
	if ( VPARR_realloc(arr, arr->nmemb+1) < 0 ) {
		return NULL;
	}
	void * const i = arr->base+(arr->nmemb-1)*arr->size;
	memcpy(i, value, arr->size);
	return i;
}

static inline int VPARR_pop_back(VPARR *arr)
{
	return VPARR_realloc(arr, arr->nmemb-1);
}

static inline int VPARR_resize(VPARR *arr, size_t count, void *value)
{
	size_t last_nmemb = arr->nmemb;
	if ( VPARR_realloc(arr, count) < 0 ) {
		return -ENOMEM;
	}
	if ( value != NULL ) {
		VPARR_FOR_RANGE_BASED_START_STOP_ENDBASE(first, last_nmemb, last, arr->nmemb, arr) {
			memcpy((void*)first, value, arr->size);
		}
	}
	return 0;
}

static inline void VPARR_swap(VPARR *arr1, VPARR *arr2)
{
	assert(arr1->size == arr2->size);
	VPARR temp = *arr1;
	memcpy(arr2, arr1, sizeof(VPARR));
	memcpy(arr1, &temp, sizeof(VPARR));
}
/**
 * @}
 */

/**
 * @defgroup static memory functions
 * These functions do not allocate or reallocate any of the memory.
 * If for ex. copying one array to another, user needs to make sure, that there
 * is enough space in destination.
 * @{
 |*/
void VPARR_copy(VPARR *dest, VPARR *src);
void VPARR_memcopy(VPARR *dest, void *src, size_t src_nmemb);
int VPARR_cmp(VPARR *s1, VPARR *s2);
int VPARR_memcmp(VPARR *s1, void *s2, size_t s2_nmemb);
size_t VPARR_count(VPARR *arr, void *val);
bool VPARR_equal(VPARR *arr1, VPARR *arr2);
void VPARR_fill(VPARR *arr, void *elem);
void *VPARR_find(VPARR *arr, void *elem);
void *VPARR_find_if(VPARR *arr, bool(*compar)(void*));
void *VPARR_find_if_not(VPARR *arr, bool(*compar)(void*));
void VPARR_for_each(VPARR *arr, void (*f)(void*));
bool VPARR_is_sorted(VPARR *arr, int (*compar)(const void *, const void *));
int VPARR_merge(VPARR *arr1, VPARR *arr2, VPARR *result, int (*compar)(const void *, const void *));
void VPARR_replace(VPARR *arr, void *old_value, void *new_value);
void VPARR_shuffle(VPARR *arr);
void *VPARR_max_element(VPARR *arr, int (*compar)(const void *, const void *));
void *VPARR_min_element(VPARR *arr, int (*compar)(const void *, const void *));
struct VPARR_minmax_s { void *min, *max; };
struct VPARR_minmax_s VPARR_minmax_element(VPARR *arr, int (*compar)(const void *, const void *));
/**
 * @}
 */

/* macro exported functions --------------------------------------------------  */

void VPARR_bsearch(VPARR *arr, void *key, int (*compar)(const void *, const void *));
#define VPARR_bsearch(arr, key, func) bsearch((key), (arr)->base, (arr)->nmemb, (arr)->size, (compar))

void VPARR_qsort(VPARR *base, int (*compar)(const void *, const void *));
#define VPARR_qsort(arr, compar) qsort((arr)->base, (arr)->nmemb, (arr)->size, (compar))
#define VPARR_sort(arr, compar) VPARR_qsort((arr),(compar))

void VPARR_qsort_r(VPARR *base, int (*compar)(const void *, const void *, void *), void *arg);
#define VPARR_qsort_r(arr, compar, arg) qsort_r((arr)->base, (arr)->nmemb, (arr)->size, (compar), (arg))

#endif /* SRC_VPARR_H_ */
