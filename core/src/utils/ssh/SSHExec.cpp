#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>

#include <unistd.h>
#include <sys/wait.h>

#include "DIET_data.h"
#include "DIET_client.h"
#include "DIET_Dagda.h"


#include "SSHExec.hh"

using namespace std;

SSHExec::SSHExec(const string& sshCommand, const string& scpCommand,
                 const string& server, unsigned int sshPort,
                 const string& userName,
                 const string& password, const string& publicKey,
                 const string& privateKey) :
sshCommand(sshCommand), scpCommand(scpCommand), server(server),
sshPort(sshPort), userName(userName), password(password), publicKey(publicKey),
privateKey(privateKey)
{
  lastExecStatus=0;
}

const int& SSHExec::getLastExecStatus() const {
  return lastExecStatus;
}

pair<string, string> SSHExec::exec(const string& cmd) const {
  vector<string> tokens;
  ostringstream command;
  pid_t pid;
  pair<string,string> result;
  int comPipeOut[2];
  int comPipeErr[2];
  int status;
  char c;
  
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
    close(comPipeOut[0]);
    close(comPipeErr[0]);
    dup2(comPipeOut[1], 1);
    dup2(comPipeErr[1], 2);
    close(comPipeOut[1]);
    close(comPipeErr[1]);
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }
  close(comPipeOut[1]);
  close(comPipeErr[1]);
 
  while (read(comPipeOut[0], &c, 1))
    result.first+=c;

  while (read(comPipeErr[0], &c, 1))
    result.second+=c;

  if (waitpid(pid, &status, 0)==-1) {
    close(comPipeOut[0]);
    close(comPipeErr[0]);
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error executing command "+command.str());
  }
  
  close(comPipeOut[0]);
  close(comPipeErr[0]);
  for (unsigned int i=0; i<tokens.size(); ++i)
    free(argv[i]);
  lastExecStatus = status;
  return result;
}

void SSHExec::copyFrom(const string& file, const string& src) const {
  vector<string> tokens;
  ostringstream command;
  pid_t pid;
  int status;
  
  command << scpCommand << " -i " << privateKey << " -o User=" << userName;
  command << " -P " << sshPort << " -p " << server << ":" << src;
  command << " " << file;
  
  istringstream is(command.str());
  
  copy(istream_iterator<string>(is),
       istream_iterator<string>(),
       back_inserter<vector<string> >(tokens));
  
  char* argv[tokens.size()+1];
  argv[tokens.size()]=NULL;
  
  for (unsigned int i=0; i<tokens.size(); ++i)
    argv[i]=strdup(tokens[i].c_str());
  
  pid = fork();
  
  if (pid==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error forking process");
  }
  if (pid==0) {  
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }
  if (waitpid(pid, &status, 0)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error executing command "+command.str());
  }
  
  for (unsigned int i=0; i<tokens.size(); ++i)
    free(argv[i]);
  lastExecStatus = status;
}

void SSHExec::copyTo(const string& file, const string& dest) const {
  vector<string> tokens;
  ostringstream command;
  pid_t pid;
  int status;
  
  command << scpCommand << " -i " << privateKey << " -o User=" << userName;
  command << " -P " << sshPort << " -p " << file << " " << server;
  command << ":" << dest;
  
  istringstream is(command.str());
  
  copy(istream_iterator<string>(is),
       istream_iterator<string>(),
       back_inserter<vector<string> >(tokens));
  
  char* argv[tokens.size()+1];
  argv[tokens.size()]=NULL;
  
  for (unsigned int i=0; i<tokens.size(); ++i)
    argv[i]=strdup(tokens[i].c_str());
  
  pid = fork();
  
  if (pid==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error forking process");
  }
  if (pid==0) {  
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }
  if (waitpid(pid, &status, 0)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw runtime_error("Error executing command "+command.str());
  }
  
  for (unsigned int i=0; i<tokens.size(); ++i)
    free(argv[i]);
  lastExecStatus = status;
}

