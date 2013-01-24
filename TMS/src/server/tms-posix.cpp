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
#include <cstring>
#include <unistd.h>
#include <climits>
#include <fcntl.h>
#include <csignal>
#include <sys/socket.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#include <cstdio>

#include <pwd.h>

#include "TmsPosixClient.hpp"

#include "POSIXParser.hpp"
#include "utils.hpp"

#include "utilVishnu.hpp"

using namespace std;
using namespace boost::system;


static const char* libBatchId = "VISHNU_BATCHJOB_ID";
static const char* libBatchName = "VISHNU_BATCHJOB_NAME";
static const char* libHostname = "VISHNU_SUBMIT_MACHINE_NAME";
static const char* libNodefile = "VISHNU_BATCHJOB_NODEFILE";
static const char* libNumNodes = "VISHNU_BATCHJOB_NUM_NODES";

static vector<struct st_job> Board;

static volatile int Terminated = 0;

static volatile sig_atomic_t ChildSigs = 0;
static volatile sig_atomic_t AlarmSig = 0;

static char HomeDir[255];

static void
CheckJobs() {
  int Taille;
  int i=0;
  bool CheckIn5s = false;

  Taille = Board.size();

  // End of Daemon ?
  if (Taille == 0) {
    Terminated = 1;
    return;
  }

  // Clean the Board off Deads Processes
  while (i < Taille) {
    for (i = 0; i<Taille; i++) {
      if (Board[i].state == TERMINATED) {
        Board.erase(Board.begin()+i);
        CheckIn5s = true;
        break;
      }
    }
    Taille = Board.size();
  }

  Taille = Board.size();

  // Manage Processes states
  for (i = 0; i<Taille; i++) {
    if (Board[i].state == KILL) {
      if (kill(Board[i].pid,0) == -1) {
        Board[i].state = TERMINATED;
        unlink(Board[i].ScriptPath);
      }
      CheckIn5s = true;
    }
  }

  // Checks for normal ends of process
  for (i = 0; i<Taille; i++) {
    if (kill(Board[i].pid,0) == -1) {
      Board[i].state = TERMINATED;
      unlink(Board[i].ScriptPath);
      CheckIn5s = true;
    }
  }

  // Groundwork for futurs signals
  if (CheckIn5s) {
    ChildSigs = 1;
    alarm(5);
  } else {
    ChildSigs = 0;
  }
}

static void
TimeStatement() {
  int Taille;
  int i;
  time_t now;
  time_t temp;
  time_t futur = 0;

  now = time(NULL);

  Taille = Board.size();

  // Manage Dangling Processes
  for (i = 0; i<Taille; i++) {
    if (Board[i].state == KILL) {
      if (kill(Board[i].pid,0) == -1) {
        Board[i].state = KILL9;
        kill(Board[i].pid,SIGKILL);
      } else {
        Board[i].state = TERMINATED;
        AlarmSig = 0;
      }
    }
  }

  if (AlarmSig == 0) {
    alarm(5);
    return;
  }

  // Manage walclocklimit
  for (i = 0; i<Taille; i++) {
    if (Board[i].maxTime != 0) {
      if ( (Board[i].startTime + Board[i].maxTime) <= now) {
        Board[i].state = KILL;
        kill(Board[i].pid,SIGTERM);
      } else {
        temp = (Board[i].startTime + Board[i].maxTime) - now;
        if (futur == 0) {
          futur = temp;
        } else {
          if (temp < futur) {
            futur = temp;
          }
        }
      }
    }
  }
 
  if (futur != 0) {
    alarm(futur);
  }
  AlarmSig = 0;

}

static void
sigchldHandler(int sig) {
  int status;
  pid_t childPid;


  while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
    ChildSigs = 1;
  }
}

static void
sigalarmHandler(int sig) {
  AlarmSig = 1;
}


static int
Daemonize() {
  long maxfd;
  int fd;

  switch (fork()) {
    case -1:
      return -1;
    case 0:
      break;
    default:
      exit(EXIT_SUCCESS);
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

  umask(0);
  chdir("/");
  maxfd = sysconf(_SC_OPEN_MAX);
  if (maxfd == -1) {
    maxfd = 128;
  }

  for (fd = 3; fd<maxfd; fd++) {
    close(fd);
  }

  close(STDIN_FILENO);
  fd = open("/dev/null",O_RDWR);

  if (fd != STDIN_FILENO) {
    return -1;
  }

  (void)dup2(STDIN_FILENO,STDOUT_FILENO);
  (void)dup2(STDIN_FILENO,STDERR_FILENO);

  return 0;
}

int
buildEnvironment(){
  int fdHostname;
  boost::system::error_code ec;
  const string hostname = boost::asio::ip::host_name(ec);
  static const boost::filesystem::path templateHostname("/tmp/NODELIST_%%%%%%");

  // variable VISHNU_SUBMIT_MACHINE_NAME
  setenv(libHostname, hostname.c_str(), true);

  // variable VISHNU_BATCHJOB_NODEFILE
  boost::filesystem::path fileHostname = boost::filesystem::unique_path(templateHostname,ec);
  fdHostname = open(fileHostname.c_str(),O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
  write(fdHostname,hostname.c_str(),strlen(hostname.c_str()));
  close(fdHostname);
  setenv(libNodefile,fileHostname.c_str(),true);

  // variable VISHNU_BATCHJOB_NUM_NODES
  setenv(libNumNodes, "1", true);

  return 0;
}


static int
execCommand(const char* command,const char* fstdout, const char* fstderr, const char* working_dir, struct st_job* current, int maxTime) {
  char* args[16];
  char commandLine[255];
  pid_t pid;
  ostringstream temp;
  int fd;
  boost::filesystem::path p;

  args[1] = (char *)"/bin/sh";
  args[2] = (char *)"-c";
  strcpy(commandLine,"exec ");
  strncat(commandLine,command,sizeof(commandLine)-strlen(commandLine)-1);
  args[3] = commandLine;
  args[4] = NULL;
  args[0] = args[1];

  if (strlen(working_dir) != 0) {
    if (chdir(working_dir) < 0) {
      chdir(HomeDir);
    }
  }

  getcwd(current->HomeDir,sizeof(current->HomeDir));

  memset(current->JobId,0,sizeof current->JobId);

  if ((pid = fork()) == 0) {
    pid = getpid();
    temp<<pid;
    setenv(libBatchId,temp.str().c_str(),true);

    if (fstdout != NULL) {
      fd = open(fstdout,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
      close(STDOUT_FILENO);
      dup2(fd,STDOUT_FILENO);
      close(fd);
    }

    if (fstderr != NULL) {
      fd = open(fstderr,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
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
  current->maxTime = maxTime;

  p = boost::filesystem::path(fstdout);

  if ( p.is_absolute() ) {
    strncpy(current->OutPutPath,fstdout,sizeof(current->OutPutPath));
  } else {
    snprintf(current->OutPutPath,sizeof(current->OutPutPath),"%s/%s",current->HomeDir,fstdout);
  }

  p = boost::filesystem::path(fstderr);
 
  if ( p.is_absolute() ) {
    strncpy(current->ErrorPath,fstderr,sizeof(current->ErrorPath));
  } else {
    snprintf(current->ErrorPath,sizeof(current->ErrorPath),"%s/%s",current->HomeDir,fstderr);
  }

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
  const char* stest = "COUCOU";
  char rtest[128];

  ret = stat(socketName,&st_info);
  if (ret == 0) {
    ret2 = ReqEcho(stest,rtest);
    if (ret2==0) {
      return -3;
    }
  } else if (ret < 0) {
    sv_errno = errno;

    if (sv_errno != ENOENT) {
        return -3;
    }
  }

  // Socket UNIX
  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    return -2;
  }

  if ( (unlink(socketName) == -1) && errno != ENOENT) {
    return -3;
  }

  memset(&addr, 0, sizeof(struct sockaddr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socketName, sizeof(addr.sun_path)-1);

  if (::bind(sfd, (struct sockaddr *)&addr,sizeof(struct sockaddr_un)) == -1) {
    return -4;
  }

  if (listen(sfd, 5) == -1) {
    return -5;
  }

  return sfd;
}

static int
AcceptRequest(int sfd, struct Request* req) {
  int cfd;

  while ( (cfd = accept(sfd,NULL,NULL)) < 0) {
    if (errno != EINTR) {
      return -7;
    }
    if ( ChildSigs == 1) {
      CheckJobs();
    }
    if ( AlarmSig == 1) {
      TimeStatement();
    }
    if (Terminated == 1) {
      return -1;
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
  char fout[256];
  char ferr[256];
  int wallclocklimit;
  std::map<std::string, std::string> context;
  boost::system::error_code ec;
  static const boost::filesystem::path foutName("VISHNU-%%%%%%.out");
  boost::filesystem::path fileOut = boost::filesystem::unique_path(foutName,ec);
  static const boost::filesystem::path ferrName("VISHNU-%%%%%%.err");
  boost::filesystem::path fileErr = boost::filesystem::unique_path(ferrName,ec);
  static const boost::filesystem::path scriptName("/tmp/VISHNU-script%%%%%.sh");
  boost::filesystem::path fileScript = boost::filesystem::unique_path(scriptName,ec);
  boost::filesystem::path tmpScript;

  sigemptyset(&emptyMask);

  sigemptyset(&blockMask);
  sigaddset(&blockMask, SIGCHLD);

  POSIXParser::parseFile(req->data.submit.cmd, context);

  if (strlen(req->data.submit.OutPutPath) != 0) {
    strncpy(fout, req->data.submit.OutPutPath, sizeof(fout));
  } else if (context.find("vishnu_output") != context.end()) {
    strncpy(fout, context["vishnu_output"].c_str(), sizeof(fout));
  } else {
    strncpy(fout,fileOut.c_str(),sizeof(fout));
  }

  if (strlen(req->data.submit.ErrorPath) != 0) {
    strncpy(ferr, req->data.submit.ErrorPath, sizeof(ferr));
  } else if (context.find("vishnu_error") != context.end()) {
    strncpy(ferr, context["vishnu_error"].c_str(), sizeof(ferr));
  } else {
    strncpy(ferr,fileErr.c_str(),sizeof(ferr));
  }

  sigprocmask(SIG_SETMASK, &blockMask, NULL);

  if (req->data.submit.walltime > 0) {
    wallclocklimit = req->data.submit.walltime;
  } else if (context.find("vishnu_wallclocklimit") != context.end()) {
    wallclocklimit = vishnu::convertStringToWallTime(context["vishnu_wallclocklimit"]);
  } else {
    wallclocklimit = 0;
  }

  if (wallclocklimit <0) {
    wallclocklimit = 0;
  }

  buildEnvironment();

  tmpScript = req->data.submit.cmd;

  boost::filesystem::copy_file(tmpScript, fileScript,
                               boost::filesystem::copy_option::overwrite_if_exists,ec);

  setenv(libBatchName, req->data.submit.JobName, true);

  if (context.find("vishnu_working_dir")  != context.end()) {
    execCommand(fileScript.c_str(), fout, ferr,
                context["vishnu_working_dir"].c_str(), &currentState, wallclocklimit);
  } else {
    execCommand(fileScript.c_str(), fout, ferr, HomeDir, &currentState, wallclocklimit);
  }

  sigprocmask(SIG_SETMASK, &emptyMask, NULL);

  strncpy(currentState.ScriptPath,fileScript.c_str(),sizeof(currentState.ScriptPath));
  Board.push_back(currentState);

  AlarmSig = 1;

  memcpy(&(ret->data.submit),&currentState,sizeof(struct st_job));

  return 0;
}

static int
RequestCancel(struct Request* req, struct Response* ret) {
  char* JobId;
  int i;
  int Taille;

  JobId = req->data.cancel.JobId;

  Taille = Board.size();

  for (i = 0; i<Taille; i++) {
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
  struct st_job NoJob;

  JobId = req->data.info.JobId;

  Taille = Board.size();

  memset(&NoJob,0,sizeof(struct st_job));
  NoJob.state = DEAD;

  for (i = 0; i<Taille; i++) {
    if (strncmp(JobId,Board[i].JobId,sizeof(Board[i].JobId)) == 0) {
      memcpy(&(ret->data.info),&(Board[i]), sizeof(struct st_job));
      return 0;
    }
  }
  memcpy(&(ret->data.info),&NoJob,sizeof(struct st_job));

  return 0;
}

static int
RequestGetStartTime(struct Request* req, struct Response* ret) {
  return 0;
}

static int
RequestKill(struct Request* req, struct Response* ret) {
  Terminated = 1;
  return 0;
}


void
LaunchDaemon() {
  struct sigaction sa;
  int sfd;
  int cfd;
  struct Request req;
  struct Response ret;
  const char* sv_sock =  "tms-posix-socket-";
  char name_sock[255];
  uid_t euid;
  struct passwd* lpasswd;

  char buffer[255];


  euid = geteuid();

  lpasswd = getpwuid(euid);
  if ( lpasswd == NULL) {
   perror("Erreur passwd");
   exit(1);
  }

  strncpy(HomeDir,lpasswd->pw_dir,sizeof(HomeDir));

  snprintf(name_sock,sizeof(name_sock),"%s/%s%d","/tmp",sv_sock,euid);


  Daemonize();

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = sigchldHandler;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    exit(6);
  }

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = sigalarmHandler;
  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    exit(6);
  }

  sfd = OpenSocketServer(name_sock);
  if (sfd < 0) {
    exit(-sfd);
  }

  for (Terminated = 0; Terminated == 0; ) {
    cfd = AcceptRequest(sfd, &req);

    if (cfd < 0) {
      break;
    }

    if (strncmp(req.sig,signature,sizeof(req.sig)) != 0) {
      continue;
    }

    if (strncmp(req.req,lb_req_echo,sizeof(req.req)) == 0) {
      ret.status = RequestEcho(&req,&ret);
    }
    if (strncmp(req.req,lb_req_submit,sizeof(req.req)) == 0) {
      ret.status = RequestSubmit(&req,&ret);
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
  unlink(name_sock);
}

