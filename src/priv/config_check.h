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
#ifndef NAMESPACE
# warning NAMESPACE not defined
# define NAMESPACE(x) x
#endif

/* Namespace -------------------------------------------------------------- */

#ifdef N
# error N is defined
#endif

#define N(name)              NAMESPACE(name)

/* Defaults for eclipse editor ------------------------------------------------ */

#ifndef TYPE
# define TYPE int
# define NAMESPACE(name) name
# define N(name) name
#endif
