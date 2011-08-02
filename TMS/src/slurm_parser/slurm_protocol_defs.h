
#ifndef _SLURM_PROTOCOL_DEFS_H
#define _SLURM_PROTOCOL_DEFS_H

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif			/* HAVE_INTTYPES_H */
#else				/* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif				/*  HAVE_CONFIG_H */

#include <slurm.h>
#include <slurmdb.h>
#include <sys/wait.h>

#include "slurm_protocol_common.h"
#include "xassert.h"
#include "working_cluster.h"


/*
 * SLURM Message types
 *
 * IMPORTANT: ADD NEW MESSAGE TYPES TO THE *END* OF ONE OF THESE NUMBERED
 * SECTIONS. ADDING ONE ELSEWHERE WOULD SHIFT THE VALUES OF EXISTING MESSAGE
 * TYPES IN CURRENT PROGRAMS AND PREVENT BACKWARD COMPATABILITY.
 */
typedef enum {
	REQUEST_NODE_REGISTRATION_STATUS = 1001,
	MESSAGE_NODE_REGISTRATION_STATUS,
	REQUEST_RECONFIGURE,
	RESPONSE_RECONFIGURE,
	REQUEST_SHUTDOWN,
	REQUEST_SHUTDOWN_IMMEDIATE,
	RESPONSE_SHUTDOWN,
	REQUEST_PING,
	REQUEST_CONTROL,
	REQUEST_SET_DEBUG_LEVEL,
	REQUEST_HEALTH_CHECK,
	REQUEST_TAKEOVER,
	REQUEST_SET_SCHEDLOG_LEVEL,

	REQUEST_BUILD_INFO = 2001,
	RESPONSE_BUILD_INFO,
	REQUEST_JOB_INFO,
	RESPONSE_JOB_INFO,
	REQUEST_JOB_STEP_INFO,
	RESPONSE_JOB_STEP_INFO,
	REQUEST_NODE_INFO,
	RESPONSE_NODE_INFO,
	REQUEST_PARTITION_INFO,
	RESPONSE_PARTITION_INFO,
	REQUEST_ACCTING_INFO,
	RESPONSE_ACCOUNTING_INFO,
	REQUEST_JOB_ID,
	RESPONSE_JOB_ID,
	REQUEST_BLOCK_INFO,
	RESPONSE_BLOCK_INFO,
	REQUEST_TRIGGER_SET,
	REQUEST_TRIGGER_GET,
	REQUEST_TRIGGER_CLEAR,
	RESPONSE_TRIGGER_GET,
	REQUEST_JOB_INFO_SINGLE,
	REQUEST_SHARE_INFO,
	RESPONSE_SHARE_INFO,
	REQUEST_RESERVATION_INFO,
	RESPONSE_RESERVATION_INFO,
	REQUEST_PRIORITY_FACTORS,
	RESPONSE_PRIORITY_FACTORS,
	REQUEST_TOPO_INFO,
	RESPONSE_TOPO_INFO,
	REQUEST_TRIGGER_PULL,

	REQUEST_UPDATE_JOB = 3001,
	REQUEST_UPDATE_NODE,
	REQUEST_CREATE_PARTITION,
	REQUEST_DELETE_PARTITION,
	REQUEST_UPDATE_PARTITION,
	REQUEST_CREATE_RESERVATION,
	RESPONSE_CREATE_RESERVATION,
	REQUEST_DELETE_RESERVATION,
	REQUEST_UPDATE_RESERVATION,
	REQUEST_UPDATE_BLOCK,

	REQUEST_RESOURCE_ALLOCATION = 4001,
	RESPONSE_RESOURCE_ALLOCATION,
	REQUEST_SUBMIT_BATCH_JOB,
	RESPONSE_SUBMIT_BATCH_JOB,
	REQUEST_BATCH_JOB_LAUNCH,
	REQUEST_CANCEL_JOB,
	RESPONSE_CANCEL_JOB,
	REQUEST_JOB_RESOURCE,
	RESPONSE_JOB_RESOURCE,
	REQUEST_JOB_ATTACH,
	RESPONSE_JOB_ATTACH,
	REQUEST_JOB_WILL_RUN,
	RESPONSE_JOB_WILL_RUN,
	REQUEST_JOB_ALLOCATION_INFO,
	RESPONSE_JOB_ALLOCATION_INFO,
	REQUEST_JOB_ALLOCATION_INFO_LITE,
	RESPONSE_JOB_ALLOCATION_INFO_LITE,
	REQUEST_UPDATE_JOB_TIME,
	REQUEST_JOB_READY,
	RESPONSE_JOB_READY,
	REQUEST_JOB_END_TIME,
	REQUEST_JOB_NOTIFY,
	REQUEST_JOB_SBCAST_CRED,
	RESPONSE_JOB_SBCAST_CRED,

	REQUEST_JOB_STEP_CREATE = 5001,
	RESPONSE_JOB_STEP_CREATE,
	REQUEST_RUN_JOB_STEP,
	RESPONSE_RUN_JOB_STEP,
	REQUEST_CANCEL_JOB_STEP,
	RESPONSE_CANCEL_JOB_STEP,
	REQUEST_UPDATE_JOB_STEP,
	DEFUNCT_RESPONSE_COMPLETE_JOB_STEP, /* DEFUNCT */
	REQUEST_CHECKPOINT,
	RESPONSE_CHECKPOINT,
	REQUEST_CHECKPOINT_COMP,
	REQUEST_CHECKPOINT_TASK_COMP,
	RESPONSE_CHECKPOINT_COMP,
	REQUEST_SUSPEND,
	RESPONSE_SUSPEND,
	REQUEST_STEP_COMPLETE,
	REQUEST_COMPLETE_JOB_ALLOCATION,
	REQUEST_COMPLETE_BATCH_SCRIPT,
	REQUEST_JOB_STEP_STAT,
	RESPONSE_JOB_STEP_STAT,
	REQUEST_STEP_LAYOUT,
	RESPONSE_STEP_LAYOUT,
	REQUEST_JOB_REQUEUE,
	REQUEST_DAEMON_STATUS,
	RESPONSE_SLURMD_STATUS,
	RESPONSE_SLURMCTLD_STATUS,
	REQUEST_JOB_STEP_PIDS,
        RESPONSE_JOB_STEP_PIDS,

	REQUEST_LAUNCH_TASKS = 6001,
	RESPONSE_LAUNCH_TASKS,
	MESSAGE_TASK_EXIT,
	REQUEST_SIGNAL_TASKS,
	REQUEST_CHECKPOINT_TASKS,
	REQUEST_TERMINATE_TASKS,
	REQUEST_REATTACH_TASKS,
	RESPONSE_REATTACH_TASKS,
	REQUEST_KILL_TIMELIMIT,
	REQUEST_SIGNAL_JOB,
	REQUEST_TERMINATE_JOB,
	MESSAGE_EPILOG_COMPLETE,
	REQUEST_ABORT_JOB,	/* job shouldn't be running, kill it without
				 * job/step/task complete responses */
	REQUEST_FILE_BCAST,
	TASK_USER_MANAGED_IO_STREAM,

	SRUN_PING = 7001,
	SRUN_TIMEOUT,
	SRUN_NODE_FAIL,
	SRUN_JOB_COMPLETE,
	SRUN_USER_MSG,
	SRUN_EXEC,
	SRUN_STEP_MISSING,

	PMI_KVS_PUT_REQ = 7201,
	PMI_KVS_PUT_RESP,
	PMI_KVS_GET_REQ,
	PMI_KVS_GET_RESP,

	RESPONSE_SLURM_RC = 8001,

	RESPONSE_FORWARD_FAILED = 9001,

	ACCOUNTING_UPDATE_MSG = 10001,
	ACCOUNTING_FIRST_REG,

} slurm_msg_type_t;

/*****************************************************************************\
 * core api configuration struct
\*****************************************************************************/
typedef struct forward {
	uint16_t   cnt;		/* number of nodes to forward to */
	uint16_t   init;	/* tell me it has been set (FORWARD_INIT) */
	char      *nodelist;	/* ranged string of who to forward the
				 * message to */
	uint32_t   timeout;	/* original timeout increments */
} forward_t;

/*core api protocol message structures */
typedef struct slurm_protocol_header {
	uint16_t version;
	uint16_t flags;
	uint16_t msg_type; /* really slurm_msg_type_t but needs to be
			      uint16_t for packing purposes. */
	uint32_t body_length;
	uint16_t ret_cnt;
	forward_t forward;
	slurm_addr_t orig_addr;
	List ret_list;
} header_t;

typedef struct forward_message {
	header_t header;
	char *buf;
	int buf_len;
	int timeout;
	List ret_list;
	pthread_mutex_t *forward_mutex;
	pthread_cond_t *notify;
} forward_msg_t;

typedef struct forward_struct {
	int timeout;
	uint16_t fwd_cnt;
	pthread_mutex_t forward_mutex;
	pthread_cond_t notify;
	forward_msg_t *forward_msg;
	char *buf;
	int buf_len;
	List ret_list;
} forward_struct_t;

typedef struct slurm_msg {
	slurm_addr_t address;
	void *auth_cred;
	slurm_fd_t conn_fd;
	void *data;
	uint32_t data_size;
	uint16_t flags;
	uint16_t msg_type; /* really a slurm_msg_type_t but needs to be
			    * this way for packing purposes.  message type */
	uint16_t protocol_version; /* DON'T PACK!  Only used if
				    * message comming from non-default
				    * slurm protocol.  Initted to
				    * NO_VAL meaning use the default. */
	/* The following were all added for the forward.c code */
	forward_t forward;
	forward_struct_t *forward_struct;
	slurm_addr_t orig_addr;
	List ret_list;
} slurm_msg_t;

typedef struct return_code_msg {
	uint32_t return_code;
} return_code_msg_t;

typedef struct batch_job_launch_msg {
	uint32_t job_id;
	uint32_t step_id;
	uint32_t uid;
	uint32_t gid;
	uint32_t ntasks;	/* number of tasks in this job         */
	uint32_t num_cpu_groups;/* elements in below cpu arrays */
	uint16_t cpu_bind_type;	/* Internal for slurmd/task_affinity   */
	char     *cpu_bind;	/* Internal for slurmd/task_affinity   */
	uint16_t *cpus_per_node;/* cpus per node */
	uint32_t *cpu_count_reps;/* how many nodes have same cpu count */
	uint16_t cpus_per_task;	/* number of CPUs requested per task */
	char *nodes;		/* list of nodes allocated to job_step */
	char *script;		/* the actual job script, default NONE */
	char *std_err;		/* pathname of stderr */
	char *std_in;		/* pathname of stdin */
	char *std_out;		/* pathname of stdout */
	char *work_dir;		/* full pathname of working directory */
	char *ckpt_dir;		/* location to store checkpoint image */
	char *restart_dir;	/* retart execution from image in this dir */
	uint32_t argc;
	char **argv;
	uint32_t envc;		/* element count in environment */
	char **environment;	/* environment variables to set for job,
				 *   name=value pairs, one per line */
	dynamic_plugin_data_t *select_jobinfo;	/* opaque data type */
	slurm_cred_t *cred;
	uint8_t open_mode;	/* stdout/err append or truncate */
	uint8_t overcommit;	/* if resources being over subscribed */
	uint16_t acctg_freq;	/* accounting polling interval	*/
	uint32_t job_mem;	/* memory limit for job		*/
	uint16_t restart_cnt;	/* batch job restart count	*/
	char **spank_job_env;	/* SPANK job environment variables */
	uint32_t spank_job_env_size;	/* size of spank_job_env */
} batch_job_launch_msg_t;

typedef struct will_run_response_msg {
	uint32_t job_id;	/* ID of job to start */
	char *node_list;	/* nodes where job will start */
	List preemptee_job_id;	/* jobs preempted to start this job */
	uint32_t proc_cnt;	/* CPUs allocated to job at start */
	time_t start_time;	/* time when job will start */
} will_run_response_msg_t;


inline void slurm_free_return_code_msg(return_code_msg_t * msg);


void slurm_free_will_run_response_msg(will_run_response_msg_t *msg);


#endif
