/*
 * vector.c.t
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

#define VECTOR_DONT_UNDEF_CONFIG
#include "vector.t.h"
#undef VECTOR_DONT_UNDEF_CONFIG

#include <stddef.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "priv/config_global.h"

#include <stdio.h>

struct VECTOR_s {
	TYPE * beg;
	TYPE * end;
	TYPE * rsrv;
};

/* -------------------------------------------------------------------------------- */

#define FOR_RANGE_IN(i, beg, end) \
	for(TYPE *(i) = (beg); (i) < (end); ++(i))
#define FOR_RANGE(this, i) \
	FOR_RANGE_IN((i), N(begin)(this), N(end)(this))
#define FOR FOR_RANGE

#define ASSERT_SIZE(a,b)                       assert( N(size)(a) == N(size)(b) )
#define ASSERT_IN_RANGE_BEGIN_END(this, x)     assert( N(begin)(this) <= x && x <= N(end)(this) )
#define ASSERT_IN_RANGE_BEGIN_BACK(this, x)    assert( N(begin)(this) <= x && x <  N(end)(this) )

/* Private Functions --------------------------------------------------------------------------- */

/**
 * @return Number of elements, which will occupy in p bytes.
 */
static inline size_t NMEMB ( N(VECTOR) * restrict this, uintptr_t p)
{
	assert( p % sizeof(TYPE) == 0 );
	return p / sizeof(TYPE);
}

/**
 * @return Number of bytes that n elements will occupy.
 */
static inline size_t TYPE_SIZEOF( N(VECTOR) * restrict this, size_t n)
{
	return n * sizeof(TYPE);
}

static int _realloc( N(VECTOR) * restrict this, size_t n)
{
	const size_t size = N(size)(this);

	TYPE *pnt = realloc(N(begin)(this), TYPE_SIZEOF(this,n));
	if ( !n ) {
		this->beg = this->end = this->rsrv = NULL;
		return 0;
	}
	if ( pnt == NULL ) {
		this->beg = this->end = this->rsrv = NULL;
		return -ENOMEM;
	}

	this->beg = pnt;
	this->end = &pnt[size];
	this->rsrv = &pnt[n];

	assert(this->end <= this->rsrv);

	return 0;
}

/* Constructor Destructor ------------------------------------------------------------------- */

N(VECTOR) * N(vector) ()
{
	return calloc(1, sizeof( N(VECTOR) ));
}

void N(vector_destructor) ( N(VECTOR) ** restrict this )
{
	free((*this)->beg);
	free(*this);
	*this = NULL;
}

/* Public const Functions  ------------------------------------------------------------------------ */

TYPE * N(at) ( const N(VECTOR) * restrict this, size_t n)
{
	return &this->beg[n];
}
TYPE * N(back) ( const N(VECTOR) * restrict this)
{
	return &this->end[-1];
}
TYPE * N(begin) ( const N(VECTOR) * restrict this)
{
	return this->beg;
}
size_t N(capacity) ( const N(VECTOR) * restrict this)
{
	return this->rsrv - this->beg;
}
bool N(empty) ( const N(VECTOR) * restrict this)
{
	return N(size)(this) == 0;
}
TYPE * N(end) ( const N(VECTOR) * restrict this)
{
	return this->end;
}
size_t N(max_size) ( const N(VECTOR) * restrict this)
{
	return SIZE_MAX / sizeof(TYPE);
}
size_t N(size) ( const N(VECTOR) * restrict this)
{
	return this->end - this->beg;
}

/* Public non-const Funtions ------------------------------------------------------------------------ */

int N(clear) ( N(VECTOR) * restrict this)
{
	return _realloc(this, 0);
}
int N(assign_range) ( N(VECTOR) * restrict this, const TYPE *first, size_t cnt)
{
	if ( _realloc(this, cnt) ) { return -ENOMEM; }
	FOR(this, i) {
		*i = *first++;
	}
	this->end = &this->beg[cnt];
	return 0;
}
int N(assign_fill) ( N(VECTOR) * restrict this, size_t n, const TYPE *val)
{
	if ( N(reserve)(this, n) ) { return -ENOMEM; }
	FOR(this,i) {
		*i = *val;
	}
	this->end = &this->beg[n];
	return 0;
}
TYPE * N(erase) ( N(VECTOR) * restrict this, TYPE *position)
{
	return N(erase_range)(this, position, 1);
}
TYPE * N(erase_range) ( N(VECTOR) * restrict this, TYPE *position, size_t cnt)
{
	ASSERT_IN_RANGE_BEGIN_END(this, position);
	assert( N(size)(this) >= cnt );
	const size_t pos = position - N(begin)(this);
	const size_t newsize = N(size)(this) - cnt;
	this->end -= cnt;
	memmove(position, position + cnt, TYPE_SIZEOF(this, N(end)(this) - position));
	if ( _realloc(this, newsize) ) { return NULL; }
	return N(at)(this, pos);
}
TYPE * N(insert_single_element) ( N(VECTOR) * restrict this, TYPE *position, const TYPE *val)
{
	return N(insert_range)(this, position, val, 1);
}
TYPE * N(insert_range) ( N(VECTOR) * restrict this, TYPE *position, const TYPE *first, size_t cnt)
{
	ASSERT_IN_RANGE_BEGIN_END(this, position);
	const size_t pos = position - N(begin)(this);
	const size_t newsize = N(size)(this) + cnt;
	if ( _realloc(this, newsize) ) { return NULL; }
	position = N(at)(this, pos);
	memmove(position + cnt, position, TYPE_SIZEOF(this, N(end)(this) - position));
	this->end += cnt;
	memcpy(position, first, TYPE_SIZEOF(this, cnt));
	return position;
}
N(VECTOR) * N(operator_equal_copy) ( N(VECTOR) * restrict this, const N(VECTOR) * restrict x)
{
	if ( N(reserve)(this, N(size)(x)) ) { return NULL; }
	return memcpy(N(begin)(this), N(begin)(x), N(size)(x));
}
int N(pop_back) ( N(VECTOR) * restrict this)
{
	return N(erase)(this, N(end)(this)) != NULL;
}
int N(push_back) ( N(VECTOR) * restrict this, const TYPE *val)
{
	return N(insert_single_element)(this, N(end)(this), val) == NULL ? -ENOMEM : 0;
}
int N(reserve) ( N(VECTOR) * restrict this, size_t n)
{
	return N(capacity)(this) < n ? _realloc(this, n) : 0;
}
int N(shrink_to_fix) ( N(VECTOR) * restrict this)
{
	return _realloc(this, N(size)(this));
}
int N(resize) ( N(VECTOR) * restrict this, size_t n)
{
	if ( N(reserve)(this, n) ) { return -ENOMEM; }
	this->end = this->rsrv;
	return 0;
}
void N(swap) ( N(VECTOR) * restrict this, N(VECTOR) * restrict x)
{
	struct VECTOR_s tmp;
	memcpy((void*)&tmp, this, sizeof(*this));
	memcpy((void*)this, x, sizeof(*this));
	memcpy((void*)x, &tmp, sizeof(*this));
}
