
#ifndef _DBD_READ_CONFIG_H
#define _DBD_READ_CONFIG_H

#if HAVE_CONFIG_H
#  include "config.h"
#if HAVE_INTTYPES_H
#  include <inttypes.h>
#else  /* !HAVE_INTTYPES_H */
#  if HAVE_STDINT_H
#    include <stdint.h>
#  endif
#endif  /* HAVE_INTTYPES_H */
#else   /* !HAVE_CONFIG_H */
#include <stdint.h>
#endif  /* HAVE_CONFIG_H */

#include <time.h>
#include <pthread.h>
#include "list.h"

#define DEFAULT_SLURMDBD_AUTHTYPE	"auth/none"
//#define DEFAULT_SLURMDBD_JOB_PURGE	12
#define DEFAULT_SLURMDBD_PIDFILE	"/var/run/slurmdbd.pid"
#define DEFAULT_SLURMDBD_ARCHIVE_DIR	"/tmp"
//#define DEFAULT_SLURMDBD_STEP_PURGE	1

/* SlurmDBD configuration parameters */
typedef struct slurm_dbd_conf {
	time_t		last_update;	/* time slurmdbd.conf read	*/
	char *		archive_dir;    /* location to localy
					 * store data if not
					 * using a script               */
	char *		archive_script;	/* script to archive old data	*/
	char *		auth_info;	/* authentication info		*/
	char *		auth_type;	/* authentication mechanism	*/
	uint16_t        control_timeout;/* how long to wait before
					 * backup takes control         */
	char *		dbd_addr;	/* network address of Slurm DBD	*/
	char *		dbd_backup;	/* hostname of Slurm DBD backup */
	char *		dbd_host;	/* hostname of Slurm DBD	*/
	uint16_t	dbd_port;	/* port number for RPCs to DBD	*/
	uint16_t	debug_level;	/* Debug level, default=3	*/
	char *	 	default_qos;	/* default qos setting when
					 * adding clusters              */
	char *		log_file;	/* Log file			*/
	uint16_t        msg_timeout;    /* message timeout		*/
	char *		pid_file;	/* where to store current PID	*/
	char *		plugindir;	/* dir to look for plugins	*/
	uint16_t        private_data;   /* restrict information         */
					/* purge variable format
					 * controlled by PURGE_FLAGS	*/
	uint32_t        purge_event;    /* purge events older than
					 * this in months or days 	*/
	uint32_t	purge_job;	/* purge time for job info	*/
	uint32_t	purge_step;	/* purge time for step info	*/
	uint32_t        purge_suspend;  /* purge suspend data older
					 * than this in months or days	*/
	uint32_t	slurm_user_id;	/* uid of slurm_user_name	*/
	char *		slurm_user_name;/* user that slurmcdtld runs as	*/
	char *		storage_backup_host;/* backup host where DB is
					     * running */
	char *		storage_host;	/* host where DB is running	*/
	char *		storage_loc;	/* database name		*/
	char *		storage_pass;   /* password for DB write	*/
	uint16_t	storage_port;	/* port DB is listening to	*/
	char *		storage_type;	/* DB to be used for storage	*/
	char *		storage_user;	/* user authorized to write DB	*/
	uint16_t        track_wckey;    /* Whether or not to track wckey*/
} slurm_dbd_conf_t;

extern pthread_mutex_t conf_mutex;
extern slurm_dbd_conf_t *slurmdbd_conf;


/*
 * free_slurmdbd_conf - free storage associated with the global variable
 *	slurmdbd_conf
 */
extern void free_slurmdbd_conf(void);

/* Return the DbdPort value */
extern uint16_t get_dbd_port(void);

/* lock and unlock the dbd_conf */
extern void slurmdbd_conf_lock(void);
extern void slurmdbd_conf_unlock(void);

/* Log the current configuration using verbose() */
extern void log_config(void);

/*
 * read_slurmdbd_conf - load the SlurmDBD configuration from the slurmdbd.conf
 *	file. This function can be called more than once if so desired.
 * RET SLURM_SUCCESS if no error, otherwise an error code
 */
extern int read_slurmdbd_conf(void);

/* Dump the configuration in name,value pairs for output to
 *	"sacctmgr show config", caller must call list_destroy() */
extern List dump_config(void);

#endif /* !_DBD_READ_CONFIG_H */
