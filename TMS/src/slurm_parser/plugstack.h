
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

#include "job_options.h"
#include "slurmstepd_job.h"

struct spank_launcher_job_info {
	uid_t       uid;
	gid_t       gid;
	uint32_t    jobid;
	uint32_t    stepid;
	slurm_step_layout_t *step_layout;
	int         argc;
	char      **argv;
};

int spank_init (slurmd_job_t *job);

int spank_init_allocator (void);

int spank_init_post_opt (void);

int spank_user (slurmd_job_t *job);

int spank_local_user (struct spank_launcher_job_info *job);

int spank_task_privileged (slurmd_job_t *job, int taskid);

int spank_user_task (slurmd_job_t *job, int taskid);

int spank_task_post_fork (slurmd_job_t *job, int taskid);

int spank_task_exit (slurmd_job_t *job, int taskid);

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

/*
 *  Generate --help style output on stream `fp' for all internal spank
 *   options which were not previously disabled (e.g. due to conflicts
 *   with existing options or other plugins). `width' defines the column
 *   after which the usage text may be displayed, and `left_pad' is the
 *   amount of space to pad on the left before printing the --option.
 */
int spank_print_options (FILE *fp, int width, int left_pad);

/*  Set all registered remote options (i.e. those passed to
 *   spank_process_option) in the job options `options'.
 */
int spank_set_remote_options (job_options_t options);

/*  Set all registered remote options (i.e. those passed to
 *   spank_process_option) in the current environment for later
 *   retreival by spank_get_remote_options_env().
 */
int spank_set_remote_options_env (void);

/*  Register any remote spank options that exist in `options'
 *    to their respective spank plugins. This function ends up invoking
 *    all plugin option callbacks, and will fail (return < 0) if
 *    a *required* plugin callback returns < 0.
 *
 *  A warning is printed if no plugin matches a remote option
 *   in the job_options structure, but the funtion does not return failure.
 */
int spank_get_remote_options (job_options_t options);

/*  Register any remote spank options that exist in the environment `env'
 *    to their respective spank plugins. This function ends up invoking
 *    all plugin option callbacks, and will fail (return < 0) if
 *    a *required* plugin callback returns < 0.
 *
 *  A warning is printed if no plugin matches a remote option
 *   in the job_options structure, but the funtion does not return failure.
 */
int spank_get_remote_options_env (char **env);

#endif /* !_PLUGSTACK_H */
