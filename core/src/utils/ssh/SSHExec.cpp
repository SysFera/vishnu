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


#include "SSHExec.hpp"

using namespace std;

SSHExec::SSHExec(const string& sshCommand, const string& scpCommand,
                 const string& server, unsigned int sshPort,
                 const string& userName,
                 const string& password, const string& publicKey,
                 const string& privateKey,
		 boost::function1<SSHResult&, string>& func) :
msshCommand(sshCommand), mscpCommand(scpCommand), mserver(server),
msshPort(sshPort), muserName(userName), mpassword(password), mpublicKey(publicKey),
mprivateKey(privateKey), mfunc(func)
{

}


SSHResult&
SSHExec::exec(const string& cmd) const {

  boost::function1<SSHResult&,string> call(boost::bind(&SSHExecImpl::execSSH, boost::ref(mexec),_1));

//  vector<string> tokens;
//  ostringstream command;
//  pid_t pid;
//  pair<string,string> result;
//  int comPipeOut[2];
//  int comPipeErr[2];
//  int status;
//  char c;
//  
//  command << sshCommand << " -i " << privateKey << " -l " << userName;
//  command << " -p " << sshPort << " " << server << " " << cmd;
//  
//  istringstream is(command.str());
//  
//  copy(istream_iterator<string>(is),
//       istream_iterator<string>(),
//       back_inserter<vector<string> >(tokens));
//  
//  char* argv[tokens.size()+1];
//  argv[tokens.size()]=NULL;
//  
//  for (unsigned int i=0; i<tokens.size(); ++i)
//    argv[i]=strdup(tokens[i].c_str());
//  
//  if (pipe(comPipeOut)==-1) {
//    for (unsigned int i=0; i<tokens.size(); ++i)
//      free(argv[i]);
//    throw runtime_error("Error creating communication pipe");
//  }
//  if (pipe(comPipeErr)==-1) {
//    for (unsigned int i=0; i<tokens.size(); ++i)
//      free(argv[i]);
//    close(comPipeOut[0]);
//    close(comPipeOut[1]);
//    throw runtime_error("Error creating communication pipe");
//  }
//  pid = fork();
//  
//  if (pid==-1) {
//    for (unsigned int i=0; i<tokens.size(); ++i)
//      free(argv[i]);
//    throw runtime_error("Error forking process");
//  }
//  if (pid==0) {
//    close(comPipeOut[0]);
//    close(comPipeErr[0]);
//    dup2(comPipeOut[1], 1);
//    dup2(comPipeErr[1], 2);
//    close(comPipeOut[1]);
//    close(comPipeErr[1]);
//    if (execvp(argv[0], argv)) {
//      exit(-1);
//    }
//  }
//  close(comPipeOut[1]);
//  close(comPipeErr[1]);
// 
//  while (read(comPipeOut[0], &c, 1))
//    result.first+=c;
//
//  while (read(comPipeErr[0], &c, 1))
//    result.second+=c;
//
//  if (waitpid(pid, &status, 0)==-1) {
//    close(comPipeOut[0]);
//    close(comPipeErr[0]);
//    for (unsigned int i=0; i<tokens.size(); ++i)
//      free(argv[i]);
//    throw runtime_error("Error executing command "+command.str());
//  }
//  
//  close(comPipeOut[0]);
//  close(comPipeErr[0]);
//  for (unsigned int i=0; i<tokens.size(); ++i)
//    free(argv[i]);
//  lastExecStatus = status;
//  return result;
}
