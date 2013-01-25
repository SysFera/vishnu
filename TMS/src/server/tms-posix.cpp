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


static const char* LIB_BATCH_ID = "VISHNU_BATCHJOB_ID";
static const char* LIB_BATCH_NAME = "VISHNU_BATCHJOB_NAME";
static const char* LIB_HOSTNAME = "VISHNU_SUBMIT_MACHINE_NAME";
static const char* LIB_NODEFILE = "VISHNU_BATCHJOB_NODEFILE";
static const char* LIB_NUM_NODES = "VISHNU_BATCHJOB_NUM_NODES";

static vector<struct trameJob> Board;

static volatile bool terminated = false;

static volatile sig_atomic_t ChildSigs = 0;
static volatile sig_atomic_t AlarmSig = 0;

static char homeDir[255];

/*
 * Function SignalHandler for SIGCHLD
 */

static bool isDead(const struct trameJob& test) {
  return (test.state == TERMINATED);
}

static void
checkJobs() {
  int taille;
  bool checkIn5s = false;
  vector<struct trameJob>::iterator it;
 
  taille = Board.size();

  // End of Daemon ?
  if (taille == 0) {
    terminated = true;
    return;
  }

  // Clean the Board off Deads Processes
  Board.erase(std::remove_if(Board.begin(), Board.end(), isDead), Board.end());

  if (taille != Board.size()) {
    checkIn5s = true;
  }
 
  // Manage Processes states
  for (it = Board.begin(); it != Board.end(); ++it) {
    if (it->state == KILL) {
      if (kill(it->pid, 0) == -1) {
        it->state = TERMINATED;
        unlink(it->scriptPath);
      }
    }
  }

  // Checks for normal ends of process
  for (it = Board.begin(); it != Board.end(); ++it) {
    if ( kill(it->pid,0) == -1 ) {
      it->state = TERMINATED;
      unlink(it->scriptPath);
      checkIn5s = true;
    }
  }

  // Groundwork for futurs signals
  if (checkIn5s) {
    ChildSigs = 1;
    alarm(5);
  } else {
    ChildSigs = 0;
  }
}

/*
 * Function signal handler SIGALARM
 */
static void
timeStatement() {
  time_t now;
  time_t tmp;
  time_t futur = 0;
  vector<struct trameJob>::iterator it;

  now = time(NULL);

  // Manage Dangling Processes
  for (it = Board.begin(); it != Board.end(); ++it) {
    if (it->state == KILL) {
      if (kill(it->pid,0) == -1) {
        it->state = KILL9;
        kill(it->pid,SIGKILL);
      } else {
        it->state = TERMINATED;
        AlarmSig = 0;
      }
    }
  }

  if (AlarmSig == 0) {
    alarm(5);
    return;
  }

  // Manage walclocklimit
  for (it = Board.begin(); it != Board.end(); ++it) {
    if (it->maxTime != 0) {
      if ( (it->startTime + it->maxTime) <= now) {
        it->state = KILL;
        kill(it->pid,SIGTERM);
      } else {
        tmp = (it->startTime + it->maxTime) - now;
        if (futur == 0) {
          futur = tmp;
        } else {
          if (tmp < futur) {
            futur = tmp;
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

/*
 * Function System sigchild handler
 */

static void
sigchldHandler(int sig) {
  int status;
  pid_t childPid;


  while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
    ChildSigs = 1;
  }
}

/*
 * Function System sigalarm handler
 */

static void
sigalarmHandler(int sig) {
  AlarmSig = 1;
}


static int
daemonize() {
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

  dup2(STDIN_FILENO,STDOUT_FILENO);
  dup2(STDIN_FILENO,STDERR_FILENO);

  return 0;
}

static void
buildEnvironment() {
  int fdHostname;
  boost::system::error_code ec;
  const string hostname = boost::asio::ip::host_name(ec);
  static const boost::filesystem::path templateHostname("/tmp/NODELIST_%%%%%%");

  // variable VISHNU_SUBMIT_MACHINE_NAME
  setenv(LIB_HOSTNAME, hostname.c_str(), true);

  // variable VISHNU_BATCHJOB_NODEFILE
  boost::filesystem::path fileHostname = boost::filesystem::unique_path(templateHostname,ec);
  fdHostname = open(fileHostname.c_str(),O_CREAT|O_WRONLY,S_IRUSR|S_IWUSR);
  write(fdHostname,hostname.c_str(),strlen(hostname.c_str()));
  close(fdHostname);
  setenv(LIB_NODEFILE,fileHostname.c_str(),true);

  // variable VISHNU_BATCHJOB_NUM_NODES
  setenv(LIB_NUM_NODES, "1", true);
}


static int
execCommand( const boost::filesystem::path &command,
             const boost::filesystem::path &fstdout,
             const boost::filesystem::path &fstderr,
             const boost::filesystem::path workingDir,
             struct trameJob *current, int maxTime) {
  char* args[5];
  std::string commandLine;
  std::string envJobId;
  pid_t pid;
  int fd;

  args[1] = const_cast<char *>("/bin/sh");
  args[2] = const_cast<char*>("-c");
  commandLine = "exec ";
  commandLine.append(command.string());
  args[3] = const_cast<char *>(commandLine.c_str());
  args[4] = NULL;
  args[0] = args[1];

  if ( ! workingDir.empty() ) {
    if (chdir(workingDir.c_str()) < 0) {
      chdir(homeDir);
    }
  }

  getcwd(current->homeDir, sizeof(current->homeDir));

  memset(current->jobId, 0, sizeof current->jobId);

  if ((pid = fork()) == 0) {
    envJobId = boost::lexical_cast<string>(geteuid());
    envJobId.push_back('-');
    envJobId.append(boost::lexical_cast<string>(getpid()));

    setenv(LIB_BATCH_ID, envJobId.c_str(), true);

    if ( ! fstdout.empty() ) {
      fd = open(fstdout.c_str(), O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
      dup2(fd,STDOUT_FILENO);
      close(fd);
    }

    if ( ! fstderr.empty() ) {
      fd = open(fstderr.c_str(), O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
      dup2(fd,STDERR_FILENO);
      close(fd);
    }

    execvp(args[1],args+1);
  }
  if (pid < 0) {
    return -1;
  }

  // get Job info
  snprintf(current->jobId, sizeof(current->jobId)-1, "%d-%d", geteuid(),pid);
  current->pid = pid;
  current->startTime = time(NULL);
  current->state = RUNNING;
  current->maxTime = maxTime;

  if ( fstdout.is_absolute() ) {
    strncpy(current->outPutPath, fstdout.c_str(), sizeof(current->outPutPath));
  } else {
    snprintf(current->outPutPath, sizeof(current->outPutPath), "%s/%s", current->homeDir, fstdout.c_str());
  }

  if ( fstderr.is_absolute() ) {
    strncpy(current->errorPath, fstderr.c_str(), sizeof(current->errorPath));
  } else {
    snprintf(current->errorPath, sizeof(current->errorPath), "%s/%s", current->homeDir, fstderr.c_str());
  }

  return 0;
}

static int
openSocketServer(const char* socketName) {
  int sfd;
  struct sockaddr_un addr;
  struct stat info;
  int ret;
  int ret2;
  int sv_errno;
  const char* stest = "COUCOU";
  char rtest[128];

  ret = stat(socketName, &info);
  if (ret == 0) {
    ret2 = reqEcho(stest,rtest);
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

  if (::bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
    return -4;
  }

  if (listen(sfd, 5) == -1) {
    return -5;
  }

  return sfd;
}

static int
acceptRequest(int sfd, struct Request* req) {
  int cfd;

  while ( (cfd = accept(sfd,NULL,NULL)) < 0) {
    if (errno != EINTR) {
      return -7;
    }
    if ( ChildSigs == 1) {
      checkJobs();
    }
    if ( AlarmSig == 1) {
      timeStatement();
    }
    if (terminated) {
      return -1;
    }
  }

  // A voir gestion retour et erreur
  read(cfd, req, sizeof(struct Request));

  return cfd;
}

static int
requestEcho(struct Request* req, struct Response* ret) {
  memcpy(ret->data.echo.data, req->data.echo.data, sizeof(ret->data.echo));
  return 0;
}



static int
requestSubmit(struct Request* req, struct Response* ret) {
  sigset_t blockMask;
  sigset_t emptyMask;
  struct trameJob currentState;
  boost::filesystem::path fout;
  boost::filesystem::path ferr;
  int wallclocklimit;
  std::map<std::string, std::string> context;
  boost::system::error_code ec;
  const boost::filesystem::path foutName("VISHNU-%%%%%%.out");
  boost::filesystem::path fileOut = boost::filesystem::unique_path(foutName,ec);
  const boost::filesystem::path ferrName("VISHNU-%%%%%%.err");
  boost::filesystem::path fileErr = boost::filesystem::unique_path(ferrName,ec);
  const boost::filesystem::path scriptName("/tmp/VISHNU-script%%%%%.sh");
  boost::filesystem::path fileScript = boost::filesystem::unique_path(scriptName,ec);
  boost::filesystem::path tmpScript;
  boost::filesystem::path workDir;

  sigemptyset(&emptyMask);

  sigemptyset(&blockMask);
  sigaddset(&blockMask, SIGCHLD);

  POSIXParser::parseFile(req->data.submit.cmd, context);

  if (strlen(req->data.submit.outPutPath) != 0) {
    fout = req->data.submit.outPutPath;
  } else if (context.find("vishnu_output") != context.end()) {
    fout = context["vishnu_output"];
  } else {
    fout = fileOut;
  }

  if (strlen(req->data.submit.errorPath) != 0) {
    ferr = req->data.submit.errorPath;
  } else if (context.find("vishnu_error") != context.end()) {
    ferr = context["vishnu_error"];
  } else {
    ferr = fileErr;
  }

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

  setenv(LIB_BATCH_NAME, req->data.submit.jobName, true);

  if (context.find("vishnu_working_dir") != context.end()) {
    workDir = context["vishnu_working_dir"];
  } else {
    workDir = homeDir;
  }

  sigprocmask(SIG_SETMASK, &blockMask, NULL);

  execCommand(fileScript, fout, ferr, workDir, &currentState, wallclocklimit);

  sigprocmask(SIG_SETMASK, &emptyMask, NULL);

  strncpy(currentState.scriptPath,fileScript.c_str(),sizeof(currentState.scriptPath));
  Board.push_back(currentState);

  AlarmSig = 1;

  ret->data.submit = currentState;

  return 0;
}

static int
requestCancel(struct Request* req, struct Response* ret) {
  char* jobId;
  int i;
  int taille;

  jobId = req->data.cancel.jobId;

  taille = Board.size();

  for (i = 0; i < taille; i++) {
    if (strncmp(jobId, Board[i].jobId, sizeof(Board[i].jobId)) == 0) {
      kill(Board[i].pid,SIGTERM);
      Board[i].state = KILL;
    }
  }

  return 0;
}

static int
requestGetInfo(struct Request* req, struct Response* ret) {
  char* jobId;
  int taille;
  int i;
  struct trameJob NoJob;

  jobId = req->data.info.jobId;

  taille = Board.size();

  memset(&NoJob,0,sizeof(struct trameJob));
  NoJob.state = DEAD;

  for (i = 0; i<taille; i++) {
    if (strncmp(jobId,Board[i].jobId,sizeof(Board[i].jobId)) == 0) {
      memcpy(&(ret->data.info),&(Board[i]), sizeof(struct trameJob));
      return 0;
    }
  }
  memcpy(&(ret->data.info),&NoJob,sizeof(struct trameJob));

  return 0;
}

static int
requestKill(struct Request* req, struct Response* ret) {
  terminated = true;
  return 0;
}


void
launchDaemon() {
  struct sigaction sa;
  int sfd;
  int cfd;
  struct Request req;
  struct Response ret;
  char name_sock[255];
  uid_t euid;
  struct passwd* lpasswd;

  euid = geteuid();

  lpasswd = getpwuid(euid);
  if ( lpasswd == NULL) {
   perror("Erreur passwd");
   exit(1);
  }

  strncpy(homeDir,lpasswd->pw_dir,sizeof(homeDir));

  snprintf(name_sock,sizeof(name_sock),"%s/%s%d","/tmp",SV_SOCK,euid);

  daemonize();

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

  sfd = openSocketServer(name_sock);
  if (sfd < 0) {
    exit(-sfd);
  }

  for (terminated = false; ! terminated ; ) {
    cfd = acceptRequest(sfd, &req);

    if (cfd < 0) {
      break;
    }

    if (strncmp(req.sig, SIGNATURE, sizeof(req.sig)) != 0) {
      continue;
    }

    if (strncmp(req.req, LB_REQ_ECHO, sizeof(req.req)) == 0) {
      ret.status = requestEcho(&req,&ret);
    }
    if (strncmp(req.req, LB_REQ_SUBMIT, sizeof(req.req)) == 0) {
      ret.status = requestSubmit(&req,&ret);
    }
    if (strncmp(req.req, LB_REQ_CANCEL, sizeof(req.req)) == 0) {
      ret.status = requestCancel(&req,&ret);
    }
    if (strncmp(req.req, LB_REQ_GINFO, sizeof(req.req)) == 0) {
      ret.status = requestGetInfo(&req,&ret);
    }
    if (strncmp(req.req, LB_REQ_KILL, sizeof(req.req)) == 0) {
      ret.status = requestKill(&req,&ret);
    }

    write(cfd,&ret,sizeof(struct Response));

    close(cfd);

    if ( ChildSigs == 1) {
      checkJobs();
    }
    if ( AlarmSig == 1) {
      timeStatement();
    }
  }
  unlink(name_sock);
}

