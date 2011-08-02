
#ifndef _OPTZ_H
#define _OPTZ_H

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#ifndef   _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#if HAVE_GETOPT_H
#  include <getopt.h>
#else
#  include "getopt.h"
#endif

struct option * optz_create (void);

void optz_destroy (struct option *optz);

int optz_add (struct option **optz, const struct option *opt);

int optz_append (struct option **optz, const struct option *);

#endif /* !_OPTZ_H */
