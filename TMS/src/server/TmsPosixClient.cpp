/**
 * \file TmsPosicClient.cpp
 * \brief This file contains the implementation of the data and the clients for the posix parser.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

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
  ssize_t nbCharreaden;


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

  memset(ret, 0, sizeof(struct Response));
  nbCharreaden = read(sfd,ret,sizeof(struct Response));

  printf("Taille req res:%ld.\n",nbCharreaden);

  close(sfd);

  //return 0;
  return nbCharreaden;
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

  strncpy(req.data.submit.OutPutPath,sub->OutPutPath,sizeof(req.data.submit.OutPutPath)-1);
  strncpy(req.data.submit.ErrorPath,sub->ErrorPath,sizeof(req.data.submit.ErrorPath)-1);
  strncpy(req.data.submit.WorkDir,sub->WorkDir,sizeof(req.data.submit.WorkDir)-1);

  ReqSend(name_sock, &req, &ret);

  printf("RetStatu:%d.\n",ret.status);
  printf("RetJobID:%s.\n",ret.data.submit.JobId);
  printf("Retpid:%d.\n",ret.data.submit.pid);
  

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
  const char* sv_sock= "tms-posix-socket-";
  char name_sock[255];
  uid_t euid;
  struct passwd* lpasswd;
  int resultat;

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
  strncpy(req.req,lb_req_echo,sizeof(req.req));
  strncpy(req.data.echo.data,chaine,sizeof(req.data.echo.data)-1);

  resultat = ReqSend(name_sock, &req, &res);
  if (resultat < 0) {
    return resultat;
  }

  strncpy(ret,res.data.echo.data,sizeof(res.data.echo.data)-1);
  if (res.status != 0) {
    return 0;
  }
  return 0;
}

static char *strccpy(char* dest, const char* src, int lg, char car) {
  int i;

  for (i = 0; (*src != car) && (i < lg); i++) {
    *dest++ = *src++;
  }
  *dest = 0;

  return dest;
}


int
ReqCancel(const char* JobId) {
  struct Request req;
  struct Response res;
  const char* sv_sock= "tms-posix-socket-";
  int resultat;
  char name_sock[255];
  char euid[255];

  strccpy(euid,JobId,sizeof(euid),'-');

  snprintf(name_sock,sizeof(name_sock),"%s/%s%s","/tmp",sv_sock,euid);

  memset(&req,0,sizeof(struct Request));
  strncpy(req.sig,signature,sizeof(req.sig));
  strncpy(req.req,lb_req_cancel,sizeof(req.req));
  strncpy(req.data.cancel.JobId,JobId,sizeof(req.data.cancel.JobId)-1);

  resultat = ReqSend(name_sock, &req, &res);
  if (resultat < 0) {
    return resultat;
  }
  return 0;
}

int
ReqInfo(const char* JobId, struct st_job* response) {
  struct Request req;
  struct Response ret;
  const char* sv_sock= "tms-posix-socket-";
  int resultat;
  char name_sock[255];
  char euid[255];

  strccpy(euid,JobId,sizeof(euid),'-');

  snprintf(name_sock,sizeof(name_sock),"%s/%s%s","/tmp",sv_sock,euid);

  memset(&req,0,sizeof(struct Request));
  strncpy(req.sig,signature,sizeof(req.sig));
  strncpy(req.req,lb_req_ginfo,sizeof(req.req));

  resultat = ReqSend(name_sock, &req, &ret);

  if (ret.status == 0) {
    memcpy(response,&(ret.data.submit),sizeof(struct st_job));
    return 0;
  }
  return -1;
}

