
#ifndef _XMALLOC_H
#define _XMALLOC_H

#if HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif

#define xmalloc(__sz) \
	slurm_xmalloc (__sz, __FILE__, __LINE__, __CURRENT_FUNC__)

#define try_xmalloc(__sz) \
	slurm_try_xmalloc(__sz, __FILE__, __LINE__, __CURRENT_FUNC__)

#define xfree(__p) \
	slurm_xfree((void **)&(__p), __FILE__, __LINE__, __CURRENT_FUNC__)

#define xrealloc(__p, __sz) \
        slurm_xrealloc((void **)&(__p), __sz, \
                       __FILE__, __LINE__, __CURRENT_FUNC__)

#define try_xrealloc(__p, __sz) \
	slurm_try_xrealloc((void **)&(__p), __sz, \
                           __FILE__, __LINE__,  __CURRENT_FUNC__)

#define xsize(__p) \
	slurm_xsize((void *)__p, __FILE__, __LINE__, __CURRENT_FUNC__)

void *slurm_xmalloc(size_t, const char *, int, const char *);
void *slurm_try_xmalloc(size_t , const char *, int , const char *);
void slurm_xfree(void **, const char *, int, const char *);
void *slurm_xrealloc(void **, size_t, const char *, int, const char *);
int  slurm_try_xrealloc(void **, size_t, const char *, int, const char *);
int  slurm_xsize(void *, const char *, int, const char *);

#define XMALLOC_MAGIC 0x42

#endif /* !_XMALLOC_H */
