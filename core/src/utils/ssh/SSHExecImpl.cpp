#include "SSHExecImpl.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <sys/wait.h>
#include "SystemException.hpp"

#define CHECKSYSCALL(cmd) {if (cmd!=0) { throw SystemException (ERRCODE_SYSTEM, "system call failed"); }}

SSHExecImpl::SSHExecImpl(SSHExec* e): me(e) {
}

SSHExecImpl::~SSHExecImpl() {
}


SSHResult*
SSHExecImpl::execSSH(string cmd) {

  vector<string> tokens;
  ostringstream command;
  pid_t pid;
  int comPipeOut[2];
  int comPipeErr[2];
  int status;
  char c;
  SSHResult* result = new SSHResult();
  string out = "", err = "";

  string sshCommand = me->getSSHcmd();
  string privateKey = me->getPrivateKey();
  string userName = me->getUsername();
  int sshPort = me->getSSHPort();
  string server = me->getServer();
  int tmp;
  
  
  command << sshCommand << " -i " << privateKey << " -l " << userName;
  command << " -p " << sshPort << " " << server << " " << cmd;
  
  istringstream is(command.str());
  
  copy(istream_iterator<string>(is),
       istream_iterator<string>(),
       back_inserter<vector<string> >(tokens));
  
  char* argv[tokens.size()+1];
  argv[tokens.size()]=NULL;
  
  for (unsigned int i=0; i<tokens.size(); ++i)
    argv[i]=strdup(tokens[i].c_str());
  
  if (pipe(comPipeOut)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error creating communication pipe");
  }
  if (pipe(comPipeErr)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    close(comPipeOut[0]);
    close(comPipeOut[1]);
    throw runtime_error("Error creating communication pipe");
  }
  pid = fork();
  
  if (pid==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error forking process");
  }
  if (pid==0) {
    CHECKSYSCALL(close(comPipeOut[0]));
    CHECKSYSCALL(close(comPipeErr[0]));
    CHECKSYSCALL(dup2(comPipeOut[1], 1));
    CHECKSYSCALL(dup2(comPipeErr[1], 2));
    CHECKSYSCALL(close(comPipeOut[1]));
    CHECKSYSCALL(close(comPipeErr[1]));
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }
  CHECKSYSCALL(close(comPipeOut[1]));
  CHECKSYSCALL(close(comPipeErr[1]));
 
  while ((tmp=read(comPipeOut[0], &c, 1))) {
    CHECKSYSCALL(tmp);
    out+=c;
  }

  while ((tmp=read(comPipeErr[0], &c, 1))) {
    CHECKSYSCALL(tmp);
    err+=c;
  }

  result->setOutput(out);
  result->setError(err);

  if (waitpid(pid, &status, 0)==-1) {
    CHECKSYSCALL(close(comPipeOut[0]));
    CHECKSYSCALL(close(comPipeErr[0]));
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error executing command "+command.str());
  }
  
  CHECKSYSCALL(close(comPipeOut[0]));
  CHECKSYSCALL(close(comPipeErr[0]));
  for (unsigned int i=0; i<tokens.size(); ++i)
    free(argv[i]);
  result->setStatus(status);
  return result;

}
