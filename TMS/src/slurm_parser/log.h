
#ifndef _LOG_H
#define _LOG_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef HAVE_SYS_SYSLOG_H
#  include <sys/syslog.h>
#endif

#include <syslog.h>
#include <stdio.h>

/* supported syslog facilities and levels */
typedef enum {
	SYSLOG_FACILITY_DAEMON = 	LOG_DAEMON,
	SYSLOG_FACILITY_USER = 		LOG_USER,
	SYSLOG_FACILITY_AUTH = 		LOG_AUTH,
#ifdef LOG_AUTHPRIV
	SYSLOG_FACILITY_AUTHPRIV =	LOG_AUTHPRIV,
#endif
	SYSLOG_FACILITY_LOCAL0 =	LOG_LOCAL0,
	SYSLOG_FACILITY_LOCAL1 =	LOG_LOCAL1,
	SYSLOG_FACILITY_LOCAL2 =	LOG_LOCAL2,
	SYSLOG_FACILITY_LOCAL3 =	LOG_LOCAL3,
	SYSLOG_FACILITY_LOCAL4 =	LOG_LOCAL4,
	SYSLOG_FACILITY_LOCAL5 =	LOG_LOCAL5,
	SYSLOG_FACILITY_LOCAL6 =	LOG_LOCAL6,
	SYSLOG_FACILITY_LOCAL7 =	LOG_LOCAL7
} 	log_facility_t;

/*
 * log levels, logging will occur at or below the selected level
 * QUIET disable logging completely.
 */
typedef enum {
	LOG_LEVEL_QUIET = 0,
	LOG_LEVEL_FATAL,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_INFO,
	LOG_LEVEL_VERBOSE,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_DEBUG2,
	LOG_LEVEL_DEBUG3,
	LOG_LEVEL_DEBUG4,
	LOG_LEVEL_DEBUG5,
	LOG_LEVEL_SCHED,
	LOG_LEVEL_END
}	log_level_t;


/*
 * log options: Each of stderr, syslog, and logfile can have a different level
 */
typedef struct {
	log_level_t stderr_level;   /* max level to log to stderr         */
	log_level_t syslog_level;   /* max level to log to syslog         */
	log_level_t logfile_level;  /* max level to log to logfile        */
	unsigned    prefix_level:1; /* prefix level (e.g. "debug: ") if 1 */
	unsigned    buffered:1;     /* Use internal buffer to never block */
} 	log_options_t;

/* some useful initializers for log_options_t
 */
#define LOG_OPTS_INITIALIZER	\
	{ LOG_LEVEL_INFO, LOG_LEVEL_INFO, LOG_LEVEL_INFO, 1, 0 }

#define LOG_OPTS_SYSLOG_DEFAULT	\
	{ LOG_LEVEL_QUIET, LOG_LEVEL_INFO, LOG_LEVEL_QUIET, 1, 0 }

#define LOG_OPTS_STDERR_ONLY	\
	{ LOG_LEVEL_INFO,  LOG_LEVEL_QUIET, LOG_LEVEL_QUIET, 1, 0 }

#define SCHEDLOG_OPTS_INITIALIZER	\
	{ LOG_LEVEL_QUIET, LOG_LEVEL_QUIET, LOG_LEVEL_QUIET, 0, 1 }

/*
 * initialize log module (called only once)
 *
 * example:
 *
 * To initialize log module to print fatal messages to stderr, and
 * all messages up to and including slurm_info() to syslog:
 *
 * log_options_t logopts = LOG_OPTS_INITIALIZER;
 * logopts.stderr_level  = LOG_LEVEL_FATAL;
 * logopts.syslog_level  = LOG_LEVEL_INFO;
 *
 * rc = slurm_log_init(argv[0], logopts, SYSLOG_FACILITY_DAEMON, NULL);
 *
 * log function automatically takes the basename() of argv0.
 */
int slurm_log_init(char *argv0, log_options_t opts,
	      log_facility_t fac, char *logfile);

/*
 * the following log a message to the log facility at the appropriate level:
 *
 * Messages do not need a newline!
 *
 * args are printf style with the following exceptions:
 *
 * fmt     expands to
 * ~~~~    ~~~~~~~~~~~
 * "%m" => strerror(errno)
 * "%t" => strftime "%x %X"  (locally preferred short date/time)
 * "%T" => strftime "%a %d %b %Y %H:%M:%S %z" (rfc822 date/time)
 */

/* slurm_fatal() aborts program unless NDEBUG defined
 * error() returns SLURM_ERROR
 */
void	slurm_fatal(const char *, ...);
int	slurm_error(const char *, ...);
void	slurm_info(const char *, ...);
void	slurm_verbose(const char *, ...) ;
void	slurm_debug(const char *, ...) ;
void	slurm_debug2(const char *, ...);

#endif /* !_LOG_H */
