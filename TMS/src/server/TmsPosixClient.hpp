/**
 * \file TmsPosixClient.hpp
 * \brief This file contains the data and the clients for the posix parser.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#ifndef __TMSPOSIXCLIENT__HH__
#define __TMSPOSIXCLIENT__HH__

#include <time.h>
#include <string>

/**
 * \brief Signature for the socket communications
 */
static const char *SIGNATURE     = "TMS-POS";

/**
 * \brief Type of message for ping
 */
static const char *LB_REQ_ECHO   = "ECHO";
/**
 * \brief Type of message for submit
 */
static const char *LB_REQ_SUBMIT = "SUBMIT";
/**
 * \brief Type of message for cancel
 */
static const char *LB_REQ_CANCEL = "CANCEL";
/**
 * \brief Type of message for getting info
 */
static const char *LB_REQ_GINFO  = "GINFO";
/**
 * \brief Type of message for killing
 */
static const char *LB_REQ_KILL   = "KILL";

/**
 * \brief Template name of socket UNIX
 */
static const char* SV_SOCK =  "tms-posix-socket-";

/**
 * \brief The states of the processes for the posix server
 * \enum job_state_t
 */
enum job_state_t { DEAD=0, RUNNING, WAITING, TERMINATED, ZOMBIE, KILL, KILL9 } ;

/**
 * \brief Structure sent for echo message
 */
struct trameEcho {
/**
 * \brief Data to display in echo
 */
  char data[128];
};

/**
 * \brief Structure sent for submit message
 * \struct trameSubmit
 */
struct trameSubmit {
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
  char outPutPath[256];
/**
 * \brief the error path
 */
  char errorPath[256];
/**
 * \brief the working dir
 */
  char workDir[256];
/**
 * \brief the Job name
 */
  char jobName[256];
};

/**
 * \brief Structure sent for cancel messages
 * \struct trameCancel
 */
struct trameCancel {
/**
 * \brief the id of the job to cancel
 */
  char jobId[16];
};

/**
 * \brief Structure sent for getting info messages
 * \struct trameInfo
 */
struct trameInfo {
/**
 * \brief The id of the job
 */
  char jobId[16];
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
 * \union requestData
 */
  union requestData {
/**
 * \brief the echo data
 */
    struct trameEcho echo;
/**
 * \brief the submit data
 */
    struct trameSubmit submit;
/**
 * \brief the cancel data
 */
    struct trameCancel cancel;
/**
 * \brief the getinfo data
 */
    struct trameInfo info;
  } data;
};

/**
 * \brief Structure describing a job
 */
struct trameJob {
/**
 * \brief the id of the job
 */
  char jobId[16];
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
  enum job_state_t state;
/**
 * \brief the output path
 */
  char outPutPath[256];
/**
 * \brief the error path
 */
  char errorPath[256];
/**
 * \brief the homeDir
 */
  char homeDir[256];
/**
 * \brief the scriptPath
 */
  char scriptPath[255];
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
 * \union responseData
 */
  union responseData {
/**
 * \brief the echo data
 */
    struct trameEcho echo;
/**
 * \brief the job data
 */
    struct trameJob submit;
/**
 * \brief the jobinfo data
 */
    struct trameJob info;
  } data;
};

/**
 * \brief low level function to send req to destination
 * \param destination where to send (socket unix)
 * \param req the request to send
 * \return 0 on success
 */
int
reqSend(const std::string& destination, const struct Request *req);

/**
 * \brief function to submit a command with the option subs and tu get the response
 * \param command the job to submit
 * \param response out, the result of the submit
 * \param sub options to submit the job
 * \return 0 on success
 */
int
reqSubmit(const std::string& command, struct trameJob *response, struct trameSubmit *sub);

/**
 * \brief function to make a echo. Test function.
 * \param chaine the string to display
 * \param ret the return of the echo
 * \return 0 on success
 */
int
reqEcho(const std::string& chaine, char *ret);

/**
 * \brief function to cancel a job
 * \param jobId the id of the job to cancel
 * \return 0 on success
 */
int
reqCancel(const std::string& jobId);

/**
 * \brief function to get data about jobId
 * \param jobId the id of the job to get info about
 * \param response out, the data about the job
 * \return 0 on success
 */
int
reqInfo(const std::string& jobId, struct trameJob *response);

/**
 * \brief function to launch the daemon that will handle the jobs for a given user
 */
void
launchDaemon();

#endif
