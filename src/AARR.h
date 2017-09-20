/*
 * AARR.h
 *
 *  Created on: 21 wrz 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

#ifndef SRC_AARR_H_
#define SRC_AARR_H_

#include <stddef.h> // size_t

#define AARR_FORWARD_DECLARE_TYPE_for_each(TYPE) \
void TYPE ## _for_each(TYPE *arr, size_t size, void (*f)(TYPE *));

#define AARR_DECLARE_TYPE_for_each(TYPE) \
void TYPE ## _for_each(TYPE *arr, size_t size, void (*f)(TYPE *)) \
{ \
	for(size_t i = 0; i < size; ++i) { \
		f(arr++); \
	} \
}

#define AARR_DECLARE_TYPE_is_sorted(TYPE) \
bool TYPE ## _is_sorted(TYPE *arr, size_t size, int (*compar)(const TYPE *, const TYPE *)) \
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


#define AARR_FORWARD_DECLARE_ALL(TYPE) \
		AARR_FORWARD_DECLARE_TYPE_for_each(TYPE)

#define AARR_DECLARE_ALL(TYPE) \
		AARR_DECLARE_TYPE_for_each(TYPE)

AARR_DECLARE_TYPE_FOR_EACH(int)

#include <stdio.h>
void printf_int(int*a)
{
	printf("%d\n", *a);
}
static int main1() {
	int a[] = {1,2,3,4};
	int_for_each(a,4,printf_int);
	return 0;
}

#endif /* SRC_AARR_H_ */
