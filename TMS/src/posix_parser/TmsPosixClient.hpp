


static const char* signature = "TMS-POS";

static const char* lb_req_echo   = "ECHO";
static const char* lb_req_submit = "SUBMIT";
static const char* lb_req_cancel = "CANCEL";
static const char* lb_req_gstate = "GSTATE";
static const char* lb_req_gstime = "GSTIME";
static const char* lb_req_kill   = "KILL";

enum job_state { DEAD=0, RUNNING, WAITING, TERMINATED, ZOMBIE } ;

struct st_echo {
  char data[128];
};

struct st_submit {
  char cmd[256];
};

struct st_cancel {
  char JobId[16];
};

struct Request {
  char sig[8];
  char req[8];
  union req_data {
    struct st_echo echo;
    struct st_submit submit;
    struct st_cancel cancel;
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
  } data;
};

int ReqSend(const char *destination, const struct Request* req);
int ReqSubmit(const char* command,struct st_job* response);
int ReqEcho(const char* chaine, char* ret);
int ReqCancel(const char* JobId);
