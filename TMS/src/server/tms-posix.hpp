#ifndef __TMS_POSIX__HH__
#define __TMS_POSIX__HH__


enum job_state { DEAD=0, RUNNING, WAITING, TERMINATED, ZOMBIE, KILL, KILL9 } ;

struct st_job {
  char JobId[16];
  pid_t pid;
  time_t startTime;
  time_t maxTime;
  enum job_state state;
};

int buildEnvironment();
void LaunchDaemon();


int ReqSubmit(const char* command,struct st_job* response);
int ReqEcho(const char* chaine, char* ret);
int ReqCancel(const char* JobId);
int ReqInfo(const char* JobId, struct st_job* reponse, struct st_submit* sub);
#endif
