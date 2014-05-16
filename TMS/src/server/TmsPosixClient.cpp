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
reqSend(const std::string& destination, const struct Request* req, struct Response* ret) {
  int sfd;
  struct sockaddr_un addr;
  int sv_errno;
  size_t nbCharWrite = sizeof(struct Request);
  ssize_t nbCharWriten = 0;
  ssize_t nbCharReaden;

  sfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sfd == -1) {
    return -1;
  }

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, destination.c_str(), sizeof(addr.sun_path) -1);

  if ( connect(sfd,(struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
    sv_errno = errno;
    close(sfd); // a voir
    errno = sv_errno;
    return -2;
  }

  while (nbCharWrite > 0) {
    nbCharWriten = write(sfd, req, nbCharWrite);
    if (nbCharWriten < 0) {
      if (errno == EINTR)
        continue;
      sv_errno = errno;
      close(sfd);
      errno = sv_errno;
      return -3;
    }
    nbCharWrite -= nbCharWriten;
  }

  memset(ret, 0, sizeof(struct Response));
  while ( (nbCharReaden = read(sfd, ret, sizeof(struct Response))) < 0)
    if (errno != EINTR) {
      sv_errno = errno;
      break;
    }

  close(sfd);

  if (nbCharReaden < 0)
    errno = sv_errno;

  return nbCharReaden;
}

int
reqSubmit(const std::string& command, struct trameJob *response, struct trameSubmit *sub) {
  struct Request req;
  struct Response ret;
  char name_sock[255];
  uid_t euid;
  int status;

  euid = geteuid();
  snprintf(name_sock, sizeof(name_sock), "%s/%s%d","/tmp", SV_SOCK, euid);

  memset(&req, 0, sizeof(struct Request));
  strncpy(req.sig, SIGNATURE, sizeof(req.sig));
  strncpy(req.req, LB_REQ_SUBMIT, sizeof(req.req));
  strncpy(req.data.submit.cmd, command.c_str(), sizeof(req.data.submit)-1);

  strncpy(req.data.submit.outPutPath, sub->outPutPath, sizeof(req.data.submit.outPutPath)-1);
  strncpy(req.data.submit.errorPath, sub->errorPath, sizeof(req.data.submit.errorPath)-1);
  strncpy(req.data.submit.workDir, sub->workDir, sizeof(req.data.submit.workDir)-1);
  strncpy(req.data.submit.jobName, sub->jobName, sizeof(req.data.submit.jobName)-1);
  req.data.submit.walltime = sub->walltime;

  status = reqSend(name_sock, &req, &ret);

  if (status < 0)
    return status;

  *response = ret.data.submit;
  return 0;
}

int
reqEcho(const std::string& chaine, char* ret) {
  struct Request req;
  struct Response res;
  int resultat;
  char name_sock[255];
  uid_t euid;

  euid = geteuid();
  snprintf(name_sock, sizeof(name_sock), "%s/%s%d","/tmp", SV_SOCK, euid);

  memset(&req, 0, sizeof(struct Request));
  strncpy(req.sig, SIGNATURE, sizeof(req.sig));
  strncpy(req.req, LB_REQ_ECHO, sizeof(req.req));
  strncpy(req.data.echo.data, chaine.c_str(), sizeof(req.data.echo.data)-1);

  resultat = reqSend(name_sock, &req, &res);
  if (resultat < 0) {
    return resultat;
  }

  strncpy(ret, res.data.echo.data, sizeof(res.data.echo.data)-1);
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
reqCancel(const std::string& jobId) {
  struct Request req;
  struct Response res;
  int resultat;
  char name_sock[255];
  char euid[255];

  strccpy(euid, jobId.c_str(), sizeof(euid), '-');

  snprintf(name_sock, sizeof(name_sock), "%s/%s%s", "/tmp", SV_SOCK, euid);

  memset(&req, 0, sizeof(struct Request));
  strncpy(req.sig, SIGNATURE, sizeof(req.sig));
  strncpy(req.req, LB_REQ_CANCEL, sizeof(req.req));
  strncpy(req.data.cancel.jobId, jobId.c_str(), sizeof(req.data.cancel.jobId)-1);

  resultat = reqSend(name_sock, &req, &res);
  if (resultat < 0) {
    return resultat;
  }
  return 0;
}

int
reqInfo(const std::string& jobId, struct trameJob *response) {
  struct Request req;
  struct Response ret;
  int resultat;
  char name_sock[255];
  char euid[255];

  strccpy(euid, jobId.c_str(), sizeof(euid), '-');

  snprintf(name_sock, sizeof(name_sock), "%s/%s%s", "/tmp", SV_SOCK, euid);

  memset(&req, 0, sizeof(struct Request));
  strncpy(req.sig, SIGNATURE, sizeof(req.sig));
  strncpy(req.req, LB_REQ_GINFO, sizeof(req.req));
  strncpy(req.data.info.jobId, jobId.c_str(), sizeof(req.data.info.jobId)-1);

  resultat = reqSend(name_sock, &req, &ret);

  if (ret.status == 0 && resultat >= 0) {
    *response = ret.data.info;
    return 0;
  }
  return -1;
}
