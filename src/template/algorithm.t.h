/*
 * algorithm.c
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */
#include <assert.h> //
#include <errno.h> // errno codes
#include <limits.h> // INT_MAX
#include <stdbool.h> // bool
#include <stddef.h> // NULL
#include <stdlib.h> // RAND_MAX rand()

#include "priv/config_check.h"

typedef TYPE * InputIterator;
typedef TYPE * OutputIterator;
typedef TYPE * ForwardIterator;
typedef TYPE * BidirectionalIterator1;
typedef TYPE * BidirectionalIterator2;
typedef bool (*UnaryPredicate)(TYPE *);
typedef bool (*BinaryPredicate)(TYPE *);

DECLARE(bool, all_of, InputIterator first, InputIterator last, UnaryPredicate pred)
{
	while (first!=last) {
		if (!pred(first)) return false;
		INC_PRE(first);
	}
	return true;
}

DECLARE(ForwardIterator, adjacent_find, ForwardIterator first, ForwardIterator last, BinaryPredicate pred)
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

DECLARE(bool, any_of, InputIterator first, InputIterator last, UnaryPredicate pred)
{
  while (first!=last) {
    if (pred(first)) return true;
    INC_PRE(first);
  }
  return false;
}

DECLARE(BidirectionalIterator2, copy_backward, BidirectionalIterator1 first,
                                         BidirectionalIterator1 last,
                                         BidirectionalIterator2 result)
{
	while (last!=first) {
		SET(result, last);
		DEC_PRE(result);
		DEC_PRE(last);
	}
	return result;
}

DECLARE(OutputIterator, copy_if, InputIterator first, InputIterator last,
                          OutputIterator result, UnaryPredicate pred)
{
	while (first!=last) {
		if (pred(first)) {
			SET(result, first);
			INC_PRE(result);
		}
		INC_PRE(first);
	}
	return result;
}
