
#ifndef _HAVE_OPT_H
#define _HAVE_OPT_H

#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#include "slurm_internal_api.h"

#include <slurm/slurm.h>

typedef struct sbatch_options {
	List clusters; /* cluster to run this on. */
	char *progname;		/* argv[0] of this program or   */

	/* batch script argv and argc, if provided on the command line */
	int script_argc;
	char **script_argv;

	char *user;		/* local username		*/
	uid_t uid;		/* local uid			*/
	gid_t gid;		/* local gid			*/
	uid_t euid;		/* effective user --uid=user	*/
	gid_t egid;		/* effective group --gid=group	*/
	char *cwd;		/* current working directory	*/

	int  ntasks;		/* --ntasks=n,      -n n	*/
	bool ntasks_set;	/* true if ntasks explicitly set */
	int  cpus_per_task;	/* --cpus-per-task=n, -c n	*/
	bool cpus_set;		/* true if cpus_per_task explicitly set */
	int  min_nodes;		/* --nodes=n,       -N n	*/
	int  max_nodes;		/* --nodes=x-n,       -N x-n	*/
	bool nodes_set;		/* true if nodes explicitly set */
	int sockets_per_node;	/* --sockets-per-node=n		*/
	int cores_per_socket;	/* --cores-per-socket=n		*/
	int threads_per_core;	/* --threads-per-core=n		*/
	int ntasks_per_node;	/* --ntasks-per-node=n		*/
	int ntasks_per_socket;	/* --ntasks-per-socket=n	*/
	int ntasks_per_core;	/* --ntasks-per-core=n		*/
	cpu_bind_type_t cpu_bind_type; /* --cpu_bind=           */
	char *cpu_bind;		/* binding map for map/mask_cpu */
	mem_bind_type_t mem_bind_type; /* --mem_bind=		*/
	char *mem_bind;		/* binding map for map/mask_mem	*/
	bool extra_set;		/* true if extra node slurm_info explicitly set */
	int  time_limit;	/* --time,   -t	(int minutes)	*/
	char *time_limit_str;	/* --time,   -t (string)	*/
	int  time_min;		/* --min-time 	(int minutes)	*/
	char *time_min_str;	/* --min-time (string)		*/
	char *partition;	/* --partition=n,   -p n   	*/
  enum task_dist_states
    distribution;	/* --distribution=, -m dist	*/
  uint32_t plane_size;    /* lllp distribution -> plane_size for
                           * when -m plane=<# of lllp per
                           * plane> */
	char *job_name;		/* --job-name=,     -J name	*/
	unsigned int jobid;     /* --jobid=jobid                */
	bool jobid_set;		/* true of jobid explicitly set */
	char *mpi_type;		/* --mpi=type			*/
	char *dependency;	/* --dependency, -P type:jobid	*/
	int nice;		/* --nice			*/
	char *account;		/* --account, -U acct_name	*/
	char *comment;		/* --comment			*/
	char *propagate;	/* --propagate[=RLIMIT_CORE,...]*/
	char *qos;		/* --qos			*/
	int immediate;		/* -i, --immediate      	*/
	uint16_t warn_signal;	/* --signal=<int>@<time>	*/
	uint16_t warn_time;	/* --signal=<int>@<time>	*/

	bool hold;		/* --hold, -H			*/
	bool no_kill;		/* --no-kill, -k		*/
	int requeue;		/* --requeue and --no-requeue	*/
	uint8_t open_mode;	/* --open-mode			*/
	int acctg_freq;		/* --acctg-freq=secs		*/
	bool overcommit;	/* --overcommit -O		*/
	uint16_t shared;	/* --share,   -s		*/
	char *licenses;		/* --licenses, -L		*/
	char *network;		/* --network=			*/
	int  quiet;
	int  slurm_verbose;
	uint16_t wait_all_nodes;  /* --wait-nodes-ready=val	*/
	char *wrap;

	/* constraint options */
	int mincpus;		/* --mincpus=n			*/
	int minsockets;		/* --minsockets=n		*/
	int mincores;		/* --mincores=n			*/
	int minthreads;		/* --minthreads=n		*/
	int mem_per_cpu;	/* --mem-per-cpu=n		*/
	int realmem;		/* --mem=n			*/
	long tmpdisk;		/* --tmp=n			*/
	char *constraints;	/* --constraints=, -C constraint*/
	char *gres;		/* --gres			*/
	bool contiguous;	/* --contiguous			*/
	char *nodelist;		/* --nodelist=node1,node2,...	*/
	char *exc_nodes;	/* --exclude=node1,node2,... -x	*/

	/* BLUEGENE SPECIFIC */
	uint16_t geometry[HIGHEST_DIMENSIONS]; /* --geometry, -g	*/
	bool reboot;		/* --reboot			*/
	bool no_rotate;		/* --no_rotate, -R		*/
	uint16_t conn_type;	/* --conn-type 			*/
	char *blrtsimage;       /* --blrts-image BlrtsImage for block */
	char *linuximage;       /* --linux-image LinuxImage for block */
	char *mloaderimage;     /* --mloader-image mloaderImage for block */
	char *ramdiskimage;     /* --ramdisk-image RamDiskImage for block */
	/*********************/

	time_t begin;		/* --begin			*/
	uint16_t mail_type;	/* --mail-type			*/
	char *mail_user;	/* --mail-user			*/
	char *ifname;		/* input file name		*/
	char *ofname;		/* output file name		*/
	char *efname;		/* error file name		*/
	int get_user_env_time;	/* --get-user-env[=timeout]	*/
	int get_user_env_mode;	/* --get-user-env=[S|L]         */
	char *export_env;	/* --export			*/
	char *wckey;            /* --wckey workload characterization key */
	char *reservation;      /* --reservation */
 	int ckpt_interval;	/* --checkpoint (int minutes)   */
 	char *ckpt_interval_str;/* --checkpoint (string)        */
 	char *ckpt_dir;		/* --checkpoint-dir (string)    */
	char **spank_job_env;	/* SPANK controlled environment for job
				 * Prolog and Epilog		*/
	int spank_job_env_size;	/* size of spank_job_env	*/
} opt_t;

extern opt_t opt;
extern int error_exit;

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
 * Return a pointer to the batch script file name if provided on the command
 * line, otherwise return NULL (in which case the script will need to be read
 * from standard input).
 */
char *process_options_first_pass(int argc, char **argv);

/* process options:
 * 1. update options with option set in the script
 * 2. update options with env vars
 * 3. update options with commandline args
 * 4. perform some verification that options are reasonable
 */
int process_options_second_pass(int argc, char *argv[], const char *file,
				const void *script_body, int script_size);

#endif	/* _HAVE_OPT_H */
