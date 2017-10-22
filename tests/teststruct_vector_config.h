/*
 * teststruct_vector_config.h
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

#ifndef TESTS_TESTSTRUCT_VECTOR_CONFIG_H_
#define TESTS_TESTSTRUCT_VECTOR_CONFIG_H_

struct teststruct_s {
	int a;
	int b;
};

# define TYPE             struct teststruct_s
# define TYPE_GET(p,i)    &(p)[(i)]
# define TYPE_SET(p,v)    *(p) = *(v)
# define TYPE_CMP(p,v)    memcmp(p,v,sizeof(TYPE))
# define NAMESPACE(name)  teststruct_s ## _ ## name

#endif /* TESTS_TESTSTRUCT_VECTOR_CONFIG_H_ */
