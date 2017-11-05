/*
 * algorithm.c
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "priv/config_check.h"

typedef TYPE * InputIterator;
typedef TYPE * OutputIterator;
typedef TYPE * ForwardIterator;
typedef TYPE * BidirectionalIterator1;
typedef TYPE * BidirectionalIterator2;
typedef bool (*UnaryPredicate)(TYPE *);
typedef bool (*BinaryPredicate)(TYPE *);

bool all_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
	while (first!=last) {
		if (!pred(first)) return false;
		INC_PRE(first);
	}
	return true;
}

ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred)
{
	if (first != last)
	{
		ForwardIterator next = first; INC(next);
		while (next != last) {
			if (pred(first,next))
				return first;
			INC_PRE(first); INC_PRE(next);
		}
	}
	return last;
}

bool any_of(InputIterator first, InputIterator last, UnaryPredicate pred)
{
  while (first!=last) {
    if (pred(first)) return true;
    INC_PRE(first);
  }
  return false;
}

BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
                                         BidirectionalIterator1 last,
                                         BidirectionalIterator2 result)
{
	while (last!=first) {
		*result++ = *last++;
	}
	return result;
}

OutputIterator copy_if (InputIterator first, InputIterator last,
		OutputIterator result, UnaryPredicate pred)
{
	while (first!=last) {
		if (pred(*first)) {
			*result = *first;
			++result;
		}
		++first;
	}
	return result;
}
