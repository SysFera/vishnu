/**
 * \file tms-posix.cpp
 * \brief This file contains the TMS-Posix scheduler for local batch.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>

#include <stdio.h>

#include <syslog.h>
#include <stdarg.h>
#include <pwd.h>

#include "TmsPosixClient.hpp"

#include "POSIXParser.hpp"
#include "utils.hpp"

using namespace std;
using namespace boost::system;


static const char* libBatchId="VISHNU_BATCHJOB_ID";
static const char* libBatchName="VISHNU_BATCHJOB_NAME";
static const char* libHostname="VISHNU_SUBMIT_MACHINE_NAME";
static const char* libNodefile="VISHNU_BATCHJOB_NODEFILE";
static const char* libNumNodes="VISHNU_BATCHJOB_NUM_NODES";

static int LogLevel=LOG_INFO;

static vector<struct st_job> Board;

static volatile int Terminated = 0;
static volatile sig_atomic_t ChildSigs = 0;
static volatile sig_atomic_t AlarmSig = 0;
/**
 * \brief To show how to use tms-posix
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */

static void
usage(char* cmd)
{
  cerr << "Usage: " << cmd << " COMMANDE_TYPE[SUBMIT] <BatchType> <JobSerializedPath> <SlaveErrorPath> <JobUpdatedSerializedPath>";
  cerr << " <SubmitOptionsSerializedPath> <job_script_path>" << endl;
  cerr << "\t\t\t\t\t" << " or " << endl;
  cerr << "Usage: " << cmd << " COMMANDE_TYPE[CANCEL] <BatchType> <JobSerializedPath>  <SlaveErrorPath>" << endl;
  exit(EXIT_FAILURE);
}


static void
CheckJobs(void) {
  int Taille;
  int i;

  Taille = Board.size();
  i = 0;

  while (i != Taille) {
    for (i=0; i<Taille; i++) {
      if (kill(Board[i].pid,0) == -1) {
        Board.erase(Board.begin()+i);
        break;
      }
    }
  }
  ChildSigs = 0;
}

static void
TimeStatement(void) {

  alarm(10);
  AlarmSig = 0;
}

/***
  une version Boost ....
  Board.erase(remove_if(Board.begin(), Board.end(),
                        [](struct st_job elem) {
                          return kill(elem.pid,0) == -1;
                        }),
              Board.end());
*****/

static void
sigchldHandler(int sig) {
  int status;
  int svErrno;
  pid_t childPid;
//  sigset_t nmask, omask;

  svErrno = errno;

  while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
    // Traitement mort d'un processus
    // Section critique
/****
  sigfillset(&nmask);
  sigprocmask(SIG_BLOCK, &nmask, &omask);
*****/
  ChildSigs = 1;
//  sigprocmask(SIG_SETMASK, &omask, NULL);
  }

  errno = svErrno;
}

static void
sigalarmHandler(int sig) {
  int status;
  int svErrno;

  svErrno = errno;

  AlarmSig = 1;

  errno = svErrno;
}

static void
tms_posixLog(int loglevel, const char *s, ...) {
	va_list va_alist;
	char buf[256];
	sigset_t nmask, omask;

  if (loglevel >= LogLevel) {
    return;
  }
	va_start(va_alist, s);
	vsnprintf(buf, sizeof(buf), s, va_alist);
	va_end(va_alist);

  sigfillset(&nmask);
  sigprocmask(SIG_BLOCK, &nmask, &omask);
  openlog("tms-posix", 0, LOG_DAEMON);
  syslog(loglevel, "%s", buf);
  closelog();
  sigprocmask(SIG_SETMASK, &omask, NULL);
}


static int
Daemonize(void) {
  long maxfd;
  int fd;

  switch (fork()) {
    case -1:
      return -1;
    case 0:
      break;
    default:
      exit(EXIT_SUCCESS);
//      return 0;
  }

  if (setsid() == -1) {
    return -1;
  }

  switch (fork()) {
    case -1:
      return -1;
    case 0:
      break;
    default:
      exit(EXIT_SUCCESS);
  }

  (void)umask(0);
  (void)chdir("/");
  maxfd = sysconf(_SC_OPEN_MAX);
  if (maxfd == -1) {
    maxfd=128;
  }

  for (fd=3; fd<maxfd; fd++) {
    close(fd);
  }

  close(STDIN_FILENO);
/****
  fd=open("/dev/null",O_RDWR);

  if (fd != STDIN_FILENO) {
    return -1;
  }

  (void)dup2(STDIN_FILENO,STDOUT_FILENO);
  (void)dup2(STDIN_FILENO,STDERR_FILENO);
 ****/
  return 0;
}

int
buildEnvironment(){
  //int i;
  int fdHostname;
  boost::system::error_code ec;
  const string hostname = boost::asio::ip::host_name(ec);
  static const boost::filesystem::path templateHostname("/tmp/NODELIST_%%%%%%");

  // variable VISHNU_SUBMIT_MACHINE_NAME
  (void)setenv(libHostname, hostname.c_str(), true);

  // variable VISHNU_BATCHJOB_NODEFILE
  boost::filesystem::path fileHostname = boost::filesystem::unique_path(templateHostname,ec);
  // permissions non dispo en boost 1.46
  // boost::filesystem::permissions(tfileHostname,boost::filesystem::owner_read|boost::filesystem::owner_write,ec);
  // Donc, on reste POSIX
  fdHostname = open(fileHostname.c_str(),O_CREAT|O_EXCL|O_WRONLY,S_IRUSR|S_IWUSR);
  (void)write(fdHostname,hostname.c_str(),strlen(hostname.c_str()));
  (void)close(fdHostname);
  (void)setenv(libNodefile,fileHostname.c_str(),true);

  // variable VISHNU_BATCHJOB_NUM_NODES
  (void)setenv(libNumNodes, "1", true);

  return 0;
}


static int
execCommand(char* command,const char* fstdout, const char* fstderr, struct st_job* current) {
  char* args[16];
  char commandLine[255];
  pid_t pid;
  ostringstream temp;
  int fd;

  args[1]=(char *)"/bin/sh";
  args[2]=(char *)"-c";
  strcpy(commandLine,"exec ");
  strncat(commandLine,command,sizeof(commandLine)-strlen(commandLine)-1);
  args[3]=commandLine;
  args[4]=NULL;
  args[0]=args[1];

  printf("Exec:%s > %s 2> %s \n",commandLine,fstdout,fstderr);

  memset(current->JobId,0,sizeof current->JobId);

  if ((pid=fork()) == 0) {
    pid=getpid();
    temp<<pid;
    (void)setenv(libBatchId,temp.str().c_str(),true);

    if (fstdout != NULL) {
      fd=open(fstdout,O_CREAT|O_CLOEXEC|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
      close(STDOUT_FILENO);
      dup2(fd,STDOUT_FILENO);
      close(fd);
    }

    if (fstderr != NULL) {
      fd=open(fstderr,O_CREAT|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
      close(STDERR_FILENO);
      dup2(fd,STDERR_FILENO);
      close(fd);
    }

    execvp(args[1],args+1);
  }
  if (pid < 0) {
    return -1;
  }
  // get Job info
  snprintf(current->JobId,sizeof(current->JobId)-1,"%d-%d",geteuid(),pid);
  current->pid = pid;
  current->startTime = time(NULL);
  current->state = RUNNING;
  current->maxTime = 0;

  printf("JobId:%s.\n",current->JobId);
  return 0;
}

static int
OpenSocketServer(const char* socketName) {
  int sfd;
  struct sockaddr_un addr;
  struct stat st_info;
  int ret;
  int ret2;
  int sv_errno;
  const char* stest="COUCOU";
  char rtest[128];

  ret = stat(socketName,&st_info);
  printf("stat:%d\n",ret);
  if (ret == 0) {
    printf("stest:%s\n",stest);
    ret2 = ReqEcho(stest,rtest);
    printf("ret:%d\n",ret2);
    if (ret2==0) {
      printf("rtest:%s\n",rtest);
      return -3;
    }
    printf("ret2:%d\n",ret2);
  } else if (ret < 0) {
    sv_errno=errno;

    perror("Erreur stat ");

    if (sv_errno != ENOENT) {
        return -3;
    }
  }

  printf("Start serveur.\n");

  // Socket UNIX
  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    tms_posixLog(LOG_DEBUG, "Error creating socket : %s",strerror(errno));
    return -2;
  }

  printf("Unlink\n");

  if ( (unlink(socketName) == -1) && errno != ENOENT) {
    tms_posixLog(LOG_DEBUG, "Error removing socket file : %s",strerror(errno));
    return -3;
  }

  printf("Bind:%s\n",socketName);
  memset(&addr, 0, sizeof(struct sockaddr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socketName, sizeof(addr.sun_path)-1);

  if (::bind(sfd, (struct sockaddr *)&addr,sizeof(struct sockaddr_un)) == -1) {
    tms_posixLog(LOG_DEBUG, "Error binding socket : %s",strerror(errno));
    return -4;
  }

  printf("Listen\n");
  if (listen(sfd, 5) == -1) {
    tms_posixLog(LOG_DEBUG, "Error listening socket : %s",strerror(errno));
    return -5;
  }

  return sfd;
}

static int
AcceptRequest(int sfd, struct Request* req) {
  int cfd;

  while ( (cfd = accept(sfd,NULL,NULL)) < 0) {
    if (errno != EINTR) {
      tms_posixLog(LOG_DEBUG, "Error accept socket:%s",strerror(errno));
      return -7;
    }
    if ( ChildSigs == 1) {
      CheckJobs();
    }
    if ( AlarmSig == 1) {
      TimeStatement();
    }
  }

  // A voir gestion retour et erreur
  read(cfd, req, sizeof(struct Request));

  return cfd;
}

static int
RequestEcho(struct Request* req, struct Response* ret) {
  memcpy(ret->data.echo.data,req->data.echo.data,sizeof(ret->data.echo));
  return 0;
}

static int
RequestSubmit(struct Request* req, struct Response* ret) {
  sigset_t blockMask;
  sigset_t emptyMask;
  struct st_job currentState;
  JobCtx Context;

  sigemptyset(&emptyMask);

  sigemptyset(&blockMask);
  sigaddset(&blockMask, SIGCHLD);

  ParseCommand(req->data.submit.cmd, Context);

  tms_posixLog(LOG_INFO, "Starting shell : %s",req->data.submit.cmd);

  sigprocmask(SIG_SETMASK, &blockMask, NULL);
  // TODO: Prendre en compte le contexte
  (void)execCommand(req->data.submit.cmd,"/tmp/SORTIE","/tmp/Erreurs",&currentState);
  sigprocmask(SIG_SETMASK, &emptyMask, NULL);

  Board.push_back(currentState);

  memcpy(&(ret->data.submit),&currentState,sizeof ret->data.submit);

  return 0;
}

static int
RequestCancel(struct Request* req, struct Response* ret) {
  char* JobId;
  int i;
  int Taille;

  JobId = req->data.cancel.JobId;

  Taille = Board.size();

  for (i=0; i<Taille; i++) {
    if (strncmp(JobId,Board[i].JobId,sizeof(Board[i].JobId)) == 0) {
      kill(Board[i].pid,SIGTERM);
      Board[i].state = KILL;
    }
  }

  return 0;
}

static int
RequestGetInfo(struct Request* req, struct Response* ret) {
  char* JobId;
  int Taille;
  int i;

  JobId = req->data.cancel.JobId;

  Taille = Board.size();

  for (i=0; i<Taille; i++) {
    if (strncmp(JobId,Board[i].JobId,sizeof(Board[i].JobId)) == 0) {
      memcpy(&(ret->data.info),&(Board[i]), sizeof(struct st_job));
      return 0;
    }
  }
  return -1;
}

static int
RequestGetStartTime(struct Request* req, struct Response* ret) {
  return 0;
}

static int
RequestKill(struct Request* req, struct Response* ret) {
  Terminated=1;
  return 0;
}

void
LaunchDaemon() {
  struct sigaction sa;
  int sfd;
  int cfd;
  struct Request req;
  struct Response ret;
  const char* sv_sock= "tms-posix-socket-";
  char name_sock[255];
  uid_t euid;
  struct passwd* lpasswd;

  tms_posixLog(LOG_INFO, "Starting tms-posix monitoring daemon");

  Board.reserve(10);

  euid = geteuid();
  printf("euid:%d\n",euid);
  lpasswd=getpwuid(euid);
  if ( lpasswd == NULL) {
   perror("Erreur passwd");
   exit(1);
  }
  printf("Home:%s\n",lpasswd->pw_dir);

  snprintf(name_sock,sizeof(name_sock),"%s/%s%d","/tmp",sv_sock,euid);
  printf("Socket:%s\n",name_sock);

  Daemonize();

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = sigchldHandler;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    tms_posixLog(LOG_DEBUG, "Error signal handler : %s",strerror(errno));
    exit(6);
  }

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = sigalarmHandler;
  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    tms_posixLog(LOG_DEBUG, "Error signal handler : %s",strerror(errno));
    exit(6);
  }

  sfd = OpenSocketServer(name_sock);
  if (sfd < 0) {
    exit(-sfd);
  }

  alarm(10);   // test

  for (Terminated = 0; Terminated == 0; ) {
    cfd = AcceptRequest(sfd, &req);

    if (cfd < 0) {
      exit(-cfd);
    }

    if (strncmp(req.sig,signature,sizeof(req.sig)) != 0) {
      continue;
    }

    if (strncmp(req.req,lb_req_echo,sizeof(req.req)) == 0) {
      ret.status = RequestEcho(&req,&ret);
    }
    if (strncmp(req.req,lb_req_submit,sizeof(req.req)) == 0) {
      ret.status = RequestSubmit(&req,&ret);
      Board.push_back(ret.data.submit);
    }
    if (strncmp(req.req,lb_req_cancel,sizeof(req.req)) == 0) {
      ret.status = RequestCancel(&req,&ret);
    }
    if (strncmp(req.req,lb_req_ginfo,sizeof(req.req)) == 0) {
      ret.status = RequestGetInfo(&req,&ret);
    }
    if (strncmp(req.req,lb_req_kill,sizeof(req.req)) == 0) {
      ret.status = RequestKill(&req,&ret);
    }

    write(cfd,&ret,sizeof(struct Response));

    close(cfd);

    if ( ChildSigs == 1) {
      CheckJobs();
    }
    if ( AlarmSig == 1) {
      TimeStatement();
    }
  }
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */

/**************

int
main(int argc, char* argv[], char* envp[])
{
  char myName[255];  // the Name of the program (and not the path)

  // Get ProcName
  strncpy(myName,GetProcName(argv[0]),sizeof(myName));

  SetProcName(argc,argv,"tms-posix [Ready]");

  (void)buildEnvironment();

  if (argc < 2)
    usage(myName);

  LaunchDaemon();

  return EXIT_SUCCESS;
} // End : main ()
*******************/
