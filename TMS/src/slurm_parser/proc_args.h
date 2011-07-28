
#ifndef _PROC_ARGS_H
#define _PROC_ARGS_H


#if HAVE_CONFIG_H
#  include "config.h"
#endif

#include <sys/types.h>
#include <unistd.h>

#include "macros.h" /* true and false */
#include "env.h"


#define format_task_dist_states(t) (t == SLURM_DIST_BLOCK) ? "block" :   \
		                 (t == SLURM_DIST_CYCLIC) ? "cyclic" : \
		                 (t == SLURM_DIST_PLANE) ? "plane" : \
		                 (t == SLURM_DIST_CYCLIC_CYCLIC) ? "cyclic:cyclic" : \
		                 (t == SLURM_DIST_CYCLIC_BLOCK) ? "cyclic:block" : \
		                 (t == SLURM_DIST_BLOCK_CYCLIC) ? "block:cyclic" : \
		                 (t == SLURM_DIST_BLOCK_BLOCK) ? "block:block" : \
			         (t == SLURM_DIST_ARBITRARY) ? "arbitrary" : \
			         "unknown"



/* print the available gres options */
void print_gres_help(void);

/* verify the requested distribution type */
task_dist_states_t verify_dist_type(const char *arg, uint32_t *plane_size);

/* verify the requested connection type */
uint16_t verify_conn_type(const char *arg);

/* verify the requested geometry arguments */
int verify_geometry(const char *arg, uint16_t *geometry);

/* return command name from its full path name */
char * base_name(char* command);

/*
 * str_to_mbytes(): verify that arg is numeric with optional "K", "M", "G"
 * or "T" at end and return the number in mega-bytes
 */
long str_to_mbytes(const char *arg);

/* verify that a node count in arg is of a known form (count or min-max) */
bool verify_node_count(const char *arg, int *min_nodes, int *max_nodes);

/* verify a node list is valid based on the dist and task count given */
bool verify_node_list(char **node_list_pptr, enum task_dist_states dist,
		      int task_count);

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

/* verify resource counts from a complex form of: X, X:X, X:X:X or X:X:X:X */
bool verify_socket_core_thread_count(const char *arg, int *min_sockets,
				     int *min_cores, int *min_threads,
				     cpu_bind_type_t *cpu_bind_type);

/* verify a hint and convert it into the implied settings */
bool verify_hint(const char *arg, int *min_sockets, int *min_cores,
		 int *min_threads, int *ntasks_per_core,
		 cpu_bind_type_t *cpu_bind_type);

/* parse the mail type */
uint16_t parse_mail_type(const char *arg);

/* print the mail type */
char *print_mail_type(const uint16_t type);

/* search PATH to confirm the access of the given command */
char *search_path(char *cwd, char *cmd, bool check_current_dir,
		  int access_mode);

/* helper function for printing options */
char *print_commandline(const int script_argc, char **script_argv);

/* helper function for printing geometry option */
char *print_geometry(const uint16_t *geometry);

/* Translate a signal option string "--signal=<int>[@<time>]" into
 * it's warn_signal and warn_time components.
 * RET 0 on success, -1 on failure */
int get_signal_opts(char *optarg, uint16_t *warn_signal, uint16_t *warn_time);

/* Convert a signal name to it's numeric equivalent.
 * Return 0 on failure */
int sig_name2num(char *signal_name);

#endif /* !_PROC_ARGS_H */
