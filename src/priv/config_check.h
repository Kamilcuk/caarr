/*
 * vector_check_config.h
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

/* Check Configuration ------------------------------------------------------------ */

#ifndef TYPE
# error TYPE not defined
#endif
#ifndef TYPE_GET
# error TYPE_GET not defined
#endif
#ifndef TYPE_SET
# error TYPE_SET not defined
#endif
#ifndef TYPE_CMP
# error TYPE_CMP not defined
#endif
#ifndef NAMESPACE
# warning NAMESPACE not defined
# define NAMESPACE
#endif

/* Namespace -------------------------------------------------------------- */

#ifdef N
# error N is defined
#endif

#define N(name)              NAMESPACE(name)

/* Parameter - add size to void containers ------------------------------------ */

#ifdef PAR_SIZE
# error PAR_SIZE defined
#endif

#ifdef TYPE_VOID
# define PAR_SIZE(...)  (size_t __size, ## __VA_ARGS__)
#else
# define PAR_SIZE(...)  (__VA_ARGS__)
#endif

/* Defaults for eclipse editor ------------------------------------------------ */

#ifndef TYPE
# define TYPE void
# include <stddef.h>
 static TYPE *TYPE_GET(TYPE *p, size_t i, size_t size) { return p; }
 static void TYPE_SET(TYPE *p, const TYPE *v, size_t size) { return p; }
 static int TYPE_CMP(const TYPE *p, const TYPE *v, size_t size) { return p; }
# define NAMESPACE(name) name
# define N(name) name
#endif
