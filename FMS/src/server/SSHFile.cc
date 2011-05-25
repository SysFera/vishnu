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
#include "FMSVishnuException.hpp"
#include "utilServer.hpp"
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
    upToDate=true;
  }
  else{

  istringstream is(fileStat.first);
  is >> owner >> group >> oct >> perms >> dec >> uid >> gid >> size
    >> atime >> mtime >> ctime >> fileType;

  transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);

  std::cout << "fileType in SSH getInfos: " << fileType << "\n";
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
}
/* Change the file group through ssh. */
int SSHFile::chgrp(const string& group) {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> chgrpResult;

  if (!exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  chgrpResult = ssh.exec(CHGRPCMD+group+" "+getPath());
  if (chgrpResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,"Error changing file group: "+chgrpResult.second);
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
  
  if (!exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  os << mode;
  chmodResult = ssh.exec(CHMODCMD+os.str()+" "+getPath());
  
  if (chmodResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,"Error changing file mode: "+chmodResult.second);
  }
  setPerms(mode);
  return 0;
}

/* Get the file head through ssh. */
string SSHFile::head(const HeadOfFileOptions& options) {
  int nline= options.getNline();
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> headResult;
  
  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  }

  os << nline;
  headResult = ssh.exec(HEADCMD+os.str()+" "+getPath());

  if (headResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error obtaining the head of the file: "+
                        headResult.second);
  }
  return headResult.first;
}

/* Get the file content through ssh. */
string SSHFile::getContent() {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> catResult;
  
  if (!exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  
  catResult = ssh.exec(CATCMD+getPath());

  if (catResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error obtaining the content of the file: "+
                        catResult.second);
  }
  return catResult.first;
}
/* Create a file through ssh. */
int SSHFile::mkfile(const mode_t mode) {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> mkfileResult;
  
  if (exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" already exists");
  
  mkfileResult = ssh.exec(MKFILECMD+getPath());
  if (mkfileResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating "+getPath()+": "+mkfileResult.second);
  }
  exists(true);
  return 0;
}


/* Create a directory through ssh. */
int SSHFile::mkdir(const mode_t mode) {
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> mkdirResult;
  
  if (exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" already exists");
  os << oct << mode;
  mkdirResult = ssh.exec(MKDIRCMD+os.str()+" "+getPath());
  if (mkdirResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating "+getPath()+": "+mkdirResult.second);
  }
  exists(true);
  return 0;
}



/* Remove the file through ssh. */
int SSHFile::rm() {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> rmResult;
  
  if (!exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  rmResult = ssh.exec(RMCMD+getPath());
  if (rmResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error removing "+getPath()+": "+rmResult.second);
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
  
  if (!exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  rmdirResult = ssh.exec(RMDIRCMD+getPath());
  if (rmdirResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error removing "+getPath()+": "+rmdirResult.second);
  }
  exists(false);
  upToDate=false;
  return 0;
}

/* Get the file tail through ssh. */
string SSHFile::tail(const TailOfFileOptions& options) {

  int nline=options.getNline();
  ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> tailResult;
  
  if (!exists()) throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  os << nline;
  tailResult = ssh.exec(TAILCMD+os.str()+" "+getPath());
  
  if (tailResult.second.length()!=0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error obtaining the head of the file: "+
                        tailResult.second);
  }
  return tailResult.first;
}


/* Get the files and subdirectory of this directory through ssh. */
list<string> SSHFile::ls(const LsDirOptions& options) const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> lsResult;
  list<string> result;
  std::cout <<"coucou dans SSH::ls\n"; 
  string lsCmd("ls ");

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
  }


  if (options.isLongFormat()){
    lsCmd.append("-l ");
  }
  if (options.isAllFiles()){
    lsCmd.append("-a ");
  }

  lsResult = ssh.exec(lsCmd+getPath());

  if (lsResult.second.length()!=0)
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error listing directory: "+lsResult.second);

  istringstream is(lsResult.first);
  char buffer[1024];
  string line;
  
  while (!is.eof()) {
    is.getline(buffer, 1024);
    line = buffer;
    result.push_back(line);
  }
  return result;
}


   /* Copy the file through scp. */
 int SSHFile::cp(const string& dest, const CpFileOptions& options){

  //   string host = File::extHost(dest);
   //  string path = File::extName(dest);

     if (!exists()) {
       throw FMSVishnuException(ERRCODE_INVALID_PATH,getPath()+" does not exist");
     }

     SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
         sshPublicKey, sshPrivateKey);
     pair<string,string> cpResult;

     cpResult = ssh.exec(CPCMD+getPath()+" "+dest);
     if (cpResult.second.length()!=0) {
       throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error copying file: "+cpResult.second);
     }

     return 0;
   }

// Defintion of SSHExec Class

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

// texec a remote command 

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
  command << " -C"  << " -o BatchMode=yes " << " -o StrictHostKeyChecking=yes";
  command  << " -o ControlMaster=yes " << " -o ControlPath=/tmp/ssh-%r@%h:%p";
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
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating communication pipe");
  }
  if (pipe(comPipeErr)==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    close(comPipeOut[0]);
    close(comPipeOut[1]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error creating communication pipe");
  }
  pid = fork();
  
  if (pid==-1) {
    for (unsigned int i=0; i<tokens.size(); ++i)
      free(argv[i]);
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error forking process");
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
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error executing command "+command.str());
  }
  
  close(comPipeOut[0]);
  close(comPipeErr[0]);
  for (unsigned int i=0; i<tokens.size(); ++i)
    free(argv[i]);
  lastExecStatus = status;
 cout << "result.second    :" << result.second << endl;
  return result;
}


