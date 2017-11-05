/*
 * vector_check_config_undef.h
 *
 *  Created on: 21 paź 2017
 *      Author: Kamil Cukrowski <kamilcukrowski __at__ gmail.com>
 *     License: jointly under MIT License and the Beerware License.
 */

#ifndef VECTOR_DONT_UNDEF_CONFIG

#ifdef TYPE
# undef TYPE
#endif
#ifdef NAMESPACE
# undef NAMESPACE
#endif
#ifdef N
# undef N
#endif

#endif
