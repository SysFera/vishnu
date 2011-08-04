
#ifndef __SLURM_PARSER_H__
#define __SLURM_PARSER_H__


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>               /* MAXPATHLEN */
#include <fcntl.h>

#include <slurm/slurm.h>
#include "slurm_internal_types.h"
#include "xstring.h"
#include "xmalloc.h"

#include "opt.h"
#include "mult_cluster.h"

#define MAX_RETRIES 15

static void  _env_merge_filter(job_desc_msg_t *desc);
static int   _fill_job_desc_from_opts(job_desc_msg_t *desc);
static void *_get_script_buffer(const char *filename, int *size);
static char *_script_wrap(char *command_string);
static void  _set_exit_code(void);
static void  _set_prio_process_env(void);
static int   _set_rlimit_env(void);
static void  _set_spank_env(void);
static void  _set_submit_dir_env(void);
static int   _set_umask_env(void);
int slurm_parse_script(int argc, char *argv[], job_desc_msg_t *desc);

#endif /*__SLURM_AUTHENTICATION_H__*/
