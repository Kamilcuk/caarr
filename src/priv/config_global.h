/*
 * vector_check_pre.h
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

#ifdef TYPE_VOID
# define _GET(p,i,len)   (TYPE_GET(p,i,len))
# define _SET(p,v,len)   (TYPE_SET(p,v,len))
# define _CMP(p,v,len)   (TYPE_CMP(p,v,len))
#else
# define _GET(p,i,_)     (TYPE_GET(p,i))
# define _SET(p,v,_)     (TYPE_SET(p,v))
# define _CMP(p,v,_)     (TYPE_CMP(p,v))
#endif

#ifdef TYPE_VOID
# define CALL(name, ...)                N(name) (__size, ## __VA_ARGS__)
#else
# define CALL(name, ...)                N(name) (__VA_ARGS__)
#endif
