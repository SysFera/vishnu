
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <string.h>

#include "slurm_parser.h"

int slurm_parse_script(int argc, char *argv[], job_desc_msg_t *desc)
{
	log_options_t logopt = LOG_OPTS_STDERR_ONLY;
	
	char *script_name;
	void *script_body;
	int script_size = 0;

	slurm_log_init(slurm_xbasename(argv[0]), logopt, 0, NULL);

	_set_exit_code();
	script_name = process_options_first_pass(argc, argv);
  script_body = _get_script_buffer(script_name, &script_size);

  if (script_body == NULL)
    exit(error_exit);

  if (process_options_second_pass(
				(argc - opt.script_argc),
				argv, slurm_xbasename(script_name),
				script_body, script_size) < 0) {
		error("submit parameter parsing");
		exit(error_exit);
	}

	_set_prio_process_env();
	_set_submit_dir_env();
	_set_umask_env();
	slurm_init_job_desc_msg(&(*desc));
	if (_fill_job_desc_from_opts(&(*desc)) == -1) {
		exit(error_exit);
	}

	(*desc).script = (char *)script_body;

	return 0;
}

/* Propagate select user environment variables to the job */
static void _env_merge_filter(job_desc_msg_t *desc)
{
	extern char **environ;
	int i, len;
	char *save_env[2] = { NULL, NULL }, *tmp, *tok, *last = NULL;

	tmp = slurm_xstrdup(opt.export_env);
	tok = strtok_r(tmp, ",", &last);
	while (tok) {
		if (strchr(tok, '=')) {
			save_env[0] = tok;
			slurm_env_array_merge(&desc->environment,
					(const char **)save_env);
		} else {
			len = strlen(tok);
			for (i=0; environ[i]; i++) {
				if (strncmp(tok, environ[i], len) ||
				    (environ[i][len] != '='))
					continue;
				save_env[0] = environ[i];
				slurm_env_array_merge(&desc->environment,
						(const char **)save_env);
				break;
			}
		}
		tok = strtok_r(NULL, ",", &last);
	}
	xfree(tmp);
}

/* Returns 0 on success, -1 on failure */
static int _fill_job_desc_from_opts(job_desc_msg_t *desc)
{
	extern char **environ;

	if (opt.jobid_set)
		desc->job_id = opt.jobid;
	desc->contiguous = opt.contiguous ? 1 : 0;
	desc->features = opt.constraints;
	desc->immediate = opt.immediate;
	desc->gres = opt.gres;
	if (opt.job_name != NULL)
		desc->name = slurm_xstrdup(opt.job_name);
	else
		desc->name = slurm_xstrdup("sbatch");
	desc->reservation  = slurm_xstrdup(opt.reservation);
	desc->wckey  = slurm_xstrdup(opt.wckey);

	desc->req_nodes = opt.nodelist;
	desc->exc_nodes = opt.exc_nodes;
	desc->partition = opt.partition;
	if (opt.min_nodes)
		desc->min_nodes = opt.min_nodes;
	if (opt.licenses)
		desc->licenses = slurm_xstrdup(opt.licenses);
	if (opt.max_nodes)
		desc->max_nodes = opt.max_nodes;
	if (opt.ntasks_per_node)
		desc->ntasks_per_node = opt.ntasks_per_node;
	desc->user_id = opt.uid;
	desc->group_id = opt.gid;
	if (opt.dependency)
		desc->dependency = slurm_xstrdup(opt.dependency);

	if (opt.cpu_bind)
		desc->cpu_bind       = opt.cpu_bind;
	if (opt.cpu_bind_type)
		desc->cpu_bind_type  = opt.cpu_bind_type;
	if (opt.mem_bind)
		desc->mem_bind       = opt.mem_bind;
	if (opt.mem_bind_type)
		desc->mem_bind_type  = opt.mem_bind_type;
	if (opt.plane_size != NO_VAL)
		desc->plane_size     = opt.plane_size;
	desc->task_dist  = opt.distribution;

	desc->network = opt.network;
	if (opt.nice)
		desc->nice = NICE_OFFSET + opt.nice;
	desc->mail_type = opt.mail_type;
	if (opt.mail_user)
		desc->mail_user = slurm_xstrdup(opt.mail_user);
	if (opt.begin)
		desc->begin_time = opt.begin;
	if (opt.account)
		desc->account = slurm_xstrdup(opt.account);
	if (opt.comment)
		desc->comment = slurm_xstrdup(opt.comment);
	if (opt.qos)
		desc->qos = slurm_xstrdup(opt.qos);

	if (opt.hold)
		desc->priority     = 0;

	if (opt.conn_type != (uint16_t) NO_VAL)
		desc->conn_type = opt.conn_type;
	if (opt.reboot)
		desc->reboot = 1;
	if (opt.no_rotate)
		desc->rotate = 0;
	if (opt.blrtsimage)
		desc->blrtsimage = slurm_xstrdup(opt.blrtsimage);
	if (opt.linuximage)
		desc->linuximage = slurm_xstrdup(opt.linuximage);
	if (opt.mloaderimage)
		desc->mloaderimage = slurm_xstrdup(opt.mloaderimage);
	if (opt.ramdiskimage)
		desc->ramdiskimage = slurm_xstrdup(opt.ramdiskimage);

	/* job constraints */
	if (opt.mincpus > -1)
		desc->pn_min_cpus = opt.mincpus;
	if (opt.realmem > -1)
		desc->pn_min_memory = opt.realmem;
	else if (opt.mem_per_cpu > -1)
		desc->pn_min_memory = opt.mem_per_cpu | MEM_PER_CPU;
	if (opt.tmpdisk > -1)
		desc->pn_min_tmp_disk = opt.tmpdisk;
	if (opt.overcommit) {
		desc->min_cpus = MAX(opt.min_nodes, 1);
		desc->overcommit = opt.overcommit;
	} else
		desc->min_cpus = opt.ntasks * opt.cpus_per_task;
	desc->max_cpus = desc->max_cpus;

	if (opt.ntasks_set)
		desc->num_tasks = opt.ntasks;
	if (opt.cpus_set)
		desc->cpus_per_task = opt.cpus_per_task;
	if (opt.ntasks_per_socket > -1)
		desc->ntasks_per_socket = opt.ntasks_per_socket;
	if (opt.ntasks_per_core > -1)
		desc->ntasks_per_core = opt.ntasks_per_core;

	/* node constraints */
	if (opt.sockets_per_node != NO_VAL)
		desc->sockets_per_node = opt.sockets_per_node;
	if (opt.cores_per_socket != NO_VAL)
		desc->cores_per_socket = opt.cores_per_socket;
	if (opt.threads_per_core != NO_VAL)
		desc->threads_per_core = opt.threads_per_core;

	if (opt.no_kill)
		desc->kill_on_node_fail = 0;
	if (opt.time_limit != NO_VAL)
		desc->time_limit = opt.time_limit;
	if (opt.time_min  != NO_VAL)
		desc->time_min = opt.time_min;
	desc->shared = opt.shared;

	desc->wait_all_nodes = opt.wait_all_nodes;
	if (opt.warn_signal)
		desc->warn_signal = opt.warn_signal;
	if (opt.warn_time)
		desc->warn_time = opt.warn_time;

	desc->environment = NULL;
	if (opt.export_env == NULL) {
		slurm_env_array_merge(&desc->environment, (const char **)environ);
	} else if (!strcasecmp(opt.export_env, "ALL")) {
		slurm_env_array_merge(&desc->environment, (const char **)environ);
	} else if (!strcasecmp(opt.export_env, "NONE")) {
		desc->environment = slurm_env_array_create();
		opt.get_user_env_time = 0;
	} else {
		_env_merge_filter(desc);
		opt.get_user_env_time = 0;
	}
	if (opt.get_user_env_time >= 0) {
		slurm_env_array_overwrite(&desc->environment,
				    "SLURM_GET_USER_ENV", "1");
	}

	if(opt.distribution == SLURM_DIST_ARBITRARY) {
		slurm_env_array_overwrite_fmt(&desc->environment,
					"SLURM_ARBITRARY_NODELIST",
					"%s", desc->req_nodes);
	}

	desc->env_size = envcount2(desc->environment);
	desc->argv = opt.script_argv;
	desc->argc = opt.script_argc;
	desc->std_err  = opt.efname;
	desc->std_in   = opt.ifname;
	desc->std_out  = opt.ofname;
	desc->work_dir = opt.cwd;
	if (opt.requeue != NO_VAL)
		desc->requeue = opt.requeue;
	if (opt.open_mode)
		desc->open_mode = opt.open_mode;
	if (opt.acctg_freq >= 0)
		desc->acctg_freq = opt.acctg_freq;

	desc->ckpt_dir = opt.ckpt_dir;
	desc->ckpt_interval = (uint16_t)opt.ckpt_interval;

	return 0;
}

static void _set_exit_code(void)
{
	int i;
	char *val = getenv("SLURM_EXIT_ERROR");

	if (val) {
		i = atoi(val);
		if (i == 0)
			error("SLURM_EXIT_ERROR has zero value");
		else
			error_exit = i;
	}
}

/* Set SLURM_SUBMIT_DIR environment variable with current state */
static void _set_submit_dir_env(void)
{
	char buf[MAXPATHLEN + 1];

	if (getenv("SLURM_SUBMIT_DIR"))	/* use this value */
		return;

	if ((getcwd(buf, MAXPATHLEN)) == NULL) {
		error("getcwd failed: %m");
		exit(error_exit);
	}

	if (setenvf(NULL, "SLURM_SUBMIT_DIR", "%s", buf) < 0) {
		error("unable to set SLURM_SUBMIT_DIR in environment");
		return;
	}
}

/* Set SLURM_UMASK environment variable with current state */
static int _set_umask_env(void)
{
	char mask_char[5];
	mode_t mask;

	if (getenv("SLURM_UMASK"))	/* use this value */
		return SLURM_SUCCESS;

	mask = (int)umask(0);
	umask(mask);

	sprintf(mask_char, "0%d%d%d",
		((mask>>6)&07), ((mask>>3)&07), mask&07);
	if (setenvf(NULL, "SLURM_UMASK", "%s", mask_char) < 0) {
		error ("unable to set SLURM_UMASK in environment");
		return SLURM_FAILURE;
	}
	return SLURM_SUCCESS;
}

/*
 * _set_prio_process_env
 *
 * Set the internal SLURM_PRIO_PROCESS environment variable to support
 * the propagation of the users nice value and the "PropagatePrioProcess"
 * config keyword.
 */
static void  _set_prio_process_env(void)
{
	int retval;

	errno = 0; /* needed to detect a real failure since prio can be -1 */

	if ((retval = getpriority (PRIO_PROCESS, 0)) == -1)  {
		if (errno) {
			error ("getpriority(PRIO_PROCESS): %m");
			return;
		}
	}

	if (setenvf (NULL, "SLURM_PRIO_PROCESS", "%d", retval) < 0) {
		error ("unable to set SLURM_PRIO_PROCESS in environment");
		return;
	}

}

/*
 * Checks if the buffer starts with a shebang (#!).
 */
static bool has_shebang(const void *buf, int size)
{
	char *str = (char *)buf;

	if (size < 2)
		return false;

	if (str[0] != '#' || str[1] != '!')
		return false;

	return true;
}

/*
 * Checks if the buffer contains a NULL character (\0).
 */
static bool contains_null_char(const void *buf, int size)
{
	char *str = (char *)buf;
	int i;

	for (i = 0; i < size; i++) {
		if (str[i] == '\0')
			return true;
	}

	return false;
}

/*
 * Checks if the buffer contains any DOS linebreak (\r\n).
 */
static bool contains_dos_linebreak(const void *buf, int size)
{
	char *str = (char *)buf;
	char prev_char = '\0';
	int i;

	for (i = 0; i < size; i++) {
		if (prev_char == '\r' && str[i] == '\n')
			return true;
		prev_char = str[i];
	}

	return false;
}

/*
 * If "filename" is NULL, the batch script is read from standard input.
 */
static void *_get_script_buffer(const char *filename, int *size)
{
	int fd;
	char *buf = NULL;
	int buf_size = BUFSIZ;
	int buf_left;
	int script_size = 0;
	char *ptr = NULL;
	int tmp_size;

	/*
	 * First figure out whether we are reading from STDIN_FILENO
	 * or from a file.
	 */
	if (filename == NULL) {
		fd = STDIN_FILENO;
	} else {
		fd = open(filename, O_RDONLY);
		if (fd == -1) {
			error("Unable to open file %s", filename);
			goto fail;
		}
	}

	/*
	 * Then read in the script.
	 */
	buf = ptr = xmalloc(buf_size);
	buf_left = buf_size;
	while((tmp_size = read(fd, ptr, buf_left)) > 0) {
		buf_left -= tmp_size;
		script_size += tmp_size;
		if (buf_left == 0) {
			buf_size += BUFSIZ;
			xrealloc(buf, buf_size);
		}
		ptr = buf + script_size;
		buf_left = buf_size - script_size;
	}
	if (filename)
		close(fd);

	/*
	 * Finally we perform some sanity tests on the script.
	 */
	if (script_size == 0) {
		error("Batch script is empty!");
		goto fail;
	} else if (slurm_xstring_is_whitespace(buf)) {
		error("Batch script contains only whitespace!");
		goto fail;
	} else if (!has_shebang(buf, script_size)) {
		error("This does not look like a batch script.  The first");
		error("line must start with #! followed by the path"
		      " to an interpreter.");
		error("For instance: #!/bin/sh");
		goto fail;
	} else if (contains_null_char(buf, script_size)) {
		error("The SLURM controller does not allow scripts that");
		error("contain a NULL character '\\0'.");
		goto fail;
	} else if (contains_dos_linebreak(buf, script_size)) {
		error("Batch script contains DOS line breaks (\\r\\n)");
		error("instead of expected UNIX line breaks (\\n).");
		goto fail;
	}

	*size = script_size;
	return buf;
fail:
	xfree(buf);
	*size = 0;
	return NULL;
}
