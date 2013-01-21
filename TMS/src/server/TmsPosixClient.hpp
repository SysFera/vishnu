/**
 * \file TmsPosicClient.hpp
 * \brief This file contains the data and the clients for the posix parser.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#ifndef __TMSPOSIXCLIENT__HH__
#define __TMSPOSIXCLIENT__HH__

#include <time.h>

/**
 * \brief Signature for the socket communications
 */
static const char* signature = "TMS-POS";

/**
 * \brief Type of message for ping
 */
static const char* lb_req_echo   = "ECHO";
/**
 * \brief Type of message for submit
 */
static const char* lb_req_submit = "SUBMIT";
/**
 * \brief Type of message for cancel
 */
static const char* lb_req_cancel = "CANCEL";
/**
 * \brief Type of message for getting info
 */
static const char* lb_req_ginfo  = "GINFO";
/**
 * \brief Type of message for killing
 */
static const char* lb_req_kill   = "KILL";

/**
 * \brief The states of the processes for the posix server
 * \enum jobb_state
 */
enum job_state { DEAD=0, RUNNING, WAITING, TERMINATED, ZOMBIE, KILL, KILL9 } ;

/**
 * \brief Structure sent for echo message
 * \struct st_echo
 */
struct st_echo {
/**
 * \brief Data to display in echo
 */
  char data[128];
};

/**
 * \brief Structure sent for submit message
 */
struct st_submit {
/**
 * \brief the command
 */
  char cmd[256];
/**
 * \brief the name of the job
 */
  char name[256];
/**
 * \brief the wall time
 */
  time_t walltime;
/**
 * \brief the output path
 */
  char OutPutPath[256];
/**
 * \brief the error path
 */
  char ErrorPath[256];
/**
 * \brief the working dir
 */
  char WorkDir[256];
};

/**
 * \brief Structure sent for cancel messages
 * \struct st_cancel
 */
struct st_cancel {
/**
 * \brief the id of the job to cancel
 */
  char JobId[16];
};

/**
 * \brief Structure sent for getting info messages
 * \struct st_info
 */
struct st_info {
/**
 * \brief The id of the job
 */
  char JobId[16];
};

/**
 * \brief Structure representinf a whole message
 */
struct Request {
/**
 * \brief The signal
 */
  char sig[8];
/**
 * \brief The request type
 */
  char req[8];
/**
 * \brief union containing the data for the request
 * \union req_data
 */
  union req_data {
/**
 * \brief the echo data
 */
    struct st_echo echo;
/**
 * \brief the submit data
 */
    struct st_submit submit;
/**
 * \brief the cancel data
 */
    struct st_cancel cancel;
/**
 * \brief the getinfo data
 */
    struct st_info info;
  } data;
};

/**
 * \brief Structure describing a job
 */
struct st_job {
/**
 * \brief the id of the job
 */
  char JobId[16];
/**
 * \brief the pid
 */
  pid_t pid;
/**
 * \brief the time the job starts
 */
  time_t startTime;
/**
 * \brief the max time the job can run
 */
  time_t maxTime;
/**
 * \brief the state of the job
 */
  enum job_state state;
/**
 * \brief the output path
 */
  char OutPutPath[256];
/**
 * \brief the error path
 */
  char ErrorPath[256];
/**
 * \brief the HomeDir
 */
  char HomeDir[256];
};

/**
 * \brief Structure containing the response from the posix server
 */
struct Response {
/**
 * \brief the status of the job
 */
  int status;
/**
 * \brief the data for the response
 * \union res_data
 */
  union res_data {
/**
 * \brief the echo data
 */
    struct st_echo echo;
/**
 * \brief the job data
 */
    struct st_job submit;
/**
 * \brief the jobinfo data
 */
    struct st_job info;
  } data;
};

/**
 * \brief low level function to send req to destination
 * \param destination where to send (socket unix)
 * \param req the request to send
 * \return 0 on success
 */
int
ReqSend(const char *destination, const struct Request* req);

/**
 * \brief function to submit a command with the option subs and tu get the response
 * \param command the job to submit
 * \param response out, the result of the submit
 * \param sub options to submit the job
 * \return 0 on success
 */
int
ReqSubmit(const char* command,struct st_job* response, struct st_submit* sub);

/**
 * \brief function to make a echo. Test function.
 * \param chaine the string to display
 * \param ret the return of the echo
 * \return 0 on success
 */
int
ReqEcho(const char* chaine, char* ret);

/**
 * \brief function to cancel a job
 * \param jobId the id of the job to cancel
 * \return 0 on success
 */
int
ReqCancel(const char* jobId);

/**
 * \brief function to get data about jobId
 * \param jobId the id of the job to get info about
 * \param response out, the data about the job
 * \return 0 on success
 */
int
ReqInfo(const char* jobId, struct st_job* reponse);

/**
 * \brief function to build the environnement before executing a job
 * \return 0 on success
 */
int
buildEnvironment();

/**
 * \brief function to launch the daemon that will handle the jobs for a given user
 */
void
LaunchDaemon();

#endif
