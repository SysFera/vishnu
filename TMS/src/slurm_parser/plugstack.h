
#ifndef _PLUGSTACK_H

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

//#include "job_options.h"
#include "slurmstepd_job.h"


int spank_init_allocator (void);

int spank_init_post_opt (void);

int spank_fini (slurmd_job_t *job);


/*
 *  Option processing
 */

/*
 *  Create a struct option table (suitable for passing to getopt_long())
 *   from SPANK plugin provided options, optionally prepending an existing
 *   table of options `orig_options'  Result must be freed by
 *   spank_option_table_destroy().
 *
 *  If any options in orig_options conflict with internal spank options,
 *   a warning will be printed and the spank option will be disabled.
 *
 */
struct option *spank_option_table_create (const struct option *orig_options);

/*
 *  Free memory associated with an option table created by
 *   spank_p[tion_table_create.
 */
void spank_option_table_destroy (struct option *opt_table);

/*
 *  Process a single spank option which was tagged by `optval' in the
 *   spank option table. If the option takes and argument (i.e. has_arg = 1)
 *   then optarg must be non-NULL.
 *
 *  Returns < 0 if no option is found which matches `optval', or if
 *   the option belongs to a *required* plugin, and the plugin's callback
 *   for that option fails.
 */
int spank_process_option (int optval, const char *optarg);


#endif /* !_PLUGSTACK_H */
