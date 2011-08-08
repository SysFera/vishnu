

#ifndef _XASSERT_H
#define _XASSERT_H	1


#ifdef NDEBUG

#  define xassert(expr)	((void) (0))

#else /* !NDEBUG */

#  define xassert(__ex)  _STMT_START { \
     (__ex) ? ((void)0) : \
     slurm_xassert_failed(__STRING(__ex), __FILE__,  __LINE__, __CURRENT_FUNC__);\
     } _STMT_END

/*  This prints the assertion failed message to the slurm log facility
 *  (see log.h) and aborts the calling program
 *  (messages go to stderr if log is not initialized)
 */
void slurm_xassert_failed(char *, const char *, int, const char *);

#endif /* NDEBUG. */

#endif /* !__XASSERT_H */
