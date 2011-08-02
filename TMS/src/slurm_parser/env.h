
#ifndef _ENV_H
#define _ENV_H

#include <sys/types.h>
#include <unistd.h>
#include <slurm/slurm.h>
#include <sys/utsname.h>

#include "macros.h"
#include "slurm_protocol_api.h"

typedef struct env_options {
	int ntasks;		/* --ntasks=n,      -n n	*/
	char *task_count;
	bool ntasks_set;	/* true if ntasks explicitly set */
	bool cpus_set;		/* true if cpus_per_task explicitly set */
	task_dist_states_t distribution; /* --distribution=, -m dist	*/
	uint16_t plane_size;         /* plane_size for SLURM_DIST_PLANE */
	cpu_bind_type_t
		cpu_bind_type;	/* --cpu_bind=			*/
	char *cpu_bind;		/* binding map for map/mask_cpu	*/
	mem_bind_type_t
		mem_bind_type;	/* --mem_bind=			*/
	char *mem_bind;		/* binding map for tasks to memory	*/
	bool overcommit;	/* --overcommit,   -O		*/
	int  slurmd_debug;	/* --slurmd-debug, -D           */
	bool labelio;		/* --label-output, -l		*/
	dynamic_plugin_data_t *select_jobinfo;
	int nhosts;
	char *nodelist;		/* nodelist in string form */
	char **env;             /* job environment */
	uint16_t comm_port;	/* srun's communication port */
	slurm_addr_t *cli;	/* launch node address */
	slurm_addr_t *self;
	int jobid;		/* assigned job id */
	int stepid;	        /* assigned step id */
	int procid;		/* global task id (across nodes) */
	int localid;		/* local task id (within node) */
	int nodeid;
	int cpus_per_task;	/* --cpus-per-task=n, -c n	*/
	int ntasks_per_node;	/* --ntasks-per-node=n		*/
	int ntasks_per_socket;	/* --ntasks-per-socket=n	*/
	int ntasks_per_core;	/* --ntasks-per-core=n		*/
	int cpus_on_node;
	pid_t task_pid;
	char *sgtids;		/* global ranks array of integers */
	uint16_t pty_port;	/* used to communicate window size changes */
	uint8_t ws_col;		/* window size, columns */
	uint8_t ws_row;		/* window size, row count */
	char *ckpt_dir;		/* --ckpt-dir=                 */
	uint16_t restart_cnt;	/* count of job restarts	*/
	uint16_t batch_flag;	/* 1 if batch: queued job with script */
} env_t;


/* NOTE: These functions operate on the job's current environment
 * if env is NULL, otherwise they operate on the argument array */
int	envcount (char **env);
char *	getenvp(char **env, const char *name);
int	setenvf(char ***envp, const char *name, const char *fmt, ...);
int	setenvfs(const char *fmt, ...);
void	unsetenvp(char **env, const char *name);

int	setup_env(env_t *env, bool preserve_env);


char **env_array_create(void);

#endif
