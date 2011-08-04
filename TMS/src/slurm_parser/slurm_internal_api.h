
#ifndef _VISHNU_SLURM_UTIL_H
#define _VISHNU_SLURM_UTIL_H

#include "slurm/slurm.h"
#include "slurm/slurmdb.h"

#include "slurm_internal_types.h"

////////////////////////////////SLURM PROC ARG UTILITY FUNCTION///////////////////////////
/*
 * verify that a resource counts in arg are of a known form X, X:X, X:X:X, or
 * X:X:X:X, where X is defined as either (count, min-max, or '*')
 * RET true if valid
 */
bool verify_socket_core_thread_count(const char *arg, int *min_sockets,
    int *min_cores, int *min_threads,
    cpu_bind_type_t *cpu_bind_type);

/*
 * get either 1 or 2 integers for a resource count in the form of either
 * (count, min-max, or '*')
 * A partial error message is passed in via the 'what' param.
 * IN arg - argument
 * IN what - variable name (for errors)
 * OUT min - first number
 * OUT max - maximum value if specified, NULL if don't care
 * IN isFatal - if set, exit on error
 * RET true if valid
 */
bool get_resource_arg_range(const char *arg, const char *what, int* min,
          int *max, bool isFatal);

/*
 * verify that a distribution type in arg is of a known form
 * returns the task_dist_states, or -1 if state is unknown
 */
task_dist_states_t verify_dist_type(const char *arg, uint32_t *plane_size);

/*
 * verify that a node count in arg is of a known form (count or min-max)
 * OUT min, max specified minimum and maximum node counts
 * RET true if valid
 */
bool verify_node_count(const char *arg, int *min_nodes, int *max_nodes);

/*
 * verify that a hint is valid and convert it into the implied settings
 * RET true if valid
 */
bool verify_hint(const char *arg, int *min_sockets, int *min_cores,
    int *min_threads, int *ntasks_per_core,
    cpu_bind_type_t *cpu_bind_type);

uint16_t parse_mail_type(const char *arg);

/*
 * verify that a connection type in arg is of known form
 * returns the connection_type or -1 if not recognized
 */
uint16_t verify_conn_type(const char *arg);

/* return command name from its full path name */
char * base_name(char* command);

/*
 * str_to_mbytes(): verify that arg is numeric with optional "K", "M", "G"
 * or "T" at end and return the number in mega-bytes
 */
long str_to_mbytes(const char *arg);

/* verify a node list is valid based on the dist and task count given */
bool verify_node_list(char **node_list_pptr, enum task_dist_states dist,
          int task_count);

/* verify a hint and convert it into the implied settings */
bool verify_hint(const char *arg, int *min_sockets, int *min_cores,
     int *min_threads, int *ntasks_per_core,
     cpu_bind_type_t *cpu_bind_type);

/* search PATH to confirm the access of the given command */
char *search_path(char *cwd, char *cmd, bool check_current_dir,
      int access_mode);

/* Translate a signal option string "--signal=<int>[@<time>]" into
 * it's warn_signal and warn_time components.
 * RET 0 on success, -1 on failure */
int get_signal_opts(char *optarg, uint16_t *warn_signal, uint16_t *warn_time);

/////////////////////////////////////SLURLM HOST LIST FUNCTION ////////////

hostlist_t hostlist_create(const char *hostlist);

char *hostlist_ranged_string_xmalloc(hostlist_t hl);

///////////////////////////////////SLURM parser time ////////////////////////

extern time_t parse_time(char *time_str, int past);

extern int time_str2mins(char *string);

//////////////////////////////////SLURM plugstack ///////////////////////////

int spank_init_allocator (void);

int spank_init_post_opt (void);

int spank_fini (slurmd_job_t *job);

struct option *spank_option_table_create (const struct option *orig_options);

void spank_option_table_destroy (struct option *opt_table);

int spank_process_option (int optval, const char *optarg);

//////////////////////////////SLURM slumdb_def///////////////////////////////////

extern List slurmdb_get_info_cluster(char *cluster_name);

////////////////////////////SLURM slurm_protocol_api /////////////////////////

char *slurm_get_cluster_name(void);

uint32_t slurm_get_slurm_user_id(void);

char *slurm_get_sched_type(void);

////////////////////////////SLURM slurm_resource_info ////////////////////////  

int slurm_verify_cpu_bind(const char *arg, char **cpu_bind,
        cpu_bind_type_t *flags);
int slurm_verify_mem_bind(const char *arg, char **mem_bind,
        mem_bind_type_t *flags);

////////////////////////////SLURM slurm_rlimits_info //////////////////////////

extern slurm_rlimits_info_t *get_slurm_rlimits_info( void );

extern int parse_rlimits( char *rlimits_str, int propagate_flag );

///////////////////////////SLURM slurm_protocol_defs /////////////////////

inline void slurm_free_return_code_msg(return_code_msg_t * msg);

void slurm_free_will_run_response_msg(will_run_response_msg_t *msg);

/////////////////////////// SLURM working_cluster /////////////////////////
extern uint16_t slurmdb_setup_cluster_dims(void);

extern uint32_t slurmdb_setup_cluster_flags(void);

extern uint32_t slurmdb_str_2_cluster_flags(char *flags_in);

extern char *slurmdb_cluster_flags_2_str(uint32_t flags_in);

/////////////////////////////SLURM env ////////////////

int envcount (char **env);

char *  getenvp(char **env, const char *name);

int setenvf(char ***envp, const char *name, const char *fmt, ...);

int setenvfs(const char *fmt, ...);

void  unsetenvp(char **env, const char *name);

int setup_env(env_t *env, bool preserve_env);

char **env_array_create(void);


#endif /* !_VISHNU_SLURM_UTIL_H */
