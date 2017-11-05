/**
 *
 */

#include "priv/config_check.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct VECTOR_s N(VECTOR);

N(VECTOR) * N(vector) ();
void N(vector_destructor) ( N(VECTOR) ** restrict this );

TYPE * N(at) ( const N(VECTOR) * restrict this, size_t n);
TYPE * N(back) ( const N(VECTOR) * restrict this);
TYPE * N(begin) ( const N(VECTOR) * restrict this);
size_t N(capacity) ( const N(VECTOR) * restrict this);
int N(clear) ( N(VECTOR) * restrict this);
bool N(empty) ( const N(VECTOR) * restrict this);
TYPE * N(end) ( const N(VECTOR) * restrict this);
size_t N(max_size) ( const N(VECTOR) * restrict this);
size_t N(size) ( const N(VECTOR) * restrict this);

int N(assign_range) ( N(VECTOR) * restrict this, const TYPE *first, size_t cnt);
int N(assign_fill) ( N(VECTOR) * restrict this, size_t n, const TYPE *val);
TYPE * N(erase) ( N(VECTOR) * restrict this, TYPE *position);
TYPE * N(erase_range) ( N(VECTOR) * restrict this, TYPE *first, size_t cnt);
TYPE * N(insert_single_element) ( N(VECTOR) * restrict this, TYPE *position, const TYPE *val);
TYPE * N(insert_range) ( N(VECTOR) * restrict this, TYPE *position, const TYPE *first, size_t cnt);
N(VECTOR) * N(operator_equal_copy) ( N(VECTOR) * restrict this, const N(VECTOR) * restrict x);
int N(pop_back) ( N(VECTOR) * restrict this);
int N(push_back) ( N(VECTOR) * restrict this, const TYPE *val);
int N(reserve) ( N(VECTOR) * restrict this, size_t n);
int N(shrink_to_fix) ( N(VECTOR) * restrict this);
int N(resize) ( N(VECTOR) * restrict this, size_t n);
void N(swap) ( N(VECTOR) * restrict this, N(VECTOR) * restrict x);

#include "priv/config_check_undef.h"
