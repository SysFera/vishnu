/**
 * \file SSHJobExec.cpp
 * \brief This file presents the implementation of the SSHJobExec.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SSHJobExec.hpp"

#include <boost/format.hpp>

#define CLEANUP_SUBMITTING_DATA(debugLevel) if (!debugLevel) { \
  vishnu::deleteFile(jobSerializedPath.c_str()); \
  vishnu::deleteFile(submitOptionsSerializedPath.c_str()); \
  vishnu::deleteFile(jobUpdateSerializedPath.c_str()); \
  vishnu::deleteFile(stderrFilePath.c_str()); \
  vishnu::deleteFile(errorPath.c_str()); \
  }

#define LOG(msg, logLevel) if (logLevel) std::cout << msg << "\n"

const std::string TMS_SERVER_FILES_DIR="/tmp";
const int SSH_CONNECT_RETRY_INTERVAL = 5;
const int SSH_CONNECT_MAX_RETRY = 20;
const std::string DEFAULT_SSH_OPTIONS =
    " -o UserKnownHostsFile=/dev/null"
    " -o StrictHostKeyChecking=no"
    " -o PasswordAuthentication=no";

/**
 * \brief Constructor
 * \param user the user login
 * \param hostname the hostname of the machine
 * \param batchType the type of the batch scheduler
 * \param batchVersion the version of the batch scheduler
 * \param jobSerialized the job serialized
 * \param submitOptionsSerialized the job options serialized
 */
SSHJobExec::SSHJobExec(const std::string& user,
                       const std::string& hostname,
                       const BatchType& batchType,
                       const std::string& batchVersion,
                       const std::string& jobSerialized,
                       const std::string& submitOptionsSerialized):
  muser(user), mhostname(hostname), mbatchType(batchType),
  mjobSerialized(jobSerialized),
  msubmitOptionsSerialized(submitOptionsSerialized)
{
  if (batchVersion.empty()) {
    mbatchVersion = "n/a";  // batchVersion MUST be not empty otherwise the call to the slave will failed
  } else {
    mbatchVersion = batchVersion;
  }
}

/**
 * \brief Destructor
 */
SSHJobExec::~SSHJobExec() {

}
/**
 * \brief Function to return the job serialized content
 * \return  job serialized content
 */
std::string
SSHJobExec::getJobSerialized() {
  return mjobSerialized;
}

/**
 * \brief Function to return the error message of a service
 * \return error message information
 */
std::string
SSHJobExec::getErrorInfo() {
  return merrorInfo;
}

/**
 * \brief Function to check the parameters before launching ssh
 * \return raises an exception on error
 */
void
SSHJobExec::checkSshParams() {
  if (muser.empty()) {
    throw SystemException(ERRCODE_SSH, "User login is empty");
  }
  if (mhostname.empty()) {
    throw SystemException(ERRCODE_SSH, "Server hostname is empty");
  }
}


/**
 * \brief Function to execute command by using ssh
 * \param serviceName the name of the service to execute
 * \param script_path the path to script to submit
 * \return raises an exception on error
 */
void
SSHJobExec::sshexec(const std::string& serviceName,
                    const std::string& script_path) {
  checkSshParams();
  std::string jobSerializedPath;
  std::string submitOptionsSerializedPath;
  std::string jobUpdateSerializedPath;
  std::string cmdDetails;

  jobSerializedPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/jobSerialized%%%%%%").string();
  vishnu::saveInFile(jobSerializedPath, mjobSerialized);

  submitOptionsSerializedPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/submitOptionsSerialized%%%%%%").string();
  vishnu::saveInFile(submitOptionsSerializedPath, msubmitOptionsSerialized);

  jobUpdateSerializedPath =  bfs::unique_path(TMS_SERVER_FILES_DIR+"/jobUpdateSerialized%%%%%%").string();

  cmdDetails = "" ;
  if (serviceName.compare("SUBMIT")==0) {  // set specific arguments for submit job
    cmdDetails += jobUpdateSerializedPath
                  + " " +  submitOptionsSerializedPath
                  + " " + script_path;
  }

  // For traditional batch scheduler we need to submit the job through ssh
  std::ostringstream cmd;
  if (mbatchType != DELTACLOUD) {
    cmd << "ssh -l " << muser << " " << mhostname << " "
        << " -o NoHostAuthenticationForLocalhost=yes "
        << " -o PasswordAuthentication=no ";
  }

  std::string errorPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/errorPath%%%%%%").string();
  std::string stderrFilePath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/stderr%%%%%%").string();

  std::string slaveDirectory = vishnu::getCurrentBinaryDir();
  cmd << slaveDirectory << "/tmsSlave "
      << serviceName << " "
      << convertBatchTypeToString(mbatchType) << " "
      << mbatchVersion << " "
      << jobSerializedPath << " " <<  errorPath << " "
      << cmdDetails
      << " 2> " << stderrFilePath;

  // Execute the command
  int ret = system((cmd.str()).c_str());

  // Treat possibly errors
  if (bfs::exists(errorPath)) {
     merrorInfo.append(vishnu::get_file_content(errorPath, false));
  }
  if (bfs::exists(stderrFilePath)) {
    merrorInfo.append(vishnu::get_file_content(stderrFilePath, false));
  }

  // Check if something went false
  if (ret) {
    if (merrorInfo.find("password") != std::string::npos) {
      merrorInfo.append(" You must copy the publickey in your authorized_keys file.");
    }
    if (WEXITSTATUS(ret) == 1 && mbatchType==SLURM) {//ATTENTION: 1 corresponds of the error_exit value in ../slurm_parser/opt.c
      merrorInfo.append("==> SLURM ERROR");
    }
    if (merrorInfo.empty()) {
      merrorInfo = (boost::format("Unknown error while executing the command: %1%\nError code: %2%")%cmd.str()%ret).str();
    }
    CLEANUP_SUBMITTING_DATA(mdebugLevel);
    LOG(merrorInfo, mdebugLevel);
    throw SystemException(ERRCODE_SSH, merrorInfo);
  }
// THE FOLLOWINF CODE IS ONLY FOR SUBMIT : YOU CRASH CANCEL OTHERWIZE
  if (serviceName.compare("SUBMIT") == 0) {  // set specific arguments for submit job
    bool wellSubmitted = false;

    // The command has been executed successfully
    std::string jobSerialized = vishnu::get_file_content(jobUpdateSerializedPath, false);
    TMS_Data::Job_ptr job = NULL;
    if (vishnu::parseEmfObject(std::string(jobSerialized), job)) {
      ::ecorecpp::serializer::serializer _ser;
      mjobSerialized = _ser.serialize_str(job);
      wellSubmitted = true;
      delete job;
    } else {
      merrorInfo.append("SSHJobExec::sshexec: job object is not well built");
      CLEANUP_SUBMITTING_DATA(mdebugLevel);
      LOG(merrorInfo, mdebugLevel);
      throw TMSVishnuException(ERRCODE_INVDATA, merrorInfo);
    }
    if ((mbatchType==LOADLEVELER || mbatchType==LSF) && (wellSubmitted==false)) {
      merrorInfo.append(vishnu::get_file_content(stderrFilePath, false));
      if (mbatchType==LOADLEVELER) {
        merrorInfo.append("\n==>LOADLEVELER ERROR");
      }
      if (mbatchType==LSF) {
        merrorInfo.append("\n==>LSF ERROR");
      }
    }
  }
  LOG(merrorInfo, mdebugLevel); merrorInfo.clear();
  CLEANUP_SUBMITTING_DATA(mdebugLevel);
}

/**
     * \brief Function to execute a script remotely
     * \param scriptPath the path to script to submit
     * \param nfsServer: The NFS server
     * \param nfsMountPointthe mount point on the NFS server
     * \param workDir The wordking directory of the job
     * \return raises an exception on error
     */
int
SSHJobExec::execRemoteScript(const std::string& scriptPath,
                             const std::string & nfsServer,
                             const std::string nfsMountPoint,
                             const std::string & workDir) {

  const std::string machineStatusFile = "/tmp/"+mhostname+".status";
  const std::string logfile = workDir+"/"+mhostname+".vishnu.log";
  std::ostringstream cmd;
  cmd << "exit; echo $? >" << machineStatusFile;

  int attempt = 1;
  LOG("[TMS][INFO] Checking ssh connection...", mdebugLevel);
  while(attempt <= SSH_CONNECT_MAX_RETRY) {
    execCmd(cmd.str());
    int ret =  vishnu::getStatusValue(machineStatusFile);
    if(ret == 0) {
      break;
    }
    sleep(SSH_CONNECT_RETRY_INTERVAL);
    attempt++;
  }
  vishnu::deleteFile(machineStatusFile.c_str());

  // If not succeed throw exception
  if(attempt > SSH_CONNECT_MAX_RETRY) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             "execRemoteScript:: can't log into the machine "+mhostname+" after "
                             + vishnu::convertToString(SSH_CONNECT_MAX_RETRY*SSH_CONNECT_RETRY_INTERVAL)+" seconds");
  }

  // Mount the NFS repository
  LOG("[TMS][INFO] Mounting the nfs directory...", mdebugLevel);
  if(nfsServer.size()>0 &&
     nfsMountPoint.size()> 0) {
    mountNfsDir(nfsServer, nfsMountPoint);
  }

  // If succeed execute the script to the virtual machine
  // This assumes that the script is located on a shared DFS
  LOG("[TMS][INFO] Executing the script...", mdebugLevel);
  execCmd("'mkdir -p "+workDir+" &>>"+logfile+"'"); // First create the output directory if it not exist
  int pid = -1;
  if(execCmd(scriptPath + " &>>"+logfile, true, workDir, &pid)) {
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             "execRemoteScript:: failed when executing the script "
                             + scriptPath + " in the virtual machine "+mhostname);
  }
  LOG("[TMS][INFO] Submission completed. PID:"+pid, mdebugLevel);
  return pid;
}


/**
     * \brief Function to convert the batch type to string
     * \param BatchType the batch type to convert
     * \return the converted batch type
     */
std::string SSHJobExec::convertBatchTypeToString(BatchType batchType) {
  std::string value;
  switch(batchType) {
  case TORQUE:
    value = "TORQUE";
    break;
  case LOADLEVELER:
    value = "LOADLEVELER";
    break;
  case SLURM:
    value = "SLURM";
    break;
  case LSF:
    value = "LSF";
    break;
  case SGE:
    value = "SGE";
    break;
  case PBSPRO:
    value = "PBS";
    break;
  case DELTACLOUD:
    value = "DELTACLOUD";
    break;
  case POSIX:
    value = "POSIX";
    break;
  default:
    value = "UNKNOWN_BATCH_TYPE";
    break;
  }
  return value;
}

/**
     * \brief Function to copy files from remote machine
     * \param outputPath the output path to get
     * \param errorPath the error path to get
     * \param copyOfOutputPath the copy of the outputPath
     * \param copyOfErrorPath the copy of errorPath
     * \return raises an exception on error
     */
int
SSHJobExec::copyFiles(const std::string& outputPath,
                      const std::string& errorPath,
                      const char* copyOfOutputPath,
                      const char* copyOfErrorPath) {

  std::ostringstream cmd1;
  cmd1 << "scp " << DEFAULT_SSH_OPTIONS << " "
       << muser << "@" << mhostname << ":" << outputPath << " " << copyOfOutputPath;

  if (system((cmd1.str()).c_str())) {
    return -1;
  }

  std::ostringstream cmd2;
  cmd2 << "scp "<< DEFAULT_SSH_OPTIONS << " "
       << muser << "@" << mhostname << ":" << errorPath << " " << copyOfErrorPath;
  if (system((cmd2.str()).c_str())) {
    return -1;
  }

  return 0;
}

/**
     * \brief Function to copy files from remote machine
     * \param path the path of the file
     * \return raises an exception on error
     */
int
SSHJobExec::copyFile(const std::string& path, const std::string& dest) {

  std::ostringstream cmd1;
  cmd1 << "scp " << DEFAULT_SSH_OPTIONS << " "
       << muser << "@" << mhostname << ":" << path << " " << dest;
  if (system((cmd1.str()).c_str())) {
    return -1;
  }
  return 0;
}

/**
     * \brief Function to execute a command via ssh
     * \param cmd the command to execute
     * \param background: Tell whether launch the script is background
     * \param outDir the directory when the output will be stored
     * \param pid: return value containing the pid of the of the running background process
     */
int
SSHJobExec::execCmd(const std::string& cmd,
                    const bool & background,
                    const std::string& outDir,
                    int* pid) {

  std::string pidFile = "$HOME/vishnu.pid";
  std::ostringstream sshCmd;
  sshCmd << "ssh " << DEFAULT_SSH_OPTIONS << " "
         << muser << "@" << mhostname << " ";

  if( ! background) {
    sshCmd << cmd;
  } else {
    try {
      pidFile =  bfs::unique_path("/tmp/vishnu.pid%%%%%%").string();
    } catch(...) {} // The pid file will be created in $HOME/vishnu.pid

    sshCmd << "'" << cmd << " 1>"+outDir+"/stdout 2>"+outDir+"/stderr & echo $!' >" << pidFile;
  }
  LOG(sshCmd.str(), mdebugLevel);
  if(system((sshCmd.str()).c_str())) {
    return -1;
  }

  // Retrieve the pid if the process was launched in background
  if(background && pid != NULL) {
    *pid = vishnu::getStatusValue (pidFile);
    vishnu::deleteFile(pidFile.c_str());
  }

  return 0;
}

/**
     * \brief To mount a NFS directory to a remote server
     * \param host: The NFS server
     * \param point the mount point on the NFS server
     */
void
SSHJobExec::mountNfsDir(const std::string & host, const std::string point) {

  // Create the command mkdir + mount
  std::ostringstream cmd;
  cmd << "'mkdir "+point+" && "
      << "mount -t nfs -o rw,nolock,vers=3 "+host+":"+point+" "+point+"'";
  if(execCmd(cmd.str(), false)) { // run in foreground
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             "mountNfsDir:: failed to mount the directory "+point);
  }
}
