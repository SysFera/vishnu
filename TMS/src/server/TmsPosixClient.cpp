
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#include <cstdio>

#include "TmsPosixClient.hpp"

int
ReqSend(const char *destination, const struct Request* req, struct Response* ret) {
  int sfd;
  struct sockaddr_un addr;
  int sv_errno;
  size_t nbCharwrite = sizeof(struct Request);
  ssize_t nbCharwriten = 0;

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    return -1;
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, destination, sizeof(addr.sun_path) -1);

  if ( connect(sfd,(struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
    sv_errno = errno;
    close(sfd); // a voir
    errno = sv_errno;
    return -2;
  }

  while (nbCharwrite > 0) {
    nbCharwriten = write(sfd,req,nbCharwrite);
    if (nbCharwriten < 0) {
      sv_errno = errno;
      close(sfd);
      errno = sv_errno;
      return -3;
    }
    nbCharwrite -= nbCharwriten;
  }

  read(sfd,ret,sizeof(struct Response));

  close(sfd);

  return 0;
}

int
ReqSubmit(const char* command, struct st_job* response, struct st_submit* sub) {
  struct Request req;
  struct Response ret;
  const char* sv_sock= "tms-posix-socket-";
  char name_sock[255];
  uid_t euid;
  struct passwd* lpasswd;

  euid = geteuid();
  printf("Client euid:%d\n",euid);
  lpasswd=getpwuid(euid);
  if ( lpasswd == NULL) {
   perror("Erreur passwd");
   return -1;
  }
  printf("Client Home:%s\n",lpasswd->pw_dir);

  snprintf(name_sock,sizeof(name_sock),"%s/%s%d","/tmp",sv_sock,euid);
  printf("Client Socket:%s\n",name_sock);

  memset(&req,0,sizeof(struct Request));
  strncpy(req.sig,signature,sizeof(req.sig));
  strncpy(req.req,lb_req_submit,sizeof(req.req));
  strncpy(req.data.submit.cmd,command,sizeof(req.data.submit)-1);

  ReqSend(sv_sock, &req, &ret);
  if (ret.status == 0) {
    memcpy(response,&(ret.data.submit),sizeof(struct st_job));
    return 0;
  }
  return -1;
//  return (ret.status == 0);
}

int
ReqEcho(const char* chaine, char* ret) {
  struct Request req;
  struct Response res;
  const char* sv_sock= "/tmp/tms-posix-socket-1001";
  int resultat;

  memset(&req,0,sizeof(struct Request));
  strncpy(req.sig,signature,sizeof(req.sig));
  strncpy(req.req,lb_req_echo,sizeof(req.req));
  strncpy(req.data.echo.data,chaine,sizeof(req.data.echo.data)-1);

  resultat = ReqSend(sv_sock, &req, &res);
  if (resultat < 0) {
    return resultat;
  }

  strncpy(ret,res.data.echo.data,sizeof(res.data.echo.data)-1);
  if (res.status != 0) {
    return 0;
  }
  return 0;
}

int
ReqCancel(const char* JobId) {
  struct Request req;
  struct Response res;
  const char* sv_sock= "/tmp/tms-posix-socket-1001";
  int resultat;

  memset(&req,0,sizeof(struct Request));
  strncpy(req.sig,signature,sizeof(req.sig));
  strncpy(req.req,lb_req_cancel,sizeof(req.req));
  strncpy(req.data.cancel.JobId,JobId,sizeof(req.data.cancel.JobId)-1);

  resultat = ReqSend(sv_sock, &req, &res);
  if (resultat < 0) {
    return resultat;
  }
  return 0;
}

int
ReqInfo(const char* JobIdi, struct st_job* response) {
  struct Request req;
  struct Response ret;
  const char* sv_sock= "/tmp/tms-posix-socket-1001";
  int resultat;

  memset(&req,0,sizeof(struct Request));
  strncpy(req.sig,signature,sizeof(req.sig));
  strncpy(req.req,lb_req_ginfo,sizeof(req.req));

  resultat = ReqSend(sv_sock, &req, &ret);

  if (ret.status == 0) {
    memcpy(response,&(ret.data.submit),sizeof(struct st_job));
    return 0;
  }
  return -1;
}

