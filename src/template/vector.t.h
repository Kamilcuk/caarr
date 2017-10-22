/**
 *
 */

#include "priv/config_check.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct VECTOR_s N(VECTOR);

/* Constructors */
N(VECTOR) * N(vector) PAR_SIZE();
N(VECTOR) * N(vector_count) PAR_SIZE(size_t n);
N(VECTOR) * N(vector_fill) PAR_SIZE(size_t n, TYPE *val);
N(VECTOR) * N(vector_copy) PAR_SIZE(N(VECTOR) * restrict other);
N(VECTOR) * N(vector_range) PAR_SIZE(TYPE *first, TYPE *last);
/* Destructor */
void N(vector_desctructor)(N(VECTOR) **restrict this);

/* Member functions */
TYPE * N(at) ( N(VECTOR) * restrict this, size_t n);
TYPE * N(back) ( N(VECTOR) * restrict this);
TYPE * N(begin) ( N(VECTOR) * restrict this);
size_t N(capacity) ( N(VECTOR) * restrict this);
int N(clear) ( N(VECTOR) * restrict this);
int N(emplace) ( N(VECTOR) * restrict this, TYPE *pos, TYPE *args, size_t nargs);
int N(emplace_back) ( N(VECTOR) * restrict this, TYPE *args, size_t nargs);
bool N(empty) ( N(VECTOR) * restrict this);
TYPE * N(end) ( N(VECTOR) * restrict this);
TYPE * N(erase_range) ( N(VECTOR) * restrict this, TYPE *first, TYPE *last);
TYPE * N(erase) ( N(VECTOR) * restrict this, TYPE *position);
size_t N(max_size) ( N(VECTOR) * restrict this);
int N(pop_back) ( N(VECTOR) * restrict this);
int N(push_back) ( N(VECTOR) * restrict this, TYPE *val);
int N(reserve) ( N(VECTOR) * restrict this, size_t n);
int N(resize) ( N(VECTOR) * restrict this, size_t n);
int N(shrink_to_fix) ( N(VECTOR) * restrict this);
size_t N(size) ( N(VECTOR) * restrict this);
void N(swap) ( N(VECTOR) * restrict this, N(VECTOR) * restrict x);

#include "priv/config_check_undef.h"
