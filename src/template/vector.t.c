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
#define DEBUG(str, ...) fprintf(stderr, "%s():%d: " str, __FUNCTION__, __LINE__, ## __VA_ARGS__)

#ifdef TYPE_VOID
# define TYPE_SIZE(a)  a->size
# define INIT_SIZE(v)   .size=v
#else
# define TYPE_SIZE(a)   ({ static_assert(0,"Dont use TYPE_SIZE is TYPE is not void!"), 0; })
# define INIT_SIZE(v)
#endif

struct VECTOR_s {
#ifdef TYPE_VOID
	const size_t size;
#endif
	TYPE * beg;
#ifdef USE_NMEMB
	size_t nmemb;
	size_t rsrv;
#else
	TYPE * end;
	TYPE * rsrv;
#endif
};

#define BEGIN(a) a->beg

#ifdef USE_NMEMB
# define INIT(arr,len)   { .beg=arr, .nmemb=len, INIT_SIZE(sizeof(*arr))}
# define END(a)          (BEGIN(a)+LEN_N(a,SIZE(a)))
# define SIZE(a)         a->nmemb
# define SIZE_SET(a,n)   a->nmemb = n
# define RSRV(a)         a->rsrv
# define RSRV_SET(a,n)   a->rsrv = n
#else
# define INIT(arr,len)   { .beg=arr, .end=arr+sizeof(*arr)*len, INIT_SIZE(sizeof(*arr))}
# define END(a)          a->end
# define SIZE(a)         CNT_N(a, END(a) - BEGIN(a))
# define SIZE_SET(a,n)   a->end = BEGIN(a) + LEN_N(a,n)
# define RSRV(a)         CNT_N(a, a->rsrv - BEGIN(a))
# define RSRV_SET(a,n)   a->rsrv = BEGIN(a) + LEN_N(a,n)
#endif

/* ------------------------------------------------------------------------------- */

#if 0
static inline void PNT_INC( N(VECTOR) * restrict this, TYPE **p) {
#ifdef TYPE_VOID
	*p += LEN_N(this, 1);
#else
	++*p;
#endif
}
static inline void PNT_DEC( N(VECTOR) * restrict this, TYPE **p) {
#ifdef TYPE_VOID
	*p -= LEN_N(this, 1);
#else
	--*p;
#endif
}
static inline TYPE * PNT_ADD( N(VECTOR) * restrict this, TYPE *p, size_t n) {
#ifdef TYPE_VOID
	return p + LEN_N(this, n);
#else
	return &p[n];
#endif
}
static inline TYPE * PNT_SUB( N(VECTOR) * restrict this, TYPE *p, size_t n) {
#ifdef TYPE_VOID
	return p - LEN_N(this, n);
#else
	return &p[-n];
#endif
}
#endif

#ifdef TYPE_VOID
# define PNT_INC(this,p)       p += LEN_N(this, 1)
# define PNT_DEC(this,p)       p -= LEN_N(this, 1)
# define PNT_ADD(this,p,n)    (p)+  LEN_N(this, n)
# define PNT_SUB(this,p,n)    (p)-  LEN_N(this, n)
#else
# define PNT_INC(this,p)      ++(p)
# define PNT_DEC(this,p)      --(p)
# define PNT_ADD(this,p,n)    &(p)[(n)]
# define PNT_SUB(this,p,n)    &(p)[-(n)]
#endif

#define BACK(this)         PNT_SUB(this, END(this), -1);

/* -------------------------------------------------------------------------------- */

#define FOR_TWO_RANGE_IN(a,i,j,beg,end) \
	for(TYPE *i = beg, *j = PNT_ADD(a,beg,1); \
		j < END(a); \
		PNT_INC(a,i), PNT_INC(a,j) )
#define FOR_TWO_RANGE(this,i,j) FOR_TWO_RANGE_IN(this,i,j,BEGIN(this),END(this))

#define FOR_TWO_RANGE_REV_IN(a,i,j,beg,end) \
	for(TYPE *i = end, *j = PNT_SUB(a,end,1); \
		i >= beg; \
		PNT_DEC(a,i), PNT_DEC(a,j) )
#define FOR_TWO_RANGE_REV(this,i,j) FOR_TWO_RANGE_REV_IN(this,i,j,BEGIN(this),)
#define FOR_RANGE_IN(a,ai,beg,end)  for(TYPE *ai = beg; ai < end; PNT_INC(a,ai))
#define FOR_RANGE(a,ai)             FOR_RANGE_IN(a,ai,BEGIN(a),END(a))
#define FOR FOR_RANGE

#ifdef TYPE_VOID
# define ASSERT_TYPE(a,b)  assert(TYPE_SIZE(a) == TYPE_SIZE(b))
#else
# define ASSERT_TYPE(a,b)
#endif
#define ASSERT_SIZE(a,b)   assert(SIZE(a) == SIZE(b))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define GET(i,n)  _GET(i,n,TYPE_SIZE(this))
#define SET(i,v)  _SET(i,v,TYPE_SIZE(this))
#define CMP(i,v)  _CMP(i,v,TYPE_SIZE(this))

/* --------------------------------------------------------------------------- */

#ifdef TYPE_VOID
# define _TYPE_SIZE(this) this->size
#else
# define _TYPE_SIZE(this) sizeof(*BEGIN(this))
#endif

/**
 * @return Number of elements, which will occupy in p bytes.
 */
static inline size_t CNT_N (struct VECTOR_s * restrict this, uintptr_t p)
{
	assert(_TYPE_SIZE(this) != 0);
	assert( p % _TYPE_SIZE(this) == 0 );
	return p / _TYPE_SIZE(this);
}

/**
 * @return Number of bytes that n elements will occupy.
 */
static inline uintptr_t LEN_N (struct VECTOR_s * restrict this, size_t n)
{
	return n * _TYPE_SIZE(this);
}

static int _realloc( N(VECTOR) * restrict this, size_t n)
{
	void *pnt = realloc(BEGIN(this), LEN_N(this,n));
	if ( !pnt ) {
		return -ENOMEM;
	}
	if ( !n ) {
		BEGIN(this) = NULL;
		SIZE_SET(this, 0);
		RSRV_SET(this, 0);
		return 0;
	}

	const size_t size = SIZE(this);
	BEGIN(this) = pnt;
	RSRV_SET(this, n);
	SIZE_SET(this, size);

	return 0;
}

static TYPE * _realloc_move_forward( N(VECTOR) * restrict this, TYPE *position0, size_t n )
{
	assert(BEGIN(this) <= position0 && position0 <= END(this));
	const size_t pos = CNT_N(this, END(this) - position0);
	if ( _realloc(this, SIZE(this)+n) ) { return NULL; }
	TYPE * const position = PNT_ADD(this, BEGIN(this), pos);
	memmove(PNT_ADD(this, position, n), position, LEN_N(this, n));
	return position;
}

static TYPE * _realloc_move_backward( N(VECTOR) * restrict this, TYPE *position0, size_t n )
{
	assert(BEGIN(this) <= position0 && position0 <= END(this));
	assert( SIZE(this) >= n );
	const size_t pos = CNT_N(this, END(this) - position0);
	memmove(position0, PNT_ADD(this, position0, n), LEN_N(this, n));
	if ( _realloc(this, SIZE(this) - n) ) { return NULL; }
	TYPE * const position = PNT_ADD(this, BEGIN(this), pos);
	return position;
}

/* ------------------------------------------------------------------- */

N(VECTOR) * N(vector) PAR_SIZE()
{
	N(VECTOR) * this = calloc(1, sizeof(struct VECTOR_s));
	if ( !this ) return NULL;
#ifdef TYPE_VOID
	*(size_t*)&(this->size) = __size;
#endif
	return this;
}

N(VECTOR) * N(vector_count) PAR_SIZE(size_t n)
{
	N(VECTOR) * this = CALL(vector);
	if ( !this ) return this;
	if ( _realloc(this, n) != 0 ) {
		free(this);
		return NULL;
	}
	return this;
}

N(VECTOR) * N(vector_fill) PAR_SIZE(size_t n, TYPE *val)
{
	N(VECTOR) * this = CALL(vector_count, n);
	if ( !this ) return this;
	FOR(this,i) {
		SET(i,val);
	}
	return this;
}

N(VECTOR) * N(vector_copy) PAR_SIZE(N(VECTOR) * restrict other)
{
	N(VECTOR) * this = CALL(vector_count, SIZE(other));
	if ( !this ) return this;
	return memcpy(BEGIN(this), BEGIN(other), SIZE(this));
}

N(VECTOR) * N(vector_range) PAR_SIZE(TYPE *first, TYPE *last)
{
	assert( last > first );
	N(VECTOR) * this = CALL(vector);
	if ( !this ) return this;
	if ( N(reserve)(this, CNT_N(this, last - first)) ) {
		free(this);
		return NULL;
	}
	return memcpy(BEGIN(this), first, SIZE(this));
}

void N(vector_desctructor) (N(VECTOR) ** restrict this)
{
	if ( _realloc(*this, 0) ) {
		assert(0);
	}
	free(*this);
	*this = NULL;
}

/* ------------------------------------------------------------------------ */


TYPE * N(at) ( N(VECTOR) * restrict this, size_t n) {
	return GET(BEGIN(this), n);
}
TYPE * N(back) ( N(VECTOR) * restrict this) {
	return BACK(this);
}
TYPE * N(begin) ( N(VECTOR) * restrict this) {
	return BEGIN(this);
}
size_t N(capacity) ( N(VECTOR) * restrict this)
{
	return RSRV(this);
}
int N(clear) ( N(VECTOR) * restrict this)
{
	return _realloc(this, 0);
}
bool N(empty) ( N(VECTOR) * restrict this)
{
	return SIZE(this) == 0;
}
TYPE * N(end) ( N(VECTOR) * restrict this)
{
	return END(this);
}
size_t N(max_size) ( N(VECTOR) * restrict this) {
	return CNT_N(this, SIZE_MAX);
}
int N(shrink_to_fix) ( N(VECTOR) * restrict this) {
	return _realloc(this, SIZE(this));
}
size_t N(size) ( N(VECTOR) * restrict this) {
	return SIZE(this);
}

int N(assign_range) ( N(VECTOR) * restrict this, size_t n, const TYPE *val) {
	if ( N(reserve)(this, n) ) {
		return -ENOMEM;
	}
	FOR(this,i) {
		SET(i,val);
	}
	return 0;
}
int N(emplace) ( N(VECTOR) * restrict this, TYPE *pos, TYPE *args)
{
	assert( pos >= BEGIN(this) && pos <= END(this) );
	if ( N(reserve)(this, SIZE(this) + nargs) ) {
		return -ENOMEM;
	}
	if ( !pos ) {
		pos = BEGIN(this);
	}
	DEBUG("%ld %ld %d %d %d\n%ld %ld %d\n", pos, args, nargs, SIZE(this), RSRV(this),
			PNT_ADD(this, pos, nargs), pos, LEN_N(this, END(this) - pos));
	memmove(PNT_ADD(this, pos, nargs), pos, LEN_N(this, END(this) - pos));
	SIZE_SET(this, SIZE(this)+nargs);
	for(; nargs; --nargs, PNT_INC(this,pos), PNT_INC(this,args)) {
		DEBUG("%d \n", *(int*)args);
		SET(pos, args);
	}
	return 0;
}
int N(emplace_back) ( N(VECTOR) * restrict this, TYPE *args)
{
	return N(emplace)(this, END(this), args);
}
TYPE * N(erase) ( N(VECTOR) * restrict this, TYPE *position)
{
	return N(erase_range)(this, position, PNT_ADD(this, position, 1));
}
TYPE * N(erase_range) ( N(VECTOR) * restrict this, TYPE *first, TYPE *last)
{
	assert( BEGIN(this) <= first && first <= END(this) );
	assert( BEGIN(this) <= last && last <= END(this) );
	assert( first < last );
	const size_t len = CNT_N(this, last-first);
	memmove(first, last, len);
	SIZE_SET(this, SIZE(this) - len);
	return PNT_ADD(this,last,1);
}
TYPE * N(insert) ( N(VECTOR) * restrict this, TYPE *position, TYPE *val)
{
	assert( BEGIN(this) <= position && position <= END(this) );
	assert(val);
	const size_t len = 1;
	if ( N(reserve)(this, SIZE(this)+len) ) {
		return NULL;
	}
	memmove(PNT_ADD(this,position,len), position, CNT_N(this, END(this)-position-len));
	SET(position, val);
	SIZE_SET(this, SIZE(this)+len);
	return position;
}
TYPE * N(insert_range) ( N(VECTOR) * restrict this, TYPE *position, TYPE *first, TYPE *last)
{
	const size_t len = CNT_N(this, last - first);
	if ( N(reserve)(this, SIZE(this)+len) ) {
		return NULL;
	}
	memmove(PNT_ADD(this,position,len), position, CNT_N(this, END(this)-position-len));
	memcpy(position, first, len);
	SIZE_SET(this, SIZE(this)+len);
	return position;
}

N(VECTOR) * N(equal) (N(VECTOR) * restrict this,
		N(VECTOR) * restrict x) {
	ASSERT_TYPE(this,x);
	if ( N(reserve)(this, SIZE(x)) ) {
		return NULL;
	}
	return memcpy(BEGIN(this), BEGIN(x), SIZE(x));
}
int N(pop_back) ( N(VECTOR) * restrict this) {
	return N(erase)(this, END(this)) != NULL;
}
int N(push_back) ( N(VECTOR) * restrict this, TYPE *val) {
	return N(emplace)(this, END(this), val, 1);
}
int N(reserve) ( N(VECTOR) * restrict this, size_t n) {
	return RSRV(this) < n ? _realloc(this, n) : 0;
}
int N(resize) ( N(VECTOR) * restrict this, size_t n) {
	if ( N(reserve)(this, n) ) return -ENOMEM;
	SIZE_SET(this, RSRV(this));
	return this;
}
void N(swap) ( N(VECTOR) * restrict this, N(VECTOR) * restrict x) {
	struct VECTOR_s tmp;
	memcpy((void*)&tmp, this, sizeof(*this));
	memcpy((void*)this, x, sizeof(*this));
	memcpy((void*)x, &tmp, sizeof(*this));
}
