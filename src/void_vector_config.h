/*
 * void_vector_config.h
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

#ifndef SRC_VOID_VECTOR_CONFIG_H_
#define SRC_VOID_VECTOR_CONFIG_H_

# define TYPE                  void
# define TYPE_GET(p,i,size)    (p)+(i)*(size)
# define TYPE_SET(p,v,size)    memcpy(p,v,size)
# define TYPE_CMP(p,v,size)    memcmp(p,v,size)
# define NAMESPACE(name)       VOID ## _ ## name
# define TYPE_VOID

#endif /* SRC_VOID_VECTOR_CONFIG_H_ */
