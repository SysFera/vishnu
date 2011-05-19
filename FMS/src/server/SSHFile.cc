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

#include "SSHFile.hh"
#include "RemoteFile.hh"
#include "File.hh"

using namespace std;

/* Default constructor. */
SSHFile::SSHFile() : File() {
  setHost("localhost");
  upToDate = false;
  sshHost = "localhost";
  sshUser = "default";
  sshPublicKey = "none";
  sshPrivateKey = "none";
}

/* Standard constructor.
 * Take as parameter:
 *   - The file path
 *   - The host of the ssh server
 *   - The user for the ssh connection
 *   - The user ssh public key path (optional)
 *   - The user ssh private key path (mandatory)
 *   - The user ssh password (optionnal)
 *   - The ssh server port
 *   - The path to the ssh executable
 *   - The path to the scp executable
 */
SSHFile::SSHFile(const SessionServer& sessionServer,
                 const string& path,
                 const string& sshHost,
                 const string& sshUser,
                 const string& sshPublicKey,
                 const string& sshPrivateKey,
                 const string& sshPassword,
                 unsigned int sshPort,
                 const string& sshCommand,
                 const string& scpCommand) : File(sessionServer,path) {
  setHost("localhost");
  upToDate = false;
  this->sshHost = sshHost;
  this->sshUser = sshUser;
  this->sshPublicKey = sshPublicKey;
  this->sshPrivateKey = sshPrivateKey;
  this->sshPassword = sshPassword;
  this->sshPort = sshPort;
  this->sshCommand = sshCommand;
  this->scpCommand = scpCommand;
}

/* Standard destructor. */
SSHFile::~SSHFile() {}

/* Copy constructor. */
SSHFile::SSHFile(const SSHFile& file) : File(file) {
  operator=(file);
}

/* Copy operator. */
SSHFile& SSHFile::operator=(const SSHFile& file) {
  upToDate = file.isUpToDate();
  this->sshHost = file.sshHost;
  this->sshUser = file.sshUser;
  this->sshPublicKey = file.sshPublicKey;
  this->sshPrivateKey = file.sshPrivateKey;
  this->sshPassword = file.sshPassword;
  this->sshPort = file.sshPort;
  this->sshCommand = file.sshCommand;
  this->scpCommand = file.scpCommand;
  return *this;
}

/* Return true if the files informations are up to date. */
bool SSHFile::isUpToDate() const {
  return upToDate;
}

/* Get the file information through ssh. */
void SSHFile::getInfos() const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> fileStat;
  string owner, group, fileType;
  mode_t perms;
  uid_t uid;
  gid_t gid;
  file_size_t size;
  time_t atime, mtime, ctime;
 cout << "Coucou dans getInfos de SSHFile  " << endl; 
  fileStat = ssh.exec(STATCMD+getPath());
  
  if (fileStat.second.length()!=0) {
    exists(false);
 cout << "Mais exist n'est pas mis a jour " << endl;
    throw runtime_error (fileStat.second.c_str());
  }
  
  istringstream is(fileStat.first);
  is >> owner >> group >> oct >> perms >> dec >> uid >> gid >> size
     >> atime >> mtime >> ctime >> fileType;
  
  transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);
  
  setOwner(owner);
  setGroup(group);
  setPerms(perms);
  setUid(uid);
  setGid(gid);
  setSize(size);
  setAtime(atime);
  setMtime(mtime);
  setCtime(ctime);
  if (fileType=="block")
    setType(block);
  if (fileType=="character")
    setType(character);
  if (fileType=="directory")
    setType(directory);
  if (fileType=="symbolic")
    setType(symboliclink);
  if (fileType=="socket")
    setType(sckt);
  if (fileType=="fifo")
    setType(fifo);
  if (fileType=="regular")
    setType(regular);
  
  exists(true);
  upToDate=true;
}

/* Change the file group through ssh. */
int SSHFile::chgrp(const string& group) {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> chgrpResult;

  if (!exists()) throw runtime_error(getPath()+" does not exist");
  chgrpResult = ssh.exec(CHGRPCMD+group+" "+getPath());
  if (chgrpResult.second.length()!=0) {
    throw runtime_error("Error changing file group: "+chgrpResult.second);
  }
  setGroup(group);
  return 0;
}

/* Change the file mode through ssh. */
int SSHFile::chmod(const mode_t mode) {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> chmodResult;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  os << oct << mode;
  chmodResult = ssh.exec(CHMODCMD+os.str()+" "+getPath());
  
  if (chmodResult.second.length()!=0) {
    throw runtime_error("Error changing file mode: "+chmodResult.second);
  }
  setPerms(mode);
  return 0;
}

/* Get the file head through ssh. */
string SSHFile::head(const unsigned int nline) {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> headResult;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  os << nline;
  headResult = ssh.exec(HEADCMD+os.str()+" "+getPath());

  if (headResult.second.length()!=0) {
    throw runtime_error("Error obtaining the head of the file: "+
                        headResult.second);
  }
  return headResult.first;
}

/* Create a directory through ssh. */
int SSHFile::mkdir(const mode_t mode) {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> mkdirResult;
  
  if (exists()) throw runtime_error(getPath()+" already exists");
  os << oct << mode;
  mkdirResult = ssh.exec(MKDIRCMD+os.str()+" "+getPath());
  if (mkdirResult.second.length()!=0) {
    throw runtime_error("Error creating "+getPath()+": "+mkdirResult.second);
  }
  exists(true);
  return 0;
}



/* Remove the file through ssh. */
int SSHFile::rm() {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> rmResult;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  rmResult = ssh.exec(RMCMD+getPath());
  if (rmResult.second.length()!=0) {
    throw runtime_error("Error removing "+getPath()+": "+rmResult.second);
  }
  exists(false);
  upToDate=false;
  return 0;
}

/* Remove this directory through ssh. */
int SSHFile::rmdir() {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> rmdirResult;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  rmdirResult = ssh.exec(RMDIRCMD+getPath());
  if (rmdirResult.second.length()!=0) {
    throw runtime_error("Error removing "+getPath()+": "+rmdirResult.second);
  }
  exists(false);
  upToDate=false;
  return 0;
}

/* Get the file tail through ssh. */
string SSHFile::tail(const unsigned int nline) {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> tailResult;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  os << nline;
  tailResult = ssh.exec(TAILCMD+os.str()+" "+getPath());
  
  if (tailResult.second.length()!=0) {
    throw runtime_error("Error obtaining the head of the file: "+
                        tailResult.second);
  }
  return tailResult.first;
}




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
  
 /* command << sshCommand << " -i " << privateKey << " -l " << userName;
  command << " -p " << sshPort << " " << server << " " << cmd;*/

  command << sshCommand  << " -l " << userName;
  command << " -p " << sshPort << " " << server << " " << cmd;

  
  istringstream is(command.str());
  
  copy(istream_iterator<string>(is),
       istream_iterator<string>(),
       back_inserter<vector<string> >(tokens));
  
/***********************************************/

vector<string>:: iterator ite;
cout << endl;
for (ite=tokens.begin(); ite!=tokens.end();++ite)
cout << *ite << endl;



/**********************************************/


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
    close(comPipeOut[0]); /* Close unused read end */
    close(comPipeErr[0]); /* Close unused write end */
    dup2(comPipeOut[1], 1);
    dup2(comPipeErr[1], 2);
    close(comPipeOut[1]);
    close(comPipeErr[1]);
    if (execvp(argv[0], argv)) {
      exit(-1);
    }
  }
  close(comPipeOut[1]); /* Close unused write end */

  close(comPipeErr[1]);/* Close unused write end */

 
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
 cout << "result.second    :" << result.second << endl;
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


