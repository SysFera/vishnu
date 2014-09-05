#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <boost/regex.hpp>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "DIET_client.h"

#include "SSHFile.hpp"
#include "File.hpp"
#include "FMSVishnuException.hpp"
#include "utilServer.hpp"
#include "FileTransferCommand.hpp"
#include "FileTypes.hpp"
#include <boost/date_time/time_zone_base.hpp>
#include <boost/scoped_ptr.hpp>

/* Default constructor. */
SSHFile::SSHFile() : File() {
  setHost("localhost");
  upToDate = false;
  sshHost = "localhost";
  sshUser = "default";
  sshPublicKey = "none";
  sshPrivateKey = "none";
  sshPort = 22;
  merror="";
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
                 const std::string& path,
                 const std::string& sshHost,
                 const std::string& sshUser,
                 const std::string& sshPublicKey,
                 const std::string& sshPrivateKey,
                 const std::string& sshPassword,
                 unsigned int sshPort,
                 const std::string& sshCommand,
                 const std::string& scpCommand) : File(sessionServer,path) {
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

/* Copy operator.  use copy and swap method instead*/
SSHFile&
SSHFile::operator=(const SSHFile& file) {
  upToDate = file.isUpToDate();
  this->sshHost = file.sshHost;
  this->sshUser = file.sshUser;
  this->sshPublicKey = file.sshPublicKey;
  this->sshPrivateKey = file.sshPrivateKey;
  this->sshPassword = file.sshPassword;
  this->sshPort = file.sshPort;
  this->sshCommand = file.sshCommand;
  this->scpCommand = file.scpCommand;
  this->merror = file.merror;

  return *this;
}

/* Return true if the files informations are up to date. */
bool
SSHFile::isUpToDate() const {
  return upToDate;
}

/* Get the file information through ssh. */
void
SSHFile::getInfos() const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> fileStat;
  std::string owner, group, fileType;
  mode_t perms;
  uid_t uid;
  gid_t gid;
  file_size_t size;
  time_t atime, mtime, ctime;

  fileStat = ssh.exec(STATCMD_DEFAULT + getPath() + "; echo -n @ $?");
  if (fileStat.second.find("Warning")!= std::string::npos) {
    fileStat = ssh.exec(STATCMD_DEFAULT + getPath() + ";echo -n @ $?");
  } else if (fileStat.second.find("illegal option")!= std::string::npos) {
    fileStat = ssh.exec(STATCMD_BSD + getPath());
    if (fileStat.second.find("Warning")!= std::string::npos) {
      fileStat = ssh.exec(STATCMD_BSD + getPath() + ";echo -n @ $?");
    }
  }

  std::string output = fileStat.first;
  int status_code = boost::lexical_cast<int>(output.substr(output.find_last_of('@')+2));

  if (status_code != 0) {

    exists(false);
    upToDate=true;

    merror= vishnu::parseErrorMessage( fileStat.second ) ;
  } else {
    std::istringstream is(fileStat.first);
    is >> owner >> group >> std::oct >> perms >> std::dec >> uid >> gid >> size
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


    if (fileType=="block") {
      setType(block);
    }
    if (fileType=="character") {
      setType(character);
    }

    if (fileType=="directory") {
      setType(directory);
    }

    if (fileType=="symbolic") {
      setType(symboliclink);
    }

    if (fileType=="socket") {
      setType(sckt);
    }

    if (fileType=="fifo") {
      setType(fifo);
    }

    if (fileType=="regular") {
      setType(regular);
    }

    exists(true);
    upToDate=true;
  }
}
/* Change the file group through ssh. */
int
SSHFile::chgrp(const std::string& group) {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string,std::string> chgrpResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }
  chgrpResult = ssh.exec(CHGRPCMD + group + " " + getPath());
  if (chgrpResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,
                             "Error changing file group: " + chgrpResult.second);
  }
  if (chgrpResult.first.find("chgrp") != std::string::npos) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error changing file group: " + chgrpResult.first);
  }
  setGroup(group);

  return 0;
}

/* Change the file mode through ssh. */
int
SSHFile::chmod(const mode_t mode) {
  std::ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> chmodResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }
  os << mode;
  chmodResult = ssh.exec(CHMODCMD+os.str()+" "+getPath());

  if (chmodResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,
                             "Error changing file mode: " + chmodResult.second);
  }
  if (chmodResult.first.find("help") != std::string::npos) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error changing file mode: " + chmodResult.first);
  }

  setPerms(mode);

  return 0;
}

/* Get the file head through ssh. */
std::string
SSHFile::head(const FMS_Data::HeadOfFileOptions& options) {
  int nline = options.getNline();
  std::ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> headResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }

  os << nline;
  headResult = ssh.exec(HEADCMD + os.str() + " " + getPath());

  if (headResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error obtaining the head of the file: "+
                             headResult.second);
  }
  if (headResult.first.find("head") != std::string::npos) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error heading file: " + headResult.first);
  }

  return headResult.first;
}

/* Get the file content through ssh. */
std::string
SSHFile::getContent() {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string,std::string> catResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }

  catResult = ssh.exec(CATCMD + getPath());

  if (catResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error obtaining the content of the file: "+
                             catResult.second);
  }

  return catResult.first;
}
/* Create a file through ssh. */
int
SSHFile::mkfile(const mode_t mode) {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string,std::string> mkfileResult;

  if (exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,
                             getPath() + " already exists");
  }

  mkfileResult = ssh.exec(MKFILECMD + getPath());
  if (mkfileResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error creating " + getPath() + ": " + mkfileResult.second);
  }
  exists(true);

  return 0;
}


/* Create a directory through ssh. */
int
SSHFile::mkdir(const FMS_Data::CreateDirOptions& options) {
  std::ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> mkdirResult;

  if (exists() && !options.isIsRecursive()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH,
                             getPath() + " already exists");
  }
  //os << oct << mode;
  if (options.isIsRecursive()) {
    os << " -p " ;
  }

  mkdirResult = ssh.exec(MKDIRCMD + os.str() + " " + getPath());
  if (mkdirResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error creating " + getPath() + ": " + mkdirResult.second);
  }
  exists(true);

  return 0;
}



/* Remove the file through ssh. */
int
SSHFile::rm(const FMS_Data::RmFileOptions& options) {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> rmResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }
  if (options.isIsRecursive()) {
    rmResult = ssh.exec(RMRCMD + getPath());
  } else {
    rmResult = ssh.exec(RMCMD + getPath());
  }


  if (rmResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error removing " + getPath() + ": " + rmResult.second);
  }
  if (rmResult.first.find("rm") != std::string::npos) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error removing file: " + rmResult.first);
  }

  exists(false);
  upToDate=false;

  return 0;
}

/* Remove this directory through ssh. */
int
SSHFile::rmdir() {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> rmdirResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }

  rmdirResult = ssh.exec(RMDIRCMD + getPath());

  if (rmdirResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error removing " + getPath() + ": " + rmdirResult.second);
  }
  if (rmdirResult.first.find("rmdir") != std::string::npos) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error removing directory: " + rmdirResult.first);
  }
  exists(false);
  upToDate = false;

  return 0;
}

/* Get the file tail through ssh. */
std::string
SSHFile::tail(const FMS_Data::TailOfFileOptions& options) {
  int nline=options.getNline();
  std::ostringstream os;
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);
  std::pair<std::string, std::string> tailResult;

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }
  os << nline;
  tailResult = ssh.exec(TAILCMD + os.str() + " " + getPath());

  if (tailResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error obtaining the head of the file: " + tailResult.second);
  }
  if (tailResult.first.find("tail") != std::string::npos) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error tailing file: " + tailResult.first);
  }

  return tailResult.first;
}


/* Get the files and subdirectory of this directory through ssh. */
FMS_Data::DirEntryList*
SSHFile::ls(const FMS_Data::LsDirOptions& options) const {
  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);

  std::pair<std::string, std::string> lsResult;
  FMS_Data::DirEntryList* result;
  FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();
  result = ecoreFactory->createDirEntryList();

  std::string lsDefaultCmd(LSCMD_DEFAULT);
  std::string lsBsdCmd(LSCMD_BSD);

  if (!exists()) {
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }

  if (options.isAllFiles()) {
    lsDefaultCmd.append("-a ");
    lsBsdCmd.append("-a ");
  }
  lsResult = ssh.exec(lsDefaultCmd+getPath());
  if (lsResult.second.find("illegal option") != std::string::npos) {
    lsResult = ssh.exec(lsBsdCmd+getPath());
  }

  if (lsResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,
                             "Error listing directory: " + lsResult.second);
  }

  std::istringstream is(lsResult.first);
  char buffer[1024];
  std::string line;
  boost::regex expression("^total [0-9]*");

  while (!is.eof()) {
    is.getline(buffer, 1024);
    line = buffer;

    // Remove empty lines and the "first" line: "total <size>"
    if (regex_search(line, expression) || line.empty()) {
      continue;
    }


    FMS_Data::DirEntry_ptr dirEntry = ecoreFactory->createDirEntry();

    // Get a Dir Entry struct from string
    File::getDirEntryFrom(line, dirEntry);
    // store it into list
    result->getDirEntries().push_back(dirEntry);
  }

  return result;
}


/* mv the file through scp. */
int
SSHFile::mv(const std::string& dest, const FMS_Data::CpFileOptions& options) {
  FMS_Data::CpFileOptions tmpOptions (options);

  if (!exists()) { //if the file does not exist
    throw FMSVishnuException(ERRCODE_INVALID_PATH,getErrorMsg());
  }

  if (false == getErrorMsg().empty()) { // other error occures
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, getErrorMsg());
  }

  if (getType() == directory) {
    tmpOptions.setIsRecursive(true);
  }

  cp(dest,tmpOptions);
  FMS_Data::RmFileOptions rmOptions;
  rmOptions.setIsRecursive("true");
  int result=rm(rmOptions);

  return result;
}



/* Transfer the file through scp or rsync. */
int
SSHFile::cp(const std::string& dest, const FMS_Data::CpFileOptions& options ) {

  boost::scoped_ptr<FileTransferCommand> transferManager(
        FileTransferCommand::getTransferManager(options, false));

  std::string remoteTranferCommand = boost::str(
                                       boost::format("%1% %2% %3")
                                       % transferManager->getCommand()
                                       % getPath()
                                       % dest);

  if (!exists()) { //if the file does not exist
    throw FMSVishnuException(ERRCODE_INVALID_PATH, getErrorMsg());
  }

  if (false == getErrorMsg().empty()) { // other error occures
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, getErrorMsg());
  }

  SSHExec ssh(sshCommand, scpCommand, sshHost, sshPort, sshUser, sshPassword,
              sshPublicKey, sshPrivateKey);

  std::pair<std::string, std::string> trResult;
  trResult = ssh.exec(remoteTranferCommand);

  if (trResult.second.find("Warning") != std::string::npos) {
    trResult = ssh.exec(remoteTranferCommand);
  }

  if (trResult.second.length() != 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR,"Error transfering file: " + trResult.second);
  }

  return 0;
}



/**
 * \brief To get A runtime error message
 * \return The error message
 */

std::string
SSHFile::getErrorMsg() const {
  return merror;
}


// Defintion of SSHExec Class

SSHExec::SSHExec(const std::string& sshCommand, const std::string& scpCommand,
                 const std::string& server, unsigned int sshPort,
                 const std::string& userName,
                 const std::string& password, const std::string& publicKey,
                 const std::string& privateKey) :
  sshCommand(sshCommand), scpCommand(scpCommand), server(server),
  sshPort(sshPort), userName(userName), password(password), publicKey(publicKey),
  privateKey(privateKey)
{
  lastExecStatus = 0;
  mprocessId = -1; //Undefined
}

const int&
SSHExec::getLastExecStatus() const {
  return lastExecStatus;
}


const int&
SSHExec::getProcessId() const {
  return mprocessId;
}

void
SSHExec::setProcessId(const int& processId) const {
  mprocessId=processId;
}


// exec a remote command
std::pair<std::string, std::string>
SSHExec::exec(const std::string& cmd) const {

  const std::string BEGIN_MARKER = "beginVishnuCommand";

  std::string command = boost::str(boost::format("%1% -l %2% -C -o BatchMode=yes "
                                                 " -o StrictHostKeyChecking=no"
                                                 " -o ForwardAgent=yes"
                                                 " -p %3% %4% ' echo %5% && %6% 2>&1 '"
                                                 )% sshCommand % userName % sshPort % server % BEGIN_MARKER % cmd);
  std::string output;
  std::pair<std::string, std::string> result;
  if (! vishnu::execSystemCommand(command, output)) { // error
    result.second = output;
  } else { // success
    size_t pos= output.find(BEGIN_MARKER);
    if (pos != std::string::npos) {
      result.first=output.substr(pos+BEGIN_MARKER.size()+1);
    }
  }

  return result;
}
