
#ifndef _VISHNU_SLURM_TYPE_H
#define _VISHNU_SLURM_TYPE_H

#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>

#include "slurm.h"
#include "slurmdb.h"

#include "list.h"
#include "macros.h"
//#include "pack.h"
//#include "cbuf.h"
#include "getopt.h"
#include "xmalloc.h"
#include "xassert.h"

#define MAX_MSG_LEN 1024
#define SLURM_IO_KEY_SIZE 8

#define SLURM_IO_STDIN 0
#define SLURM_IO_STDOUT 1
#define SLURM_IO_STDERR 2
#define SLURM_IO_ALLSTDIN 3
#define SLURM_IO_CONNECTION_TEST 4

/*
 * Values for the propagate rlimits flag.
 */
#define PROPAGATE_RLIMITS    1  /* The default is to propagate rlimits */
#define NO_PROPAGATE_RLIMITS 0

////////////////////////SLURM eio //////////////////////////

typedef struct eio_obj eio_obj_t;

typedef struct eio_handle_components eio_handle_t;


struct eio_obj {
  int fd;                           /* fd to operate on                */
  void *arg;                        /* application-specific data       */
  struct io_operations *ops;        /* pointer to ops struct for obj   */
  bool shutdown;
};

///////////////////////////SLURM io_hdr /////////////////////////

struct slurm_io_init_msg {
  uint16_t      version;
  unsigned char cred_signature[SLURM_IO_KEY_SIZE];
  uint32_t      nodeid;
  uint32_t      stdout_objs;
  uint32_t      stderr_objs;
};


typedef struct slurm_io_header {
  uint16_t      type;
  uint16_t      gtaskid;
  uint16_t      ltaskid;
  uint32_t      length;
} io_hdr_t;


/////////////////////////////////////SLURM stepd_api //////////////////

typedef enum {
  SLURMSTEPD_NOT_RUNNING = 0,
  SLURMSTEPD_STEP_STARTING,
  SLURMSTEPD_STEP_RUNNING,
  SLURMSTEPD_STEP_ENDING
} slurmstepd_state_t;

/////////////////////////////////////SLURM  slurm_rlimits_info //////////

struct slurm_rlimits_info {
  int  resource;          /* Values:  RLIMIT_NPROC, RLIMIT_MEMLOCK, ... */
  char *name;             /* String: "NPROC",      "MEMLOCK", ...       */
  int  propagate_flag;    /* PROPAGATE_RLIMITS or NO_PROPAGATE_RLIMITS  */
};

typedef struct slurm_rlimits_info slurm_rlimits_info_t;

/////////////////////////////////////SLURM  job_options /////////////////

typedef struct job_options * job_options_t; /* opaque data type */

struct job_option_info {
  int type;
  char *option;
  char *optarg;
};


///////////////////////////////////SLURM env ///////////////////////////////

typedef struct env_options {
  int ntasks;   /* --ntasks=n,      -n n  */
  char *task_count;
  bool ntasks_set;  /* true if ntasks explicitly set */
  bool cpus_set;    /* true if cpus_per_task explicitly set */
  task_dist_states_t distribution; /* --distribution=, -m dist  */
  uint16_t plane_size;         /* plane_size for SLURM_DIST_PLANE */
  cpu_bind_type_t
    cpu_bind_type;  /* --cpu_bind=      */
  char *cpu_bind;   /* binding map for map/mask_cpu */
  mem_bind_type_t
    mem_bind_type;  /* --mem_bind=      */
  char *mem_bind;   /* binding map for tasks to memory  */
  bool overcommit;  /* --overcommit,   -O   */
  int  slurmd_debug;  /* --slurmd-debug, -D           */
  bool labelio;   /* --label-output, -l   */
  dynamic_plugin_data_t *select_jobinfo;
  int nhosts;
  char *nodelist;   /* nodelist in string form */
  char **env;             /* job environment */
  uint16_t comm_port; /* srun's communication port */
  slurm_addr_t *cli;  /* launch node address */
  slurm_addr_t *self;
  int jobid;    /* assigned job id */
  int stepid;         /* assigned step id */
  int procid;   /* global task id (across nodes) */
  int localid;    /* local task id (within node) */
  int nodeid;
  int cpus_per_task;  /* --cpus-per-task=n, -c n  */
  int ntasks_per_node;  /* --ntasks-per-node=n    */
  int ntasks_per_socket;  /* --ntasks-per-socket=n  */
  int ntasks_per_core;  /* --ntasks-per-core=n    */
  int cpus_on_node;
  pid_t task_pid;
  char *sgtids;   /* global ranks array of integers */
  uint16_t pty_port;  /* used to communicate window size changes */
  uint8_t ws_col;   /* window size, columns */
  uint8_t ws_row;   /* window size, row count */
  char *ckpt_dir;   /* --ckpt-dir=                 */
  uint16_t restart_cnt; /* count of job restarts  */
  uint16_t batch_flag;  /* 1 if batch: queued job with script */
} env_t;

//////////////////////////////SLURM slurmstepd_job ///////////////////////

typedef struct srun_key {
	unsigned char data[SLURM_IO_KEY_SIZE];
} srun_key_t;

typedef struct srun_info {
	srun_key_t *key;	   /* srun key for IO verification         */
	slurm_addr_t resp_addr;	   /* response addr for task exit msg      */
	slurm_addr_t ioaddr;       /* Address to connect on for normal I/O.
				      Spawn IO uses messages to the normal
				      resp_addr. */
} srun_info_t;

typedef enum task_state {
	SLURMD_TASK_INIT,
	SLURMD_TASK_STARTING,
	SLURMD_TASK_RUNNING,
	SLURMD_TASK_COMPLETE
} slurmd_task_state_t;

typedef struct task_info {
	pthread_mutex_t mutex;	    /* mutex to protect task state          */
	slurmd_task_state_t state;  /* task state                           */

	int             id;	    /* local task id                        */
	uint32_t        gtid;	    /* global task id                       */
	pid_t           pid;	    /* task pid                             */

	char           *ifname;     /* standard input file name             */
	char           *ofname;     /* standard output file name            */
	char           *efname;     /* standard error file name             */
	int             stdin_fd;   /* standard input file descriptor       */
	int             stdout_fd;  /* standard output file descriptor      */
	int             stderr_fd;  /* standard error file descriptor       */
	int             to_stdin;   /* write file descriptor for task stdin */
	int             from_stdout;/* read file descriptor from task stdout*/
	int             from_stderr;/* read file descriptor from task stderr*/
	eio_obj_t      *in;         /* standard input event IO object       */
	eio_obj_t      *out;        /* standard output event IO object      */
	eio_obj_t      *err;        /* standard error event IO object       */

	bool            esent;      /* true if exit status has been sent    */
	bool            exited;     /* true if task has exited              */
	int             estatus;    /* this task's exit status              */

	int		argc;
	char	      **argv;
} slurmd_task_info_t;

typedef struct slurmd_job {
	slurmstepd_state_t state;
	uint32_t       jobid;  /* Current SLURM job id                      */
	uint32_t       stepid; /* Current step id (or NO_VAL)               */
	uint32_t       nnodes; /* number of nodes in current job            */
	uint32_t       ntasks; /* total number of tasks in current job  */
	uint32_t       nodeid; /* relative position of this node in job     */
	uint32_t       node_tasks; /* number of tasks on *this* node        */
	uint32_t       cpus_per_task;	/* number of cpus desired per task  */
	uint32_t       debug;  /* debug level for job slurmd                */
	uint32_t       job_mem;  /* MB of memory reserved for the job       */
	uint32_t       step_mem; /* MB of memory reserved for the step      */
	uint16_t       cpus;   /* number of cpus to use for this job        */
	uint16_t       argc;   /* number of commandline arguments           */
	char         **env;    /* job environment                           */
	char         **argv;   /* job argument vector                       */
	char          *cwd;    /* path to current working directory         */
	task_dist_states_t task_dist;/* -m distribution                     */
	char          *node_name; /* node name of node running job
				   * needed for front-end systems           */
	cpu_bind_type_t cpu_bind_type; /* --cpu_bind=                       */
	char          *cpu_bind;       /* binding map for map/mask_cpu      */
	mem_bind_type_t mem_bind_type; /* --mem_bind=                       */
	char          *mem_bind;       /* binding map for tasks to memory   */
	switch_jobinfo_t *switch_job; /* switch-specific job information     */
	uid_t         uid;     /* user id for job                           */
	gid_t         gid;     /* group ID for job                          */
	int           ngids;   /* length of the following gids array        */
	gid_t        *gids;    /* array of gids for user specified in uid   */
	bool           aborted;    /* true if already aborted               */
	bool           batch;      /* true if this is a batch job           */
	bool           run_prolog; /* true if need to run prolog            */
	bool           user_managed_io;
	time_t         timelimit;  /* time at which job must stop           */
	char          *task_prolog; /* per-task prolog                      */
	char          *task_epilog; /* per-task epilog                      */
	struct passwd *pwd;   /* saved passwd struct for user job           */
	slurmd_task_info_t  **task;  /* array of task information pointers  */
	eio_handle_t  *eio;
	List 	       sruns; /* List of srun_info_t pointers               */
	List           clients; /* List of struct client_io_info pointers   */
	List stdout_eio_objs; /* List of objs that gather stdout from tasks */
	List stderr_eio_objs; /* List of objs that gather stderr from tasks */
	List free_incoming;   /* List of free struct io_buf * for incoming
			       * traffic. "incoming" means traffic from srun
			       * to the tasks.
			       */
	List free_outgoing;   /* List of free struct io_buf * for outgoing
			       * traffic "outgoing" means traffic from the
			       * tasks to srun.
			       */
	int incoming_count;   /* Count of total incoming message buffers
			       * including free_incoming buffers and
			       * buffers in use.
			       */
	int outgoing_count;   /* Count of total outgoing message buffers
			       * including free_outgoing buffers and
			       * buffers in use.
			       */

	List outgoing_cache;  /* cache of outgoing stdio messages
			       * used when a new client attaches
			       */

	uint8_t	buffered_stdio; /* stdio buffering flag, 1 for line-buffering,
				 * 0 for no buffering
				 */
	uint8_t labelio;	/* 1 for labelling output with the task id */

	pthread_t      ioid;  /* pthread id of IO thread                    */
	pthread_t      msgid; /* pthread id of message thread               */
	eio_handle_t  *msg_handle; /* eio handle for the message thread     */

	pid_t          jmgr_pid;     /* job manager pid                     */
	pid_t          pgid;         /* process group id for tasks          */

	uint16_t       task_flags;
	uint16_t       multi_prog;
	uint16_t       overcommit;
	env_t          *envtp;
	uint32_t       cont_id;

	char          *batchdir;
	jobacctinfo_t *jobacct;
	uint8_t        open_mode;	/* stdout/err append or truncate */
	uint8_t        pty;		/* set if creating pseudo tty	*/
	job_options_t  options;
	char          *ckpt_dir;
	time_t         ckpt_timestamp;
	char          *restart_dir;	/* restart from context */
	uint32_t       resv_id;		/* Cray/BASIL reservation ID	*/
	uint16_t       restart_cnt;	/* batch job restart count	*/
	char	      *job_alloc_cores;	/* needed by the SPANK cpuset plugin */
	char	      *step_alloc_cores;/* needed by the SPANK cpuset plugin */
	List           job_gres_list;	/* Needed by GRES plugin */
	List           step_gres_list;	/* Needed by GRES plugin */
} slurmd_job_t;

///////////////////////////SLURM slurm_protocol_defs /////////////////////

#include <netinet/in.h>

/* LINUX SPECIFIC */
/* this is the slurm equivalent of the operating system file descriptor,
 * which in linux is just an int */
typedef int32_t slurm_fd_t ;

/* this is the slurm equivalent of the BSD sockets sockaddr
 * also defined in slurm/slurm.h for users */
#ifndef __slurm_addr_t_defined
#  define  __slurm_addr_t_defined
   typedef struct sockaddr_in slurm_addr_t ;
#endif

/* this is the slurm equivalent of the BSD sockets fd_set */
typedef fd_set slurm_fd_set ;
typedef fd_set _slurm_fd_set ;

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

#endif /* !_VISHNU_SLURM_TYPE_H */
