#ifndef __TMSPOSIXCLIENT__HH__
#define __TMSPOSIXCLIENT__HH__

#include <time.h>

static const char* signature = "TMS-POS";

static const char* lb_req_echo   = "ECHO";
static const char* lb_req_submit = "SUBMIT";
static const char* lb_req_cancel = "CANCEL";
static const char* lb_req_ginfo  = "GINFO";
static const char* lb_req_kill   = "KILL";

enum job_state { DEAD=0, RUNNING, WAITING, TERMINATED, ZOMBIE, KILL, KILL9 } ;

struct st_echo {
  char data[128];
};

struct st_submit {
  char cmd[256];
  char name[256];
  time_t walltime;
  char OutPutPath[256];
  char ErrorPath[256];
  char WorkDir[256];
};

struct st_cancel {
  char JobId[16];
};

struct st_info {
  char JobId[16];
};

struct Request {
  char sig[8];
  char req[8];
  union req_data {
    struct st_echo echo;
    struct st_submit submit;
    struct st_cancel cancel;
    struct st_info info;
  } data;
};

struct st_job {
  char JobId[16];
  pid_t pid;
  time_t startTime;
  time_t maxTime;
  enum job_state state;
};

struct Response {
  int status;
  union res_data {
    struct st_echo echo;
    struct st_job submit;
    struct st_job info;
  } data;
};

int ReqSend(const char *destination, const struct Request* req);

int ReqSubmit(const char* command,struct st_job* response, struct st_submit* sub);
int ReqEcho(const char* chaine, char* ret);
int ReqCancel(const char* JobId);
int ReqInfo(const char* JobId, struct st_job* reponse);

int buildEnvironment();
void LaunchDaemon();

#endif
