
#ifndef _PARSE_TIME_H_
#define _PARSE_TIME_H_

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif                        /* HAVE_INTTYPES_H */
#else                           /* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif

#include <time.h>

/* Convert string to equivalent time value
 * input formats:
 *   today or tomorrow
 *   midnight, noon, teatime (4PM)
 *   HH:MM [AM|PM]
 *   MMDDYY or MM/DD/YY or MM.DD.YY
 *   YYYY-MM-DD[THH[:MM[:SS]]]
 *   now + count [minutes | hours | days | weeks]
 *
 * Invalid input results in message to stderr and return value of zero
 */
extern time_t parse_time(char *time_str, int past);

/* Convert a string to an equivalent time value
 * input formats:
 *   min
 *   min:sec
 *   hr:min:sec
 *   days-hr:min:sec
 *   days-hr
 * output:
 *   minutes
 */
extern int time_str2mins(char *string);

/* used to get a 2 char abbriviated month name from int 0-11 */
extern char *mon_abbr(int mon);

#endif
