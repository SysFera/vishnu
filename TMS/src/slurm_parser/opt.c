
#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <string.h>		/* strcpy, strncasecmp */

#ifdef HAVE_STRINGS_H
#  include <strings.h>
#endif

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#if HAVE_GETOPT_H
#  include <getopt.h>
#else
#  include "getopt.h"
#endif

#ifdef HAVE_LIMITS_H
#  include <limits.h>
#endif

#include <fcntl.h>
#include <stdarg.h>		/* va_start   */
#include <stdio.h>
#include <stdlib.h>		/* getenv     */
#include <ctype.h>		/* isdigit    */
#include <sys/param.h>		/* MAXPATHLEN */
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>

#include "uid.h"
#include "xmalloc.h"
#include "xstring.h"

#include "opt.h"

/* generic OPT_ definitions -- mainly for use with env vars  */
#define OPT_NONE        0x00
#define OPT_INT         0x01
#define OPT_STRING      0x02
#define OPT_DEBUG       0x03
#define OPT_NODES       0x04
#define OPT_BOOL        0x05
#define OPT_CORE        0x06
#define OPT_CONN_TYPE	0x07
#define OPT_DISTRIB	0x08
#define OPT_NO_ROTATE	0x09
#define OPT_GEOMETRY	0x0a
#define OPT_MULTI	0x0b
#define OPT_EXCLUSIVE	0x0c
#define OPT_OVERCOMMIT	0x0d
#define OPT_OPEN_MODE	0x0e
#define OPT_ACCTG_FREQ  0x0f
#define OPT_NO_REQUEUE  0x10
#define OPT_REQUEUE     0x11
#define OPT_CPU_BIND    0x12
#define OPT_MEM_BIND    0x13
#define OPT_WCKEY       0x14
#define OPT_SIGNAL      0x15
#define OPT_GET_USER_ENV  0x16
#define OPT_EXPORT        0x17
#define OPT_CLUSTERS      0x18

/* generic getopt_long flags, integers and *not* valid characters */
#define LONG_OPT_PROPAGATE   0x100
#define LONG_OPT_CPU_BIND    0x101
#define LONG_OPT_MEM_BIND    0x102
#define LONG_OPT_JOBID       0x105
#define LONG_OPT_TMP         0x106
#define LONG_OPT_MEM         0x107
#define LONG_OPT_MINCPU      0x108
#define LONG_OPT_CONT        0x109
#define LONG_OPT_UID         0x10a
#define LONG_OPT_GID         0x10b
#define LONG_OPT_MINSOCKETS  0x10c
#define LONG_OPT_MINCORES    0x10d
#define LONG_OPT_MINTHREADS  0x10e
#define LONG_OPT_CORE	     0x10f
#define LONG_OPT_CONNTYPE    0x110
#define LONG_OPT_EXCLUSIVE   0x111
#define LONG_OPT_BEGIN       0x112
#define LONG_OPT_MAIL_TYPE   0x113
#define LONG_OPT_MAIL_USER   0x114
#define LONG_OPT_NICE        0x115
#define LONG_OPT_NO_REQUEUE  0x116
#define LONG_OPT_COMMENT     0x117
#define LONG_OPT_WRAP        0x118
#define LONG_OPT_REQUEUE     0x119
#define LONG_OPT_NETWORK     0x120
#define LONG_OPT_QOS             0x127
#define LONG_OPT_SOCKETSPERNODE  0x130
#define LONG_OPT_CORESPERSOCKET  0x131
#define LONG_OPT_THREADSPERCORE  0x132
#define LONG_OPT_NTASKSPERNODE   0x136
#define LONG_OPT_NTASKSPERSOCKET 0x137
#define LONG_OPT_NTASKSPERCORE   0x138
#define LONG_OPT_MEM_PER_CPU     0x13a
#define LONG_OPT_HINT            0x13b
#define LONG_OPT_BLRTS_IMAGE     0x140
#define LONG_OPT_LINUX_IMAGE     0x141
#define LONG_OPT_MLOADER_IMAGE   0x142
#define LONG_OPT_RAMDISK_IMAGE   0x143
#define LONG_OPT_REBOOT          0x144
#define LONG_OPT_GET_USER_ENV    0x146
#define LONG_OPT_OPEN_MODE       0x147
#define LONG_OPT_ACCTG_FREQ      0x148
#define LONG_OPT_WCKEY           0x149
#define LONG_OPT_RESERVATION     0x14a
#define LONG_OPT_CHECKPOINT      0x14b
#define LONG_OPT_CHECKPOINT_DIR  0x14c
#define LONG_OPT_SIGNAL          0x14d
#define LONG_OPT_TIME_MIN        0x14e
#define LONG_OPT_GRES            0x14f
#define LONG_OPT_WAIT_ALL_NODES  0x150
#define LONG_OPT_EXPORT          0x151

/*---- global variables, defined in opt.h ----*/
opt_t opt;
int error_exit = 1;

/*---- forward declarations of static functions  ----*/

typedef struct env_vars env_vars_t;


/* Get a decimal integer from arg */
static int  _get_int(const char *arg, const char *what);

/* fill in default options  */
static void _opt_default(void);

/* set options from batch script */
static void _opt_batch_script(const char *file, const void *body, int size);

/* set options from pbs batch script */
static void _opt_pbs_batch_script(const char *file, const void *body, int size);

/* set options based upon env vars  */
static void _opt_env(void);
static void _proc_get_user_env(char *val);

/* verify options sanity  */
static bool _opt_verify(void);

static void _process_env_var(env_vars_t *e, const char *val);

static uint16_t _parse_pbs_mail_type(const char *arg);

static  void _fullpath(char **filename, const char *cwd);
static void _set_options(int argc, char **argv);
static void _set_pbs_options(int argc, char **argv);
static void _parse_pbs_resource_list(char *rl);

/*---[ end forward declarations of static functions ]---------------------*/

/*
 * print error message to stderr with opt.progname prepended
 */
#undef USE_ARGERROR
#if USE_ARGERROR
static void argerror(const char *msg, ...)
  __attribute__ ((format (printf, 1, 2)));
static void argerror(const char *msg, ...)
{
	va_list ap;
	char buf[256];

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);

	fprintf(stderr, "%s: %s\n",
		opt.progname ? opt.progname : "sbatch", buf);
	va_end(ap);
}
#else
#  define argerror error
#endif				/* USE_ARGERROR */

/*
 * If the node list supplied is a file name, translate that into
 *	a list of nodes, we orphan the data pointed to
 * RET true if the node list is a valid one
 */
static bool _valid_node_list(char **node_list_pptr)
{
	int count = NO_VAL;

	/* If we are using Arbitrary and we specified the number of
	   procs to use then we need exactly this many since we are
	   saying, lay it out this way!  Same for max and min nodes.
	   Other than that just read in as many in the hostfile */
	if(opt.ntasks_set)
		count = opt.ntasks;
	else if(opt.nodes_set) {
		if(opt.max_nodes)
			count = opt.max_nodes;
		else if(opt.min_nodes)
			count = opt.min_nodes;
	}

	return verify_node_list2(node_list_pptr, opt.distribution, count);
}

/*
 * _opt_default(): used by initialize_and_process_args to set defaults
 */
static void _opt_default()
{
	char buf[MAXPATHLEN + 1];
	int i;
	uid_t uid = getuid();

	opt.user = uid_to_string(uid);
	if (strcmp(opt.user, "nobody") == 0)
		slurm_fatal("Invalid user id: %u", uid);

	opt.script_argc = 0;
	opt.script_argv = NULL;

	opt.uid = uid;
	opt.gid = getgid();

	if ((getcwd(buf, MAXPATHLEN)) == NULL) {
		slurm_error("getcwd failed: %m");
		exit(error_exit);
	}
	opt.cwd = slurm_xstrdup(buf);

	opt.clusters = NULL;
	opt.progname = NULL;

	opt.ntasks = 1;
	opt.ntasks_set = false;
	opt.cpus_per_task = 1;
	opt.cpus_set = false;
	opt.min_nodes = 1;
	opt.max_nodes = 0;
	opt.nodes_set = false;
	opt.sockets_per_node = NO_VAL; /* requested sockets */
	opt.cores_per_socket = NO_VAL; /* requested cores */
	opt.threads_per_core = NO_VAL; /* requested threads */
	opt.ntasks_per_node      = 0;  /* ntask max limits */
	opt.ntasks_per_socket    = NO_VAL;
	opt.ntasks_per_core      = NO_VAL;
	opt.cpu_bind_type = 0;
	opt.cpu_bind = NULL;
	opt.mem_bind_type = 0;
	opt.mem_bind = NULL;
	opt.time_limit = NO_VAL;
	opt.time_min = NO_VAL;
	opt.partition = NULL;

	opt.job_name = NULL;
	opt.jobid    = NO_VAL;
	opt.jobid_set = false;
	opt.dependency = NULL;
	opt.account  = NULL;
	opt.comment  = NULL;
	opt.qos      = NULL;

	opt.distribution = SLURM_DIST_UNKNOWN;
	opt.plane_size   = NO_VAL;

	opt.shared = (uint16_t)NO_VAL;
	opt.no_kill = false;

	opt.immediate	= false;
	opt.requeue	= NO_VAL;
	opt.overcommit	= false;

	opt.quiet = 0;
	opt.slurm_verbose = 0;
	opt.warn_signal = 0;
	opt.warn_time   = 0;
	opt.wait_all_nodes = (uint16_t) NO_VAL;

	/* constraint default (-1 is no constraint) */
	opt.mincpus	    = -1;
	opt.mem_per_cpu	    = -1;
	opt.realmem	    = -1;
	opt.tmpdisk	    = -1;

	opt.hold	    = false;
	opt.constraints	    = NULL;
	opt.gres	    = NULL;
	opt.contiguous	    = false;
	opt.nodelist	    = NULL;
	opt.exc_nodes	    = NULL;

	for (i=0; i<HIGHEST_DIMENSIONS; i++)
		opt.geometry[i]	    = (uint16_t) NO_VAL;
	opt.reboot          = false;
	opt.no_rotate	    = false;
	opt.conn_type	    = (uint16_t) NO_VAL;

	opt.euid	    = (uid_t) -1;
	opt.egid	    = (gid_t) -1;

	opt.propagate	    = NULL;  /* propagate specific rlimits */

	opt.ifname = slurm_xstrdup("/dev/null");
	opt.ofname = NULL;
	opt.efname = NULL;

	opt.export_env        = NULL;
	opt.get_user_env_time = -1;
	opt.get_user_env_mode = -1;
	opt.acctg_freq        = -1;
	opt.reservation       = NULL;
	opt.wckey             = NULL;

	opt.ckpt_interval = 0;
	opt.ckpt_interval_str = NULL;
	opt.ckpt_dir = slurm_xstrdup(opt.cwd);
}

static void _set_distribution(task_dist_states_t distribution,
			      char **dist, char **lllp_dist)
{
	if (((int)distribution >= 0)
	&&  (distribution != SLURM_DIST_UNKNOWN)) {
		switch(distribution) {
		case SLURM_DIST_CYCLIC:
			*dist      = "cyclic";
			break;
		case SLURM_DIST_BLOCK:
			*dist      = "block";
			break;
		case SLURM_DIST_PLANE:
			*dist      = "plane";
			*lllp_dist = "plane";
			break;
		case SLURM_DIST_ARBITRARY:
			*dist      = "arbitrary";
			break;
		case SLURM_DIST_CYCLIC_CYCLIC:
			*dist      = "cyclic";
			*lllp_dist = "cyclic";
			break;
		case SLURM_DIST_CYCLIC_BLOCK:
			*dist      = "cyclic";
			*lllp_dist = "block";
			break;
		case SLURM_DIST_BLOCK_CYCLIC:
			*dist      = "block";
			*lllp_dist = "cyclic";
			break;
		case SLURM_DIST_BLOCK_BLOCK:
			*dist      = "block";
			*lllp_dist = "block";
			break;
		default:
			slurm_error("unknown dist, type %d", distribution);
			break;
		}
	}
}

/*---[ env var processing ]-----------------------------------------------*/

/*
 * try to use a similar scheme as popt.
 *
 * in order to add a new env var (to be processed like an option):
 *
 * define a new entry into env_vars[], if the option is a simple int
 * or string you may be able to get away with adding a pointer to the
 * option to set. Otherwise, process var based on "type" in _opt_env.
 */
struct env_vars {
	const char *var;
	int type;
	void *arg;
	void *set_flag;
};


env_vars_t env_vars[] = {
  {"SBATCH_ACCOUNT",       OPT_STRING,     &opt.account,       NULL          },
  {"SBATCH_ACCTG_FREQ",    OPT_INT,        &opt.acctg_freq,    NULL          },
  {"SBATCH_BLRTS_IMAGE",   OPT_STRING,     &opt.blrtsimage,    NULL          },
  {"SBATCH_CHECKPOINT",    OPT_STRING,     &opt.ckpt_interval_str, NULL      },
  {"SBATCH_CHECKPOINT_DIR",OPT_STRING,     &opt.ckpt_dir,      NULL          },
  {"SBATCH_CLUSTERS",      OPT_CLUSTERS,   &opt.clusters,      NULL          },
  {"SLURM_CLUSTERS",       OPT_CLUSTERS,   &opt.clusters,      NULL          },
  {"SBATCH_CNLOAD_IMAGE",  OPT_STRING,     &opt.linuximage,    NULL          },
  {"SBATCH_CONN_TYPE",     OPT_CONN_TYPE,  NULL,               NULL          },
  {"SBATCH_CPU_BIND",      OPT_CPU_BIND,   NULL,               NULL          },
  {"SBATCH_DEBUG",         OPT_DEBUG,      NULL,               NULL          },
  {"SBATCH_DISTRIBUTION",  OPT_DISTRIB ,   NULL,               NULL          },
  {"SBATCH_EXCLUSIVE",     OPT_EXCLUSIVE,  NULL,               NULL          },
  {"SBATCH_GEOMETRY",      OPT_GEOMETRY,   NULL,               NULL          },
  {"SBATCH_IMMEDIATE",     OPT_BOOL,       &opt.immediate,     NULL          },
  {"SBATCH_IOLOAD_IMAGE",  OPT_STRING,     &opt.ramdiskimage,  NULL          },
  {"SBATCH_JOBID",         OPT_INT,        &opt.jobid,         NULL          },
  {"SBATCH_JOB_NAME",      OPT_STRING,     &opt.job_name,      NULL          },
  {"SBATCH_LINUX_IMAGE",   OPT_STRING,     &opt.linuximage,    NULL          },
  {"SBATCH_MEM_BIND",      OPT_MEM_BIND,   NULL,               NULL          },
  {"SBATCH_MLOADER_IMAGE", OPT_STRING,     &opt.mloaderimage,  NULL          },
  {"SBATCH_NETWORK",       OPT_STRING,     &opt.network,       NULL          },
  {"SBATCH_NO_REQUEUE",    OPT_NO_REQUEUE, NULL,               NULL          },
  {"SBATCH_NO_ROTATE",     OPT_BOOL,       &opt.no_rotate,     NULL          },
  {"SBATCH_OPEN_MODE",     OPT_OPEN_MODE,  NULL,               NULL          },
  {"SBATCH_OVERCOMMIT",    OPT_OVERCOMMIT, NULL,               NULL          },
  {"SBATCH_PARTITION",     OPT_STRING,     &opt.partition,     NULL          },
  {"SBATCH_QOS",           OPT_STRING,     &opt.qos,           NULL          },
  {"SBATCH_RAMDISK_IMAGE", OPT_STRING,     &opt.ramdiskimage,  NULL          },
  {"SBATCH_REQUEUE",       OPT_REQUEUE,    NULL,               NULL          },
  {"SBATCH_SIGNAL",        OPT_SIGNAL,     NULL,               NULL          },
  {"SBATCH_TIMELIMIT",     OPT_STRING,     &opt.time_limit_str,NULL          },
  {"SBATCH_WAIT_ALL_NODES",OPT_INT,        &opt.wait_all_nodes,NULL          },
  {"SBATCH_WCKEY",         OPT_STRING,     &opt.wckey,         NULL          },
  {"SBATCH_GET_USER_ENV",  OPT_GET_USER_ENV, NULL,             NULL          },
  {"SBATCH_EXPORT",        OPT_STRING,     &opt.export_env,    NULL          },

  {NULL, 0, NULL, NULL}
};


/*
 * _opt_env(): used by initialize_and_process_args to set options via
 *            environment variables. See comments above for how to
 *            extend srun to process different vars
 */
static void _opt_env()
{
	char       *val = NULL;
	env_vars_t *e   = env_vars;

	while (e->var) {
		if ((val = getenv(e->var)) != NULL)
			_process_env_var(e, val);
		e++;
	}
}

static void
_process_env_var(env_vars_t *e, const char *val)
{
	char *end = NULL;

	slurm_debug2("now processing env var %s=%s", e->var, val);

	if (e->set_flag) {
		*((bool *) e->set_flag) = true;
	}

	switch (e->type) {
	case OPT_STRING:
		*((char **) e->arg) = slurm_xstrdup(val);
		break;
	case OPT_INT:
		if (val != NULL) {
			*((int *) e->arg) = (int) strtol(val, &end, 10);
			if (!(end && *end == '\0')) {
				slurm_error("%s=%s invalid. ignoring...",
				      e->var, val);
			}
		}
		break;

	case OPT_BOOL:
		/* A boolean env variable is true if:
		 *  - set, but no argument
		 *  - argument is "yes"
		 *  - argument is a non-zero number
		 */
		if (val == NULL || strcmp(val, "") == 0) {
			*((bool *)e->arg) = true;
		} else if (strcasecmp(val, "yes") == 0) {
			*((bool *)e->arg) = true;
		} else if ((strtol(val, &end, 10) != 0)
			   && end != val) {
			*((bool *)e->arg) = true;
		} else {
			*((bool *)e->arg) = false;
		}
		break;

	case OPT_DEBUG:
		if (val != NULL) {
			opt.slurm_verbose = (int) strtol(val, &end, 10);
			if (!(end && *end == '\0'))
				slurm_error("%s=%s invalid", e->var, val);
		}
		break;

	case OPT_CPU_BIND:
		if (slurm_verify_cpu_bind(val, &opt.cpu_bind,
					  &opt.cpu_bind_type))
			exit(error_exit);
		break;

	case OPT_MEM_BIND:
		if (slurm_verify_mem_bind(val, &opt.mem_bind,
					  &opt.mem_bind_type))
			exit(error_exit);
		break;

	case OPT_DISTRIB:
		opt.distribution = verify_dist_type2(val,
						    &opt.plane_size);
		if (opt.distribution == SLURM_DIST_UNKNOWN)
			slurm_error("distribution type `%s' is invalid", val);
		break;

	case OPT_NODES:
		opt.nodes_set = verify_node_count2( val,
						   &opt.min_nodes,
						   &opt.max_nodes );
		if (opt.nodes_set == false) {
			slurm_error("\"%s=%s\" -- invalid node count.",
			      e->var, val);
      exit(error_exit);
		}
		break;

	case OPT_CONN_TYPE:
    //opt.conn_type = verify_conn_type(val);
		break;

	case OPT_NO_ROTATE:
		opt.no_rotate = true;
		break;

	case OPT_GEOMETRY:
		break;

	case OPT_EXCLUSIVE:
		opt.shared = 0;
		break;

	case OPT_OVERCOMMIT:
		opt.overcommit = true;
		break;

	case OPT_OPEN_MODE:
		if ((val[0] == 'a') || (val[0] == 'A'))
			opt.open_mode = OPEN_MODE_APPEND;
		else if ((val[0] == 't') || (val[0] == 'T'))
			opt.open_mode = OPEN_MODE_TRUNCATE;
		else
			slurm_error("Invalid SBATCH_OPEN_MODE: %s. Ignored", val);
		break;

	case OPT_NO_REQUEUE:
		opt.requeue = 0;
		break;

	case OPT_REQUEUE:
		opt.requeue = 1;
		break;
	case OPT_WCKEY:
		xfree(opt.wckey);
		opt.wckey = slurm_xstrdup(val);
		break;
	case OPT_SIGNAL:
		if (get_signal_opts2((char *)val, &opt.warn_signal,
				    &opt.warn_time)) {
			slurm_error("Invalid signal specification: %s", val);
			exit(error_exit);
		}
		break;
	case OPT_GET_USER_ENV:
		if (val)
			_proc_get_user_env((char *)val);
		else
			opt.get_user_env_time = 0;
		break;
	case OPT_CLUSTERS:
     slurm_addto_char_list(opt.clusters, (char *)val);
		break;
	default:
		/* do nothing */
		break;
	}
}


/*---[ command line option processing ]-----------------------------------*/

static struct option long_options[] = {
	{"account",       required_argument, 0, 'A'},
	{"batch",         no_argument,       0, 'b'}, /* batch option
							 is only here for
							 moab tansition
							 doesn't do anything */
	{"extra-node-slurm_info", required_argument, 0, 'B'},
	{"cpus-per-task", required_argument, 0, 'c'},
	{"constraint",    required_argument, 0, 'C'},
	{"dependency",    required_argument, 0, 'd'},
	{"workdir",       required_argument, 0, 'D'},
	{"error",         required_argument, 0, 'e'},
	{"nodefile",      required_argument, 0, 'F'},
	{"geometry",      required_argument, 0, 'g'},
	{"hold",          no_argument,       0, 'H'}, /* undocumented */
	{"input",         required_argument, 0, 'i'},
	{"immediate",     no_argument,       0, 'I'},
	{"job-name",      required_argument, 0, 'J'},
	{"no-kill",       no_argument,       0, 'k'},
	{"licenses",      required_argument, 0, 'L'},
	{"distribution",  required_argument, 0, 'm'},
	{"cluster",       required_argument, 0, 'M'},
	{"clusters",      required_argument, 0, 'M'},
	{"tasks",         required_argument, 0, 'n'},
	{"ntasks",        required_argument, 0, 'n'},
	{"nodes",         required_argument, 0, 'N'},
	{"output",        required_argument, 0, 'o'},
	{"overcommit",    no_argument,       0, 'O'},
	{"partition",     required_argument, 0, 'p'},
	{"quiet",         no_argument,       0, 'Q'},
	{"no-rotate",     no_argument,       0, 'R'},
	{"share",         no_argument,       0, 's'},
	{"time",          required_argument, 0, 't'},
	{"nodelist",      required_argument, 0, 'w'},
	{"exclude",       required_argument, 0, 'x'},
	{"acctg-freq",    required_argument, 0, LONG_OPT_ACCTG_FREQ},
	{"begin",         required_argument, 0, LONG_OPT_BEGIN},
	{"blrts-image",   required_argument, 0, LONG_OPT_BLRTS_IMAGE},
	{"checkpoint",    required_argument, 0, LONG_OPT_CHECKPOINT},
	{"checkpoint-dir",required_argument, 0, LONG_OPT_CHECKPOINT_DIR},
	{"cnload-image",  required_argument, 0, LONG_OPT_LINUX_IMAGE},
	{"comment",       required_argument, 0, LONG_OPT_COMMENT},
	{"conn-type",     required_argument, 0, LONG_OPT_CONNTYPE},
	{"contiguous",    no_argument,       0, LONG_OPT_CONT},
	{"cores-per-socket", required_argument, 0, LONG_OPT_CORESPERSOCKET},
	{"cpu_bind",      required_argument, 0, LONG_OPT_CPU_BIND},
	{"exclusive",     no_argument,       0, LONG_OPT_EXCLUSIVE},
	{"export",        required_argument, 0, LONG_OPT_EXPORT},
	{"get-user-env",  optional_argument, 0, LONG_OPT_GET_USER_ENV},
	{"gres",          required_argument, 0, LONG_OPT_GRES},
	{"gid",           required_argument, 0, LONG_OPT_GID},
	{"hint",          required_argument, 0, LONG_OPT_HINT},
	{"ioload-image",  required_argument, 0, LONG_OPT_RAMDISK_IMAGE},
	{"jobid",         required_argument, 0, LONG_OPT_JOBID},
	{"linux-image",   required_argument, 0, LONG_OPT_LINUX_IMAGE},
	{"mail-type",     required_argument, 0, LONG_OPT_MAIL_TYPE},
	{"mail-user",     required_argument, 0, LONG_OPT_MAIL_USER},
	{"mem",           required_argument, 0, LONG_OPT_MEM},
	{"mem-per-cpu",   required_argument, 0, LONG_OPT_MEM_PER_CPU},
	{"mem_bind",      required_argument, 0, LONG_OPT_MEM_BIND},
	{"mincores",      required_argument, 0, LONG_OPT_MINCORES},
	{"mincpus",       required_argument, 0, LONG_OPT_MINCPU},
	{"minsockets",    required_argument, 0, LONG_OPT_MINSOCKETS},
	{"minthreads",    required_argument, 0, LONG_OPT_MINTHREADS},
	{"mloader-image", required_argument, 0, LONG_OPT_MLOADER_IMAGE},
	{"network",       required_argument, 0, LONG_OPT_NETWORK},
	{"nice",          optional_argument, 0, LONG_OPT_NICE},
	{"no-requeue",    no_argument,       0, LONG_OPT_NO_REQUEUE},
	{"ntasks-per-core",  required_argument, 0, LONG_OPT_NTASKSPERCORE},
	{"ntasks-per-node",  required_argument, 0, LONG_OPT_NTASKSPERNODE},
	{"ntasks-per-socket",required_argument, 0, LONG_OPT_NTASKSPERSOCKET},
	{"open-mode",     required_argument, 0, LONG_OPT_OPEN_MODE},
	{"propagate",     optional_argument, 0, LONG_OPT_PROPAGATE},
	{"qos",		  required_argument, 0, LONG_OPT_QOS},
	{"ramdisk-image", required_argument, 0, LONG_OPT_RAMDISK_IMAGE},
	{"reboot",        no_argument,       0, LONG_OPT_REBOOT},
	{"requeue",       no_argument,       0, LONG_OPT_REQUEUE},
	{"reservation",   required_argument, 0, LONG_OPT_RESERVATION},
	{"signal",        required_argument, 0, LONG_OPT_SIGNAL},
	{"sockets-per-node", required_argument, 0, LONG_OPT_SOCKETSPERNODE},
	{"tasks-per-node",required_argument, 0, LONG_OPT_NTASKSPERNODE},
	{"time-min",      required_argument, 0, LONG_OPT_TIME_MIN},
	{"threads-per-core", required_argument, 0, LONG_OPT_THREADSPERCORE},
	{"tmp",           required_argument, 0, LONG_OPT_TMP},
	{"uid",           required_argument, 0, LONG_OPT_UID},
	{"wait-all-nodes",required_argument, 0, LONG_OPT_WAIT_ALL_NODES},
	{"wckey",         required_argument, 0, LONG_OPT_WCKEY},
	{"wrap",          required_argument, 0, LONG_OPT_WRAP},
	{NULL,            0,                 0, 0}
};

static char *opt_string =
	"+bA:B:c:C:d:D:e:F:g:hHi:IJ:kL:m:M:n:N:o:Op:P:QRst:uU:vVw:x:";


/*
 * process_options_first_pass()
 *
 * In this first pass we only look at the command line options, and we
 * will only handle a few options (help, usage, quiet, slurm_verbose, version),
 * and look for the script name and arguments (if provided).
 *
 * We will parse the environment variable options, batch script options,
 * and all of the rest of the command line options in
 * process_options_second_pass().
 *
 * Return a pointer to the batch script file name is provided on the command
 * line, otherwise return NULL, and the script will need to be read from
 * standard input.
 */
char *process_options_first_pass(int argc, char **argv)
{
	int opt_char, option_index = 0;
	char *str = NULL;

	/* initialize option defaults */
	_opt_default();

	opt.progname = slurm_xbasename(argv[0]);
	optind = 0;

	while((opt_char = getopt_long(argc, argv, opt_string,
				      long_options, &option_index)) != -1) {
    switch (opt_char) {
      case '?':
        fprintf(stderr, "Consult \"SLURM manual reference \" for more "
            "slurm_information\n");
        exit(error_exit);
        break;
      case 'h':
        break;	
      case 'Q':
        break;
      case 'u':
        break;
      case 'v':
        break;
      case 'V':
        break;
      case LONG_OPT_WRAP:
        break;
      default:
        /* will be parsed in second pass function */
        break;
    }
  }
	xfree(str);

	if (argc > optind) {
		int i;
		char **leftover;

		opt.script_argc = argc - optind;
		leftover = argv + optind;
		opt.script_argv = (char **) xmalloc((opt.script_argc + 1)
						    * sizeof(char *));
		for (i = 0; i < opt.script_argc; i++)
			opt.script_argv[i] = slurm_xstrdup(leftover[i]);
		opt.script_argv[i] = NULL;
	}
	if (opt.script_argc > 0) {
		char *fullpath;
		char *cmd       = opt.script_argv[0];
		int  mode       = R_OK;

		if ((fullpath = search_path2(opt.cwd, cmd, true, mode))) {
			xfree(opt.script_argv[0]);
			opt.script_argv[0] = fullpath;
		}

		return opt.script_argv[0];
	} else {
		return NULL;
	}
}

/* process options:
 * 1. update options with option set in the script
 * 2. update options with env vars
 * 3. update options with commandline args
 * 4. perform some verification that options are reasonable
 */
int process_options_second_pass(int argc, char *argv[], const char *file,
				const void *script_body, int script_size)
{
	/* set options from batch script */
	_opt_batch_script(file, script_body, script_size);

	/* set options from pbs batch script */
	_opt_pbs_batch_script(file, script_body, script_size);

	/* set options from env vars */
	_opt_env();

	/* set options from command line */
	_set_options(argc, argv);

	if (!_opt_verify())
		exit(error_exit);

	return 1;

}

/*
 * _next_line - Interpret the contents of a byte buffer as characters in
 *	a file.  _next_line will find and return the next line in the buffer.
 *
 *	If "state" is NULL, it will start at the beginning of the buffer.
 *	_next_line will update the "state" pointer to point at the
 *	spot in the buffer where it left off.
 *
 * IN buf - buffer containing file contents
 * IN size - size of buffer "buf"
 * IN/OUT state - used by _next_line to determine where the last line ended
 *
 * RET - xmalloc'ed character string, or NULL if no lines remaining in buf.
 */
static char *_next_line(const void *buf, int size, void **state)
{
	char *line;
	char *current, *ptr;

	if (*state == NULL) /* initial state */
		*state = (void *)buf;

	if ((*state - buf) >= size) /* final state */
		return NULL;

	ptr = current = (char *)*state;
	while ((*ptr != '\n') && (ptr < ((char *)buf+size)))
		ptr++;

	line = slurm_xstrndup(current, (ptr-current));

	/*
	 *  Advance state past newline
	 */
	*state = (ptr < ((char *) buf + size)) ? ptr+1 : ptr;
	return line;
}

/*
 * _get_argument - scans a line for something that looks like a command line
 *	argument, and return an xmalloc'ed string containing the argument.
 *	Quotes can be used to group characters, including whitespace.
 *	Quotes can be included in an argument be escaping the quotes,
 *	preceding the quote with a backslash (\").
 *
 * IN - line
 * OUT - skipped - number of characters parsed from line
 * RET - xmalloc'ed argument string (may be shorter than "skipped")
 *       or NULL if no arguments remaining
 */
static char *
_get_argument(const char *file, int lineno, const char *line, int *skipped)
{
	const char *ptr;
	char argument[BUFSIZ];
	char q_char = '\0';
	bool escape_flag = false;
	bool quoted = false;
	int i;

	ptr = line;
	*skipped = 0;

	/* skip whitespace */
	while (isspace(*ptr) && *ptr != '\0') {
		ptr++;
	}

	if (*ptr == '\0')
		return NULL;

	/* copy argument into "argument" buffer, */
	i = 0;
	while ((quoted || !isspace(*ptr)) && *ptr != '\n' && *ptr != '\0') {

		if (escape_flag) {
			escape_flag = false;
		} else if (*ptr == '\\') {
			escape_flag = true;
			ptr++;
			continue;
		} else if (quoted) {
			if (*ptr == q_char) {
				quoted = false;
				ptr++;
				continue;
			}
		} else if (*ptr == '"' || *ptr == '\'') {
			quoted = true;
			q_char = *(ptr++);
			continue;
		} else if (*ptr == '#') {
			/* found an un-escaped #, rest of line is a comment */
			break;
		}

		argument[i++] = *(ptr++);
	}
	argument[i] = '\0';

	if (quoted) /* Unmatched quote */
		slurm_fatal("%s: line %d: Unmatched `%c` in [%s]",
		      file, lineno, q_char, line);

	*skipped = ptr - line;

	return (i > 0 ? slurm_xstrdup (argument) : NULL);
}

/*
 * set options from batch script
 *
 * Build an argv-style array of options from the script "body",
 * then pass the array to _set_options for() further parsing.
 */
static void _opt_batch_script(const char * file, const void *body, int size)
{
	char *magic_word1 = "#SBATCH";
	char *magic_word2 = "#SLURM";
	int magic_word_len1, magic_word_len2;
	int argc;
	char **argv;
	void *state = NULL;
	char *line;
	char *option;
	char *ptr;
	int skipped = 0, warned = 0, lineno = 0;
	int i;

	magic_word_len1 = strlen(magic_word1);
	magic_word_len2 = strlen(magic_word2);

	/* getopt_long skips over the first argument, so fill it in */
	argc = 1;
	argv = xmalloc(sizeof(char *));
	argv[0] = "sbatch";

	while((line = _next_line(body, size, &state)) != NULL) {
		lineno++;
		if (!strncmp(line, magic_word1, magic_word_len1))
			ptr = line + magic_word_len1;
		else if (!strncmp(line, magic_word2, magic_word_len2)) {
			ptr = line + magic_word_len2;
			if (!warned) {
				slurm_error("Change from #SLURM to #SBATCH in your "
				      "script and verify the options are "
				      "valid in SLURM sbatch");
				warned = 1;
			}
		} else {
			xfree(line);
			continue;
		}

		/* this line starts with the magic word */
		while ((option = _get_argument(file, lineno, ptr, &skipped))) {
			slurm_debug2("Found in script, argument \"%s\"", option);
			argc += 1;
			xrealloc(argv, sizeof(char*) * argc);
			argv[argc-1] = option;
			ptr += skipped;
		}
		xfree(line);
	}

	if (argc > 0)
		_set_options(argc, argv);

	for (i = 1; i < argc; i++)
		xfree(argv[i]);
	xfree(argv);
}

/*
 * set pbs options from batch script
 *
 * Build an argv-style array of options from the script "body",
 * then pass the array to _set_options for() further parsing.
 */
static void _opt_pbs_batch_script(const char *file, const void *body, int size)
{
	char *magic_word = "#PBS";
	int magic_word_len;
	int argc;
	char **argv;
	void *state = NULL;
	char *line;
	char *option;
	char *ptr;
	int skipped = 0;
	int lineno = 0;
	int i;

	magic_word_len = strlen(magic_word);
	/* getopt_long skips over the first argument, so fill it in */
	argc = 1;
	argv = xmalloc(sizeof(char *));
	argv[0] = "sbatch";

	while((line = _next_line(body, size, &state)) != NULL) {
		lineno++;
		if (strncmp(line, magic_word, magic_word_len) != 0) {
			xfree(line);
			continue;
		}

		/* this line starts with the magic word */
		ptr = line + magic_word_len;
		while ((option = _get_argument(file, lineno, ptr, &skipped))) {
			slurm_debug2("Found in script, argument \"%s\"", option);
			argc += 1;
			xrealloc(argv, sizeof(char*) * argc);
			argv[argc-1] = option;
			ptr += skipped;
		}
		xfree(line);
	}

	if (argc > 0)
		_set_pbs_options(argc, argv);

	for (i = 1; i < argc; i++)
		xfree(argv[i]);
	xfree(argv);
}

static void _set_options(int argc, char **argv)
{
	int opt_char, option_index = 0, max_val = 0;
	char *tmp;

	optind = 0;
	while((opt_char = getopt_long(argc, argv, opt_string,
				      long_options, &option_index)) != -1) {
		switch (opt_char) {
		case '?':
			slurm_error("Consult \"SLURM manual reference\" for more slurm_information");
			exit(error_exit);
			break;
		case 'A':
		case 'U':	/* backwards compatibility */
			xfree(opt.account);
			opt.account = slurm_xstrdup(optarg);
			break;
		case 'b':
			/* Only here for Moab transition not suppose
			   to do anything */
			break;
		case 'B':
			opt.extra_set = verify_socket_core_thread_count2(
						optarg,
						&opt.sockets_per_node,
						&opt.cores_per_socket,
						&opt.threads_per_core,
						&opt.cpu_bind_type);

			if (opt.extra_set == false) {
				slurm_error("invalid resource allocation -B `%s'",
					optarg);
				exit(error_exit);
			}
			break;
		case 'c':
			opt.cpus_set = true;
			opt.cpus_per_task =
				_get_int(optarg, "cpus-per-task");
			break;
		case 'C':
			xfree(opt.constraints);
			opt.constraints = slurm_xstrdup(optarg);
			break;
		case 'd':
			xfree(opt.dependency);
			opt.dependency = slurm_xstrdup(optarg);
			break;
		case 'D':
			xfree(opt.cwd);
			opt.cwd = slurm_xstrdup(optarg);
			break;
		case 'e':
			xfree(opt.efname);
			if (strncasecmp(optarg, "none", (size_t)4) == 0)
				opt.efname = slurm_xstrdup("/dev/null");
			else
				opt.efname = slurm_xstrdup(optarg);
			break;
		case 'F':
			xfree(opt.nodelist);
			tmp = slurm_read_hostfile(optarg, 0);
			if (tmp != NULL) {
				opt.nodelist = slurm_xstrdup(tmp);
				free(tmp);
			} else {
				slurm_error("\"%s\" is not a valid node file",
				      optarg);
				exit(error_exit);
			}
			break;
      case 'g':
			break;
    case 'h':
      break;
    case 'H':
			opt.hold = true;
			break;
		case 'i':
			xfree(opt.ifname);
			if (strncasecmp(optarg, "none", (size_t)4) == 0)
				opt.ifname = slurm_xstrdup("/dev/null");
			else
				opt.ifname = slurm_xstrdup(optarg);
			break;
		case 'I':
			opt.immediate = true;
			break;
		case 'J':
			xfree(opt.job_name);
			opt.job_name = slurm_xstrdup(optarg);
			break;
		case 'k':
			opt.no_kill = true;
			break;
		case 'L':
			xfree(opt.licenses);
			opt.licenses = slurm_xstrdup(optarg);
			break;
		case 'm':
			opt.distribution = verify_dist_type2(optarg,
							    &opt.plane_size);
			if (opt.distribution == SLURM_DIST_UNKNOWN) {
				slurm_error("distribution type `%s' "
				      "is not recognized", optarg);
				exit(error_exit);
			}
			break;
		case 'M':
			if (opt.clusters)
				slurm_list_destroy(opt.clusters);
        slurm_addto_char_list(opt.clusters, optarg);
			break;
		case 'n':
			opt.ntasks_set = true;
			opt.ntasks =
				_get_int(optarg, "number of tasks");
			break;
		case 'N':
			opt.nodes_set =
				verify_node_count2(optarg,
						  &opt.min_nodes,
						  &opt.max_nodes);
			if (opt.nodes_set == false) {
				slurm_error("invalid node count `%s'",
				      optarg);
				exit(error_exit);
			}
			break;
		case 'o':
			xfree(opt.ofname);
			if (strncasecmp(optarg, "none", (size_t)4) == 0)
				opt.ofname = slurm_xstrdup("/dev/null");
			else
				opt.ofname = slurm_xstrdup(optarg);
			break;
		case 'O':
			opt.overcommit = true;
			break;
		case 'p':
			xfree(opt.partition);
			opt.partition = slurm_xstrdup(optarg);
			break;
		case 'P':
			slurm_verbose("-P option is deprecated, use -d instead");
			xfree(opt.dependency);
			opt.dependency = slurm_xstrdup(optarg);
			break;
		case 'Q':
			opt.quiet++;
			break;
		case 'R':
			opt.no_rotate = true;
			break;
		case 's':
			opt.shared = 1;
			break;
		case 't':
			xfree(opt.time_limit_str);
			opt.time_limit_str = slurm_xstrdup(optarg);
			break;
    case 'u':
      break;
    case 'v':
      break;
    case 'V':
      break;
		case 'w':
			xfree(opt.nodelist);
			opt.nodelist = slurm_xstrdup(optarg);
#ifdef HAVE_BG
			slurm_info("\tThe nodelist option should only be used if\n"
			     "\tthe block you are asking for can be created.\n"
			     "\tIt should also include all the midplanes you\n"
			     "\twant to use, partial lists may not\n"
			     "\twork correctly.\n"
			     "\tPlease consult smap before using this option\n"
			     "\tor your job may be stuck with no way to run.");
#endif
			break;
		case 'x':
			xfree(opt.exc_nodes);
			opt.exc_nodes = slurm_xstrdup(optarg);
			if (!_valid_node_list(&opt.exc_nodes))
				exit(error_exit);
			break;
    case LONG_OPT_CONT:
      opt.contiguous = true;
      break;
    case LONG_OPT_EXCLUSIVE:
      opt.shared = 0;
      break;
    case LONG_OPT_CPU_BIND:
      if (slurm_verify_cpu_bind(optarg, &opt.cpu_bind,
            &opt.cpu_bind_type))
        exit(error_exit);
      break;
    case LONG_OPT_MEM_BIND:
			if (slurm_verify_mem_bind(optarg, &opt.mem_bind,
						  &opt.mem_bind_type))
				exit(error_exit);
			break;
		case LONG_OPT_MINCPU:
			opt.mincpus = _get_int(optarg, "mincpus");
			if (opt.mincpus < 0) {
				slurm_error("invalid mincpus constraint %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MINCORES:
			slurm_verbose("mincores option has been deprecated, use "
				"cores-per-socket");
			opt.cores_per_socket = _get_int(optarg, "mincores");
			if (opt.cores_per_socket < 0) {
				slurm_error("invalid mincores constraint %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MINSOCKETS:
			slurm_verbose("minsockets option has been deprecated, use "
				"sockets-per-node");
			opt.sockets_per_node = _get_int(optarg, "minsockets");
			if (opt.sockets_per_node < 0) {
				slurm_error("invalid minsockets constraint %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MINTHREADS:
			slurm_verbose("minthreads option has been deprecated, use "
				"threads-per-core");
			opt.threads_per_core = _get_int(optarg, "minthreads");
			if (opt.threads_per_core < 0) {
				slurm_error("invalid minthreads constraint %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MEM:
			opt.realmem = (int) str_to_mbytes2(optarg);
			if (opt.realmem < 0) {
				slurm_error("invalid memory constraint %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MEM_PER_CPU:
			opt.mem_per_cpu = (int) str_to_mbytes2(optarg);
			if (opt.mem_per_cpu < 0) {
				slurm_error("invalid memory constraint %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_TMP:
			opt.tmpdisk = str_to_mbytes2(optarg);
			if (opt.tmpdisk < 0) {
				slurm_error("invalid tmp value %s", optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_JOBID:
			opt.jobid = _get_int(optarg, "jobid");
			opt.jobid_set = true;
			break;
		case LONG_OPT_UID:
			if (opt.euid != (uid_t) -1) {
				slurm_error("duplicate --uid option");
				exit(error_exit);
			}
			if (uid_from_string (optarg, &opt.euid) < 0) {
				slurm_error("--uid=\"%s\" invalid", optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_GID:
			if (opt.egid != (gid_t) -1) {
				slurm_error("duplicate --gid option");
				exit(error_exit);
			}
			if (gid_from_string (optarg, &opt.egid) < 0) {
				slurm_error("--gid=\"%s\" invalid", optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_CONNTYPE:
			break;
		case LONG_OPT_BEGIN:
			opt.begin = parse_time2(optarg, 0);
			if (opt.begin == 0) {
				slurm_error("Invalid time specification %s", optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MAIL_TYPE:
			opt.mail_type |= parse_mail_type2(optarg);
			if (opt.mail_type == 0) {
				slurm_error("--mail-type=%s invalid", optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_MAIL_USER:
			xfree(opt.mail_user);
			opt.mail_user = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_NICE:
			if (optarg)
				opt.nice = strtol(optarg, NULL, 10);
			else
				opt.nice = 100;
			if (abs(opt.nice) > NICE_OFFSET) {
				slurm_error("Invalid nice value, must be between "
				      "-%d and %d", NICE_OFFSET, NICE_OFFSET);
				exit(error_exit);
			}
			if (opt.nice < 0) {
				uid_t my_uid = getuid();
				if ((my_uid != 0) &&
				    (my_uid != slurm_get_slurm_user_id())) {
					slurm_error("Nice value must be "
					      "non-negative, value ignored");
					opt.nice = 0;
				}
			}
			break;
		case LONG_OPT_NO_REQUEUE:
			opt.requeue = 0;
			break;
		case LONG_OPT_REQUEUE:
			opt.requeue = 1;
			break;
		case LONG_OPT_COMMENT:
			xfree(opt.comment);
			opt.comment = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_QOS:
			xfree(opt.qos);
			opt.qos = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_SOCKETSPERNODE:
			max_val = 0;
			get_resource_arg_range2( optarg, "sockets-per-node",
						&opt.sockets_per_node,
						&max_val, true );
			if ((opt.sockets_per_node == 1) &&
			    (max_val == INT_MAX))
				opt.sockets_per_node = NO_VAL;
			break;
		case LONG_OPT_CORESPERSOCKET:
			max_val = 0;
			get_resource_arg_range2( optarg, "cores-per-socket",
						&opt.cores_per_socket,
						&max_val, true );
			if ((opt.cores_per_socket == 1) &&
			    (max_val == INT_MAX))
				opt.cores_per_socket = NO_VAL;
			break;
		case LONG_OPT_THREADSPERCORE:
			max_val = 0;
			get_resource_arg_range2( optarg, "threads-per-core",
						&opt.threads_per_core,
						&max_val, true );
			if ((opt.threads_per_core == 1) &&
			    (max_val == INT_MAX))
				opt.threads_per_core = NO_VAL;
			break;
		case LONG_OPT_NTASKSPERNODE:
			opt.ntasks_per_node = _get_int(optarg,
				"ntasks-per-node");
			setenvf(NULL, "SLURM_NTASKS_PER_NODE", "%d",
				opt.ntasks_per_node);
			break;
		case LONG_OPT_NTASKSPERSOCKET:
			opt.ntasks_per_socket = _get_int(optarg,
				"ntasks-per-socket");
			setenvf(NULL, "SLURM_NTASKS_PER_SOCKET", "%d",
				opt.ntasks_per_socket);
			break;
		case LONG_OPT_NTASKSPERCORE:
			opt.ntasks_per_core = _get_int(optarg,
				"ntasks-per-core");
			setenvf(NULL, "SLURM_NTASKS_PER_CORE", "%d",
				opt.ntasks_per_socket);
			break;
		case LONG_OPT_HINT:
			/* Keep after other options filled in */
			if (verify_hint2(optarg,
					&opt.sockets_per_node,
					&opt.cores_per_socket,
					&opt.threads_per_core,
					&opt.ntasks_per_core,
					&opt.cpu_bind_type)) {
				exit(error_exit);
			}
			break;
		case LONG_OPT_BLRTS_IMAGE:
			xfree(opt.blrtsimage);
			opt.blrtsimage = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_LINUX_IMAGE:
			xfree(opt.linuximage);
			opt.linuximage = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_MLOADER_IMAGE:
			xfree(opt.mloaderimage);
			opt.mloaderimage = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_RAMDISK_IMAGE:
			xfree(opt.ramdiskimage);
			opt.ramdiskimage = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_REBOOT:
			opt.reboot = true;
			break;
		case LONG_OPT_WRAP:
			break;
		case LONG_OPT_GET_USER_ENV:
			if (optarg)
				_proc_get_user_env(optarg);
			else
				opt.get_user_env_time = 0;
			break;
		case LONG_OPT_OPEN_MODE:
			if ((optarg[0] == 'a') || (optarg[0] == 'A'))
				opt.open_mode = OPEN_MODE_APPEND;
			else if ((optarg[0] == 't') || (optarg[0] == 'T'))
				opt.open_mode = OPEN_MODE_TRUNCATE;
			else {
				slurm_error("Invalid --open-mode argument: %s. "
				      "Ignored", optarg);
			}
			break;
		case LONG_OPT_ACCTG_FREQ:
			opt.acctg_freq = _get_int(optarg, "acctg-freq");
			break;
		case LONG_OPT_PROPAGATE:
			xfree(opt.propagate);
			if (optarg)
				opt.propagate = slurm_xstrdup(optarg);
			else
				opt.propagate = slurm_xstrdup("ALL");
			break;
		case LONG_OPT_NETWORK:
			xfree(opt.network);
			opt.network = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_WCKEY:
			xfree(opt.wckey);
			opt.wckey = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_RESERVATION:
			xfree(opt.reservation);
			opt.reservation = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_CHECKPOINT:
			xfree(opt.ckpt_interval_str);
			opt.ckpt_interval_str = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_CHECKPOINT_DIR:
			xfree(opt.ckpt_dir);
			opt.ckpt_dir = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_SIGNAL:
			if (get_signal_opts2(optarg, &opt.warn_signal,
					    &opt.warn_time)) {
				slurm_error("Invalid signal specification: %s",
				      optarg);
				exit(error_exit);
			}
			break;
		case LONG_OPT_TIME_MIN:
			xfree(opt.time_min_str);
			opt.time_min_str = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_GRES:
			if (!strcasecmp(optarg, "help") ||
			    !strcasecmp(optarg, "list")) {
			 break;
      }
			xfree(opt.gres);
			opt.gres = slurm_xstrdup(optarg);
			break;
		case LONG_OPT_WAIT_ALL_NODES:
			opt.wait_all_nodes = strtol(optarg, NULL, 10);
			break;
		case LONG_OPT_EXPORT:
			xfree(opt.export_env);
			opt.export_env = slurm_xstrdup(optarg);
			break;
		default:
      break;
		}
	}

	if (optind < argc) {
		slurm_error("Invalid argument: %s", argv[optind]);
		exit(error_exit);
	}

}

static void _proc_get_user_env(char *optarg)
{
	char *end_ptr;

	if ((optarg[0] >= '0') && (optarg[0] <= '9'))
		opt.get_user_env_time = strtol(optarg, &end_ptr, 10);
	else {
		opt.get_user_env_time = 0;
		end_ptr = optarg;
	}

	if ((end_ptr == NULL) || (end_ptr[0] == '\0'))
		return;
	if      ((end_ptr[0] == 's') || (end_ptr[0] == 'S'))
		opt.get_user_env_mode = 1;
	else if ((end_ptr[0] == 'l') || (end_ptr[0] == 'L'))
		opt.get_user_env_mode = 2;
}

static void _set_pbs_options(int argc, char **argv)
{
	int opt_char, option_index = 0;

	char *pbs_opt_string = "+a:A:c:C:e:hIj:k:l:m:M:N:o:p:q:r:S:u:v:VWz";

	struct option pbs_long_options[] = {
		{"start_time", required_argument, 0, 'a'},
		{"account", required_argument, 0, 'A'},
		{"checkpoint", required_argument, 0, 'c'},
		{"working_dir", required_argument, 0, 'C'},
		{"error", required_argument, 0, 'e'},
		{"hold", no_argument, 0, 'h'},
		{"interactive", no_argument, 0, 'I'},
		{"join", optional_argument, 0, 'j'},
		{"keep", required_argument, 0, 'k'},
		{"resource_list", required_argument, 0, 'l'},
		{"mail_options", required_argument, 0, 'm'},
		{"mail_user_list", required_argument, 0, 'M'},
		{"job_name", required_argument, 0, 'N'},
		{"out", required_argument, 0, 'o'},
		{"priority", required_argument, 0, 'p'},
		{"destination", required_argument, 0, 'q'},
		{"rerunable", required_argument, 0, 'r'},
		{"script_path", required_argument, 0, 'S'},
		{"running_user", required_argument, 0, 'u'},
		{"variable_list", required_argument, 0, 'v'},
		{"all_env", no_argument, 0, 'V'},
		{"attributes", no_argument, 0, 'W'},
		{"no_std", no_argument, 0, 'z'},
		{NULL, 0, 0, 0}
	};


	optind = 0;
	while((opt_char = getopt_long(argc, argv, pbs_opt_string,
				      pbs_long_options, &option_index))
	      != -1) {
		switch (opt_char) {
		case 'a':
			opt.begin = parse_time2(optarg, 0);
			break;
		case 'A':
			xfree(opt.account);
			opt.account = slurm_xstrdup(optarg);
			break;
		case 'c':
			break;
		case 'C':
			xfree(opt.cwd);
			opt.cwd = slurm_xstrdup(optarg);
			break;
		case 'e':
			xfree(opt.efname);
			if (strncasecmp(optarg, "none", (size_t) 4) == 0)
				opt.efname = slurm_xstrdup("/dev/null");
			else
				opt.efname = slurm_xstrdup(optarg);
			break;
		case 'h':
			opt.hold = true;
			break;
		case 'I':
			break;
		case 'j':
			break;
		case 'k':
			break;
		case 'l':
			_parse_pbs_resource_list(optarg);
			break;
		case 'm':
			opt.mail_type |= _parse_pbs_mail_type(optarg);
			if ((opt.mail_type == 0) && strcasecmp(optarg, "n")) {
				slurm_error("-m=%s invalid", optarg);
				exit(error_exit);
			}
			break;
		case 'M':
			xfree(opt.mail_user);
			opt.mail_user = slurm_xstrdup(optarg);
			break;
		case 'N':
			xfree(opt.job_name);
			opt.job_name = slurm_xstrdup(optarg);
			break;
		case 'o':
			xfree(opt.ofname);
			if (strncasecmp(optarg, "none", (size_t) 4) == 0)
				opt.ofname = slurm_xstrdup("/dev/null");
			else
				opt.ofname = slurm_xstrdup(optarg);
			break;
		case 'p':
			if (optarg)
				opt.nice = strtol(optarg, NULL, 10);
			else
				opt.nice = 100;
			if (abs(opt.nice) > NICE_OFFSET) {
				slurm_error("Invalid nice value, must be between "
				      "-%d and %d", NICE_OFFSET, NICE_OFFSET);
				exit(error_exit);
			}
			break;
		case 'q':
			xfree(opt.partition);
			opt.partition = slurm_xstrdup(optarg);
			break;
		case 'r':
			break;
		case 'S':
			break;
		case 'u':
			break;
		case 'v':
			break;
		case 'V':
			break;
		case 'W':
			xfree(opt.constraints);
			opt.constraints = slurm_xstrdup(optarg);
			break;
		case 'z':
			break;
		default:
			slurm_error("Unrecognized command line parameter %c",
			      opt_char);
			exit(error_exit);
		}
	}

	if (optind < argc) {
		slurm_error("Invalid argument: %s", argv[optind]);
		exit(error_exit);
	}
}

static char *_get_pbs_node_name(char *node_options, int *i)
{
	int start = (*i);
	char *value = NULL;

	while(node_options[*i]
	      && node_options[*i] != '+'
	      && node_options[*i] != ':')
		(*i)++;

	value = xmalloc((*i)-start+1);
	memcpy(value, node_options+start, (*i)-start);

	if(node_options[*i])
		(*i)++;

	return value;
}

static void _get_next_pbs_node_part(char *node_options, int *i)
{
	while(node_options[*i]
	      && node_options[*i] != '+'
	      && node_options[*i] != ':')
		(*i)++;
	if(node_options[*i])
		(*i)++;
}

static void _parse_pbs_nodes_opts(char *node_opts)
{
	int i = 0;
	char *temp = NULL;
	int ppn = 0;
	int node_cnt = 0;
	hostlist_t hl = slurm_hostlist_create(NULL);

	while(node_opts[i]) {
		if(!strncmp(node_opts+i, "ppn=", 4)) {
			i+=4;
			ppn += strtol(node_opts+i, NULL, 10);
			_get_next_pbs_node_part(node_opts, &i);
		} else if(isdigit(node_opts[i])) {
			node_cnt += strtol(node_opts+i, NULL, 10);
			_get_next_pbs_node_part(node_opts, &i);
		} else if(isalpha(node_opts[i])) {
			temp = _get_pbs_node_name(node_opts, &i);
			slurm_hostlist_push(hl, temp);
			xfree(temp);
		} else
			i++;

	}

	if(!node_cnt)
		node_cnt = 1;
	else {
		opt.nodes_set = true;
		opt.min_nodes = opt.max_nodes = node_cnt;
	}

	if(ppn) {
		ppn *= node_cnt;
		opt.ntasks_set = true;
		opt.ntasks = ppn;
	}

	if(slurm_hostlist_count(hl) > 0) {
		xfree(opt.nodelist);
		opt.nodelist = slurm_hostlist_ranged_string_xmalloc(hl);
#ifdef HAVE_BG
		slurm_info("\tThe nodelist option should only be used if\n"
		     "\tthe block you are asking for can be created.\n"
		     "\tPlease consult smap before using this option\n"
		     "\tor your job may be stuck with no way to run.");
#endif
	}

	slurm_hostlist_destroy(hl);
}

static void _get_next_pbs_option(char *pbs_options, int *i)
{
	while(pbs_options[*i] && pbs_options[*i] != ',')
		(*i)++;
	if(pbs_options[*i])
		(*i)++;
}

static char *_get_pbs_option_value(char *pbs_options, int *i)
{
	int start = (*i);
	char *value = NULL;

	while(pbs_options[*i] && pbs_options[*i] != ',')
		(*i)++;
	value = xmalloc((*i)-start+1);
	memcpy(value, pbs_options+start, (*i)-start);

	if(pbs_options[*i])
		(*i)++;

	return value;
}

static void _parse_pbs_resource_list(char *rl)
{
	int i = 0;
	char *temp = NULL;

	while(rl[i]) {
		if(!strncmp(rl+i, "arch=", 5)) {
			i+=5;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "cput=", 5)) {
			i+=5;
			temp = _get_pbs_option_value(rl, &i);
			if (!temp) {
				slurm_error("No value given for cput");
				exit(error_exit);
			}
			xfree(opt.time_limit_str);
			opt.time_limit_str = slurm_xstrdup(temp);
			xfree(temp);
		} else if(!strncmp(rl+i, "file=", 5)) {
			int end = 0;

			i+=5;
			temp = _get_pbs_option_value(rl, &i);
			if(!temp) {
				slurm_error("No value given for file");
				exit(error_exit);
			}
			end = strlen(temp) - 1;
			if (toupper(temp[end]) == 'B') {
				/* In Torque they do GB or MB on the
				 * end of size, we just want G or M so
				 * we will remove the b on the end
				 */
				temp[end] = '\0';
			}
			opt.tmpdisk = str_to_mbytes2(temp);
			if (opt.tmpdisk < 0) {
				slurm_error("invalid tmp value %s", temp);
				exit(error_exit);
			}
			xfree(temp);
		} else if(!strncmp(rl+i, "host=", 5)) {
			i+=5;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "mem=", 4)) {
			int end = 0;

			i+=4;
			temp = _get_pbs_option_value(rl, &i);
			if(!temp) {
				slurm_error("No value given for mem");
				exit(error_exit);
			}
			end = strlen(temp) - 1;
			if (toupper(temp[end]) == 'B') {
				/* In Torque they do GB or MB on the
				 * end of size, we just want G or M so
				 * we will remove the b on the end
				 */
				temp[end] = '\0';
			}
			opt.realmem = (int) str_to_mbytes2(temp);
			if (opt.realmem < 0) {
				slurm_error("invalid memory constraint %s", temp);
				exit(error_exit);
			}

			xfree(temp);
		} else if(!strncmp(rl+i, "nice=", 5)) {
			i+=5;
			temp = _get_pbs_option_value(rl, &i);
			if (temp)
				opt.nice = strtol(temp, NULL, 10);
			else
				opt.nice = 100;
			if (abs(opt.nice) > NICE_OFFSET) {
				slurm_error("Invalid nice value, must be between "
				      "-%d and %d", NICE_OFFSET, NICE_OFFSET);
				exit(error_exit);
			}
			xfree(temp);
		} else if(!strncmp(rl+i, "nodes=", 6)) {
			i+=6;
			temp = _get_pbs_option_value(rl, &i);
			if(!temp) {
				slurm_error("No value given for nodes");
				exit(error_exit);
			}
			_parse_pbs_nodes_opts(temp);
			xfree(temp);
		} else if(!strncmp(rl+i, "opsys=", 6)) {
 			i+=6;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "other=", 6)) {
			i+=6;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "pcput=", 6)) {
			i+=6;
			temp = _get_pbs_option_value(rl, &i);
			if(!temp) {
				slurm_error("No value given for pcput");
				exit(error_exit);
			}
			xfree(opt.time_limit_str);
			opt.time_limit_str = slurm_xstrdup(temp);
			xfree(temp);
		} else if(!strncmp(rl+i, "pmem=", 5)) {
			i+=5;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "pvmem=", 6)) {
			i+=6;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "software=", 9)) {
			i+=9;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "vmem=", 5)) {
			i+=5;
			_get_next_pbs_option(rl, &i);
		} else if(!strncmp(rl+i, "walltime=", 9)) {
			i+=9;
			temp = _get_pbs_option_value(rl, &i);
			if(!temp) {
				slurm_error("No value given for walltime");
				exit(error_exit);
			}
			xfree(opt.time_limit_str);
			opt.time_limit_str = slurm_xstrdup(temp);
			xfree(temp);
		} else
			i++;
	}
}

/*
 * _opt_verify : perform some post option processing verification
 *
 */
static bool _opt_verify(void)
{
	bool verified = true;
	char *dist = NULL, *lllp_dist = NULL;

	if (opt.quiet && opt.slurm_verbose) {
		slurm_error ("don't specify both --slurm_verbose (-v) and --quiet (-Q)");
		verified = false;
	}

	_fullpath(&opt.efname, opt.cwd);
	_fullpath(&opt.ifname, opt.cwd);
	_fullpath(&opt.ofname, opt.cwd);

	if ((opt.ntasks_per_node > 0) && (!opt.ntasks_set)) {
		opt.ntasks = opt.min_nodes * opt.ntasks_per_node;
		opt.ntasks_set = 1;
	}

	if (opt.mincpus < opt.cpus_per_task)
		opt.mincpus = opt.cpus_per_task;

	if ((opt.job_name == NULL) && (opt.script_argc > 0))
		opt.job_name = base_name2(opt.script_argv[0]);
	if (opt.job_name)
		setenv("SLURM_JOB_NAME", opt.job_name, 0);

	/* check for realistic arguments */
	if (opt.ntasks <= 0) {
		slurm_error("invalid number of tasks (-n %d)", opt.ntasks);
		verified = false;
	}

	if (opt.cpus_per_task <= 0) {
		slurm_error("invalid number of cpus per task (-c %d)",
		      opt.cpus_per_task);
		verified = false;
	}

	if ((opt.min_nodes < 0) || (opt.max_nodes < 0) ||
	    (opt.max_nodes && (opt.min_nodes > opt.max_nodes))) {
		slurm_error("invalid number of nodes (-N %d-%d)",
		      opt.min_nodes, opt.max_nodes);
		verified = false;
	}

#ifdef HAVE_BGL
	if (opt.blrtsimage && strchr(opt.blrtsimage, ' ')) {
		slurm_error("invalid BlrtsImage given '%s'", opt.blrtsimage);
		verified = false;
	}
#endif

	if (opt.linuximage && strchr(opt.linuximage, ' ')) {
#ifdef HAVE_BGL
		slurm_error("invalid LinuxImage given '%s'", opt.linuximage);
#else
		slurm_error("invalid CnloadImage given '%s'", opt.linuximage);
#endif
		verified = false;
	}

	if (opt.mloaderimage && strchr(opt.mloaderimage, ' ')) {
		slurm_error("invalid MloaderImage given '%s'", opt.mloaderimage);
		verified = false;
	}

	if (opt.ramdiskimage && strchr(opt.ramdiskimage, ' ')) {
#ifdef HAVE_BGL
		slurm_error("invalid RamDiskImage given '%s'", opt.ramdiskimage);
#else
		slurm_error("invalid IoloadImage given '%s'", opt.ramdiskimage);
#endif
		verified = false;
	}

	if ((opt.realmem > -1) && (opt.mem_per_cpu > -1)) {
		if (opt.realmem < opt.mem_per_cpu) {
			slurm_info("mem < mem-per-cpu - resizing mem to be equal "
			     "to mem-per-cpu");
			opt.realmem = opt.mem_per_cpu;
		}
	}

        /* Check to see if user has specified enough resources to
	 * satisfy the plane distribution with the specified
	 * plane_size.
	 * if (n/plane_size < N) and ((N-1) * plane_size >= n) -->
	 * problem Simple check will not catch all the problem/invalid
	 * cases.
	 * The limitations of the plane distribution in the cons_res
	 * environment are more extensive and are documented in the
	 * SLURM reference guide.  */
	if (opt.distribution == SLURM_DIST_PLANE && opt.plane_size) {
		if ((opt.min_nodes <= 0) ||
		    ((opt.ntasks/opt.plane_size) < opt.min_nodes)) {
			if (((opt.min_nodes-1)*opt.plane_size) >= opt.ntasks) {
#if(0)
				slurm_info("Too few processes ((n/plane_size) %d < N %d) "
				     "and ((N-1)*(plane_size) %d >= n %d)) ",
				     opt.ntasks/opt.plane_size, opt.min_nodes,
				     (opt.min_nodes-1)*opt.plane_size, opt.ntasks);
#endif
				slurm_error("Too few processes for the requested "
				      "{plane,node} distribution");
				exit(error_exit);
			}
		}
	}

	_set_distribution(opt.distribution, &dist, &lllp_dist);
	if(dist)
		if (setenvf(NULL, "SLURM_DISTRIBUTION", "%s", dist)) {
			slurm_error("Can't set SLURM_DISTRIBUTION env variable");
		}

	if(opt.distribution == SLURM_DIST_PLANE)
		if (setenvf(NULL, "SLURM_DIST_PLANESIZE", "%d",
			    opt.plane_size)) {
			slurm_error("Can't set SLURM_DIST_PLANESIZE env variable");
		}

	if(lllp_dist)
		if (setenvf(NULL, "SLURM_DIST_LLLP", "%s", lllp_dist)) {
			slurm_error("Can't set SLURM_DIST_LLLP env variable");
		}

	/* bound threads/cores from ntasks_cores/sockets */
	if (opt.ntasks_per_core > 0) {
		/* if cpu_bind_type doesn't already have a auto pref,
		 * choose the level based on the level of ntasks
		 */
		if (!(opt.cpu_bind_type & (CPU_BIND_TO_SOCKETS |
					   CPU_BIND_TO_CORES |
					   CPU_BIND_TO_THREADS))) {
			opt.cpu_bind_type |= CPU_BIND_TO_CORES;
		}
	}
	if (opt.ntasks_per_socket > 0) {
		/* if cpu_bind_type doesn't already have a auto pref,
		 * choose the level based on the level of ntasks
		 */
		if (!(opt.cpu_bind_type & (CPU_BIND_TO_SOCKETS |
					   CPU_BIND_TO_CORES |
					   CPU_BIND_TO_THREADS))) {
			opt.cpu_bind_type |= CPU_BIND_TO_SOCKETS;
		}
	}

	/* massage the numbers */
	if ((opt.nodes_set || opt.extra_set)				&&
	    ((opt.min_nodes == opt.max_nodes) || (opt.max_nodes == 0))	&&
	    !opt.ntasks_set) {
		/* 1 proc / node default */
		opt.ntasks = MAX(opt.min_nodes, 1);

		/* 1 proc / min_[socket * core * thread] default */
		if (opt.sockets_per_node != NO_VAL) {
			opt.ntasks *= opt.sockets_per_node;
			opt.ntasks_set = true;
		}
		if (opt.cores_per_socket != NO_VAL) {
			opt.ntasks *= opt.cores_per_socket;
			opt.ntasks_set = true;
		}
		if (opt.threads_per_core != NO_VAL) {
			opt.ntasks *= opt.threads_per_core;
			opt.ntasks_set = true;
		}

	} else if (opt.nodes_set && opt.ntasks_set) {

		/*
		 *  make sure # of procs >= min_nodes
		 */
		if (opt.ntasks < opt.min_nodes) {

			slurm_info ("Warning: can't run %d processes on %d "
			      "nodes, setting nnodes to %d",
			      opt.ntasks, opt.min_nodes, opt.ntasks);

			opt.min_nodes = opt.ntasks;
			if (   opt.max_nodes
			       && (opt.min_nodes > opt.max_nodes) )
				opt.max_nodes = opt.min_nodes;
		}

	} /* else if (opt.ntasks_set && !opt.nodes_set) */

	if(!opt.nodelist) {
		if((opt.nodelist = slurm_xstrdup(getenv("SLURM_HOSTFILE")))) {
			/* make sure the file being read in has a / in
			   it to make sure it is a file in the
			   valid_node_list function */
			if(!strstr(opt.nodelist, "/")) {
				char *add_slash = slurm_xstrdup("./");
				xstrcat(add_slash, opt.nodelist);
				xfree(opt.nodelist);
				opt.nodelist = add_slash;
			}
			opt.distribution = SLURM_DIST_ARBITRARY;
			if (!_valid_node_list(&opt.nodelist)) {
				slurm_error("Failure getting NodeNames from "
				      "hostfile");
				exit(error_exit);
			} else {
				slurm_debug("loaded nodes (%s) from hostfile",
				      opt.nodelist);
			}
		}
	} else {
		if (!_valid_node_list(&opt.nodelist))
			exit(error_exit);
	}

	/* set up the proc and node counts based on the arbitrary list
	   of nodes */
	if((opt.distribution == SLURM_DIST_ARBITRARY)
	   && (!opt.nodes_set || !opt.ntasks_set)) {
		hostlist_t hl = slurm_hostlist_create(opt.nodelist);
		if(!opt.ntasks_set) {
			opt.ntasks_set = 1;
			opt.ntasks = slurm_hostlist_count(hl);
		}
		if(!opt.nodes_set) {
			opt.nodes_set = 1;
			slurm_hostlist_uniq(hl);
			opt.min_nodes = opt.max_nodes
				= slurm_hostlist_count(hl);
		}
		slurm_hostlist_destroy(hl);
	}

	if (opt.time_limit_str) {
		opt.time_limit = time_str2mins2(opt.time_limit_str);
		if ((opt.time_limit < 0) && (opt.time_limit != INFINITE)) {
			slurm_error("Invalid time limit specification");
			exit(error_exit);
		}
		if (opt.time_limit == 0)
			opt.time_limit = INFINITE;
	}
	if (opt.time_min_str) {
		opt.time_min = time_str2mins2(opt.time_min_str);
		if ((opt.time_min < 0) && (opt.time_min != INFINITE)) {
			slurm_error("Invalid time-min specification");
			exit(error_exit);
		}
		if (opt.time_min == 0)
			opt.time_min = INFINITE;
	}

	if (opt.ckpt_interval_str) {
		opt.ckpt_interval = time_str2mins2(opt.ckpt_interval_str);
		if ((opt.ckpt_interval < 0) &&
		    (opt.ckpt_interval != INFINITE)) {
			slurm_error("Invalid checkpoint interval specification");
			exit(error_exit);
		}
	}

	if ((opt.euid != (uid_t) -1) && (opt.euid != opt.uid))
		opt.uid = opt.euid;

	if ((opt.egid != (gid_t) -1) && (opt.egid != opt.gid))
		opt.gid = opt.egid;

	if (opt.immediate) {
		char *sched_name = slurm_get_sched_type();
		if (strcmp(sched_name, "sched/wiki") == 0) {
			slurm_info("WARNING: Ignoring the -I/--immediate option "
				"(not supported by Maui)");
			opt.immediate = false;
		}
		xfree(sched_name);
	}

	if (opt.open_mode) {
		/* Propage mode to spawned job using environment variable */
		if (opt.open_mode == OPEN_MODE_APPEND)
			setenvf(NULL, "SLURM_OPEN_MODE", "a");
		else
			setenvf(NULL, "SLURM_OPEN_MODE", "t");
	}
	if (opt.dependency)
		setenvfs("SLURM_JOB_DEPENDENCY=%s", opt.dependency);

	if (opt.acctg_freq >= 0)
		setenvf(NULL, "SLURM_ACCTG_FREQ", "%d", opt.acctg_freq);

#ifdef HAVE_AIX
	if (opt.network == NULL)
		opt.network = "us,sn_all,bulk_xfer";
	setenv("SLURM_NETWORK", opt.network, 1);
#endif

	if (slurm_verify_cpu_bind(NULL, &opt.cpu_bind,
				  &opt.cpu_bind_type))
		exit(error_exit);
	if (opt.cpu_bind_type && (getenv("SBATCH_CPU_BIND") == NULL)) {
		char tmp[64];
		slurm_sprint_cpu_bind_type(tmp, opt.cpu_bind_type);
		if (opt.cpu_bind) {
			setenvf(NULL, "SBATCH_CPU_BIND", "%s:%s",
				tmp, opt.cpu_bind);
		} else {
			setenvf(NULL, "SBATCH_CPU_BIND", "%s", tmp);
		}
	}
	if (opt.mem_bind_type && (getenv("SBATCH_MEM_BIND") == NULL)) {
		char tmp[64];
		slurm_sprint_mem_bind_type(tmp, opt.mem_bind_type);
		if (opt.mem_bind) {
			setenvf(NULL, "SBATCH_MEM_BIND", "%s:%s",
				tmp, opt.mem_bind);
		} else {
			setenvf(NULL, "SBATCH_MEM_BIND", "%s", tmp);
		}
	}

	return verified;
}

static uint16_t _parse_pbs_mail_type(const char *arg)
{
	uint16_t rc;

	if (strcasecmp(arg, "b") == 0)
		rc = MAIL_JOB_BEGIN;
	else if  (strcasecmp(arg, "e") == 0)
		rc = MAIL_JOB_END;
	else if (strcasecmp(arg, "a") == 0)
		rc = MAIL_JOB_FAIL;
	else if (strcasecmp(arg, "bea") == 0
		|| strcasecmp(arg, "eba") == 0
		|| strcasecmp(arg, "eab") == 0
		|| strcasecmp(arg, "bae") == 0)
		rc = MAIL_JOB_BEGIN | MAIL_JOB_END |  MAIL_JOB_FAIL;
	else if (strcasecmp(arg, "be") == 0
		|| strcasecmp(arg, "eb") == 0)
		rc = MAIL_JOB_BEGIN | MAIL_JOB_END;
	else if (strcasecmp(arg, "ba") == 0
		|| strcasecmp(arg, "ab") == 0)
		rc = MAIL_JOB_BEGIN | MAIL_JOB_FAIL;
	else if (strcasecmp(arg, "ea") == 0
		|| strcasecmp(arg, "ae") == 0)
		rc = MAIL_JOB_END |  MAIL_JOB_FAIL;
	else
		rc = 0;		/* arg="n" or failure */

	return rc;
}

/*
 *  Get a decimal integer from arg.
 *
 *  Returns the integer on success, exits program on failure.
 *
 */
static int
_get_int(const char *arg, const char *what)
{
	char *p;
	long int result = strtol(arg, &p, 10);

	if ((*p != '\0') || (result < 0L)) {
		slurm_error ("Invalid numeric value \"%s\" for %s.", arg, what);
		exit(error_exit);
	}

	if (result > INT_MAX) {
		slurm_error ("Numeric argument (%ld) to big for %s.", result, what);
	}

	return (int) result;
}


/*
 * Return an absolute path for the "filename".  If "filename" is already
 * an absolute path, it returns a copy.  Free the returned with xfree().
 */
static void _fullpath(char **filename, const char *cwd)
{
	char *ptr = NULL;

	if ((*filename == NULL) || (*filename[0] == '/'))
		return;

	ptr = slurm_xstrdup(cwd);
	xstrcat(ptr, "/");
	xstrcat(ptr, *filename);
	xfree(*filename);
	*filename = ptr;
}
