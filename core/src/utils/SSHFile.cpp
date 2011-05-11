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
SSHFile::SSHFile(const string& path,
                 const string& sshHost,
                 const string& sshUser,
                 const string& sshPublicKey,
                 const string& sshPrivateKey,
                 const string& sshPassword,
                 unsigned int sshPort,
                 const string& sshCommand,
                 const string& scpCommand) : File(path) {
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

#ifndef LIBSSH2
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
  
  fileStat = ssh.exec(STATCMD+getPath());
  
  if (fileStat.second.length()!=0) {
    exists(false);
    return;
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

/* Copy the file through scp. */
File* SSHFile::cp(const string& dest) {
  string host = File::extHost(dest);
  string path = File::extName(dest);
  SSHFile* localResult;
  RemoteFile* remoteResult;
  char* transferID;

  transferID = strdup(gen_uuid().c_str());

  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (host=="localhost") {
    SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
                sshPublicKey, sshPrivateKey);
    pair<string,string> cpResult;
    
    cpResult = ssh.exec(CPCMD+getPath()+" "+path);
    if (cpResult.second.length()!=0) {
      throw runtime_error("Error copying file: "+cpResult.second);
    }
    localResult = new SSHFile(path, sshHost, sshUser, sshPublicKey,
                              sshPrivateKey, sshPassword, sshPort,
                              sshCommand, scpCommand);
    localResult->chgrp(getGroup());
    localResult->chmod(getPerms());
    return localResult;
  }
  
  diet_profile_t* profile;
  char* errMsg, *dataID;
  mode_t mode = getPerms();
  file_type_t type = getType();
  
  sysEndianChg<mode_t>(mode);
  sysEndianChg<file_type_t>(type);
  
  dagda_put_file(const_cast<char*>(getPath().c_str()), DIET_PERSISTENT, &dataID);
  profile = diet_profile_alloc(CP_GETFILE_SRV(getHost()), 6, 6, 7);
  diet_string_set(diet_parameter(profile, 0), const_cast<char*>(getPath().c_str()),
                  DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 1), const_cast<char*>(getOwner().c_str()),
                  DIET_VOLATILE);
  diet_paramstring_set(diet_parameter(profile, 2), const_cast<char*>(getHost().c_str()),
                       DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 3), const_cast<char*>(getGroup().c_str()), DIET_VOLATILE);
  diet_scalar_set(diet_parameter(profile, 4), &mode, DIET_VOLATILE, DIET_LONGINT);
  diet_scalar_set(diet_parameter(profile, 5), &type, DIET_VOLATILE, DIET_INT);
  diet_string_set(diet_parameter(profile, 6), dataID, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 7), transferID, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile, 8), NULL, DIET_VOLATILE);
  
  if (diet_call(profile))
    throw runtime_error("Error calling DIET service");
  diet_string_get(diet_parameter(profile, 8), &errMsg, NULL);
  
  if (strlen(errMsg)!=0) {
    string err = errMsg;
    throw runtime_error(err);
  }
  
  remoteResult = new RemoteFile(dest, getOwner());
  return remoteResult;
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

/* Move a file through ssh and scp. */
File* SSHFile::mv(const string& dest) {
  string host = File::extHost(dest);
  string path = File::extName(dest);
  RemoteFile* remoteResult;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (host=="localhost") {
    SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
                sshPublicKey, sshPrivateKey);
    pair<string,string> mvResult;
    
    mvResult = ssh.exec(MVCMD+getPath()+" "+path);
    if (mvResult.second.length()!=0) {
      throw runtime_error("Error moving file: "+mvResult.second);
    }

    setPath(path);
    upToDate = false;
    return this;
  }
  
  cout << "Move "+getPath()+" to "+host+" ("+path+")" << endl;
  cp(dest);
  rm();
  remoteResult = new RemoteFile(dest, getOwner());
  return remoteResult;
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

/* Get the files and subdirectory of this directory through ssh. */
list<string> SSHFile::lsDir() const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> lsResult;
  list<string> result;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (getType()!=directory)
    throw runtime_error(getPath()+" is not a directory");
  
  lsResult = ssh.exec(LSCMD+getPath());
  
  if (lsResult.second.length()!=0)
    throw runtime_error("Error listing directory: "+lsResult.second);
  
  istringstream is(lsResult.first);
  char buffer[1024];
  string line;
  
  while (!is.eof()) {
    is.getline(buffer, 1024);
    line = buffer;
    result.push_back(line);
  }
  result.pop_front();
  return result;
}

/* Get the files and subdirectory of this directory through ssh. */
list<string> SSHFile::lsDirSimple() const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> lsResult;
  list<string> result;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (getType()!=directory)
    throw runtime_error(getPath()+" is not a directory");
  
  lsResult = ssh.exec(LSSIMPLE+getPath());
  
  if (lsResult.second.length()!=0)
    throw runtime_error("Error listing directory: "+lsResult.second);
  
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


list<string> SSHFile::lsDirRec() const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  pair<string,string> lsResult;
  list<string> result;
  
  if (!exists()) throw runtime_error(getPath()+" does not exist");
  if (getType()!=directory)
    throw runtime_error(getPath()+" is not a directory");
  
  lsResult = ssh.exec(LSRECCMD+getPath());
  if (lsResult.second.length()!=0)
    throw runtime_error("Error listing directory: "+lsResult.second);
  
  istringstream is(lsResult.first);
  char buffer[1024];
  string line;
  
  while (!is.eof()) {
    is.getline(buffer, 1024);
    line = buffer;
    result.push_back(line);
  }
  result.pop_front();

  return result;
}

void SSHFile::getInfos() const {
  struct stat fileStat;
  SSHSession session(sshHost, sshPort, sshUser, sshPassword,
                     sshPublicKey, sshPrivateKey);
  LIBSSH2_SESSION* sshSession;
  LIBSSH2_CHANNEL* channel;
  
  sshSession = session.getSession();
  channel = libssh2_scp_recv(sshSession, getPath().c_str(),
                             &fileStat);
  if (!channel)
    throw runtime_error("Failed to open ssh channel");
  setPerms(fileStat.st_mode && ACCESSPERMS);
  setUid(fileStat.st_uid);
  setGid(fileStat.st_gid);
  setSize(fileStat.st_size);
  setAtime(fileStat.st_atime);
  setMtime(fileStat.st_mtime);
  setCtime(fileStat.st_ctime);
  if (S_ISBLK(fileStat.st_mode))
    setType(block);
  if (S_ISCHR(fileStat.st_mode))
    setType(character);
  if (S_ISDIR(fileStat.st_mode))
    setType(directory);
  if (S_ISLNK(fileStat.st_mode))
    setType(symboliclink);
  if (S_ISSOCK(fileStat.st_mode))
    setType(sckt);
  if (S_ISFIFO(fileStat.st_mode))
    setType(fifo);
  if (S_ISREG(fileStat.st_mode))
    setType(regular);
  
  setOwner(sshUser);
  //  setGroup(grp->gr_name);
  
  exists(true);
  upToDate=true;
}

