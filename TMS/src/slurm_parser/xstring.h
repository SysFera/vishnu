
#ifndef _XSTRING_H
#define _XSTRING_H	1

#include "macros.h"

#define xstrcat(__p, __q)		slurm_xstrcat(&(__p), __q)
#define xstrfmtcat(__p, __fmt, args...)	slurm_xstrfmtcat(&(__p), __fmt, ## args)

/*
** The following functions take a ptr to a string and expand the
** size of that string as needed to append the requested data.
** the macros above are provided to automatically take the
** address of the first argument, thus simplifying the interface
**
** space is allocated with xmalloc/xrealloc, so caller must use
** xfree to free.
**
*/

/*
** cat str2 onto str1, expanding str1 as necessary
*/
void slurm_xstrcat(char **str1, const char *str2);

/*
** concatenate printf-style formatted string onto str
** return value is result from vsnprintf(3)
*/
int slurm_xstrfmtcat(char **str, const char *fmt, ...)
  __attribute__ ((format (printf, 2, 3)));

/*
** strdup which uses xmalloc routines
*/
char *slurm_xstrdup(const char *str);

/*
** strdup formatted which uses xmalloc routines
*/
char *slurm_xstrdup_printf(const char *fmt, ...)
  __attribute__ ((format (printf, 1, 2)));

/*
** strndup which uses xmalloc routines
*/
char *slurm_xstrndup(const char *str, size_t n);

/*
** replacement for libc basename
*/
char *slurm_xbasename(char *path);

/*
 * Return true if all characters in a string are whitespace characters,
 * otherwise return false.  ("str" must be terminated by a null character)
 */
bool slurm_xstring_is_whitespace(const char *str);

#endif /* !_XSTRING_H */
