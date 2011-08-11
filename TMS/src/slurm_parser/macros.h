
#ifndef _MACROS_H
#define _MACROS_H 	1

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#ifndef NULL
#  include <stddef.h>	/* for NULL */
#endif

#include <stdbool.h>
#include <pthread.h>

#include <errno.h>              /* for errno   */
#include "log.h"	/* for error() */

#ifndef FALSE
#  define FALSE	false
#endif

#ifndef TRUE
#  define TRUE	true
#endif

#ifndef MAX
#  define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#  define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

/*
** define __CURRENT_FUNC__ macro for returning current function
*/
#if defined (__GNUC__) && (__GNUC__ < 3)
#  define __CURRENT_FUNC__	__PRETTY_FUNCTION__
#else  /* !__GNUC__ */
#  ifdef _AIX
#    define __CURRENT_FUNC__	__func__
#  else
#    define __CURRENT_FUNC__    ""
#  endif /* _AIX */
#endif /* __GNUC__ */

#ifndef __STRING
#  define __STRING(arg)		#arg
#endif


#endif /* !_MACROS_H */
