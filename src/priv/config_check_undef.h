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
#ifdef TYPE_GET
# undef TYPE_GET
#endif
#ifdef TYPE_SET
# undef TYPE_SET
#endif
#ifdef TYPE_CMP
# undef TYPE_CMP
#endif
#ifdef NAMESPACE
# undef NAMESPACE
#endif
#ifdef TYPE_VOID
# undef TYPE_VOID
#endif
#ifdef PAR_SIZE
# undef PAR_SIZE
#endif

#ifndef USE_NMEMB
# undef USE_NMEMB
#endif

#ifdef N
# undef N
#endif

#endif
