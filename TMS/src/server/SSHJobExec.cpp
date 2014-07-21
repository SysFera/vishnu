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
#include <boost/format.hpp>
#include <ecore.hpp>
#include <ecorecpp.hpp>
#include "TMS_Data.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SSHJobExec.hpp"
#include "Logger.hpp"

#define CLEANUP_SUBMITTING_DATA(debugLevel) if (!debugLevel) { \
  vishnu::deleteFile(submitOptionsSerializedPath.c_str()); \
  vishnu::deleteFile(jobUpdateSerializedPath.c_str()); \
  vishnu::deleteFile(stderrFilePath.c_str()); \
  vishnu::deleteFile(errorPath.c_str()); \
  }

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
 * \param jobSteps The list of steps
 * \return raises an exception on error
 */
void
SSHJobExec::sshexec(const std::string& actionName,
                    const std::string& script_path,
                    TMS_Data::ListJobs& jobSteps) {
  checkSshParams();
  std::string submitOptionsSerializedPath;


  std::string jobUpdateSerializedPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/jobResultSerialized%%%%%%").string();
  submitOptionsSerializedPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/submitOptionsSerialized%%%%%%").string();
  vishnu::saveInFile(submitOptionsSerializedPath, msubmitOptionsSerialized);


  std::string detailsForSubmit = "" ;
  if (actionName == "SUBMIT") {
    detailsForSubmit = boost::str(boost::format("%1% %2% %3%")
                                  % jobUpdateSerializedPath
                                  % submitOptionsSerializedPath
                                  % script_path);
  }

  // For traditional batch scheduler we need to submit the job through ssh
  // Not the case for cloud where we make RPC call
  std::string cmd;
  if (mbatchType != DELTACLOUD
      && mbatchType != OPENNEBULA) {
    cmd = boost::str(boost::format("ssh -l %1% %2% "
                                   "-o NoHostAuthenticationForLocalhost=yes "
                                   "-o PasswordAuthentication=no "
                                   )   % muser % mhostname);
  }

  std::string errorPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/errorPath%%%%%%").string();
  std::string stderrFilePath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/stderr%%%%%%").string();
  std::string jobPath = bfs::unique_path(TMS_SERVER_FILES_DIR+"/jobSerialized%%%%%%").string();

  vishnu::saveInFile(jobPath, mjobSerialized);

  cmd += boost::str(boost::format("tmsSlave %1% %2% %3% %4% %5% %6% 2> %7%")
                    % actionName
                    % vishnu::convertBatchTypeToString(mbatchType)
                    % mbatchVersion
                    % jobPath
                    % errorPath
                    % detailsForSubmit
                    % stderrFilePath);

  std::string msgError;
  // Execute the command and treat the possibly errors and check output
  if (! vishnu::execSystemCommand(cmd, msgError)) {
    if (merrorInfo.find("password") != std::string::npos) {
      merrorInfo.append("\nYou must copy the publickey in your authorized_keys file.");
    }
    if (merrorInfo.empty()) {
      merrorInfo = boost::str(boost::format("Unknown error while executing the command: %1%") % cmd);
    }
    CLEANUP_SUBMITTING_DATA(mdebugLevel);
    LOG(merrorInfo, LogErr);
    throw SystemException(ERRCODE_SSH, merrorInfo);
  }

  if (bfs::exists(errorPath)) {
    merrorInfo.append(vishnu::get_file_content(errorPath, false));
  }
  if (bfs::exists(stderrFilePath)) {
    merrorInfo.append(vishnu::get_file_content(stderrFilePath, false));
  }

  // THE FOLLOWIND CODE IS ONLY FOR SUBMIT : YOU CRASH CANCEL OTHERWIZE
  if (actionName == "SUBMIT") {
    if (bfs::exists(jobUpdateSerializedPath)) {
      TMS_Data::ListJobs_ptr jobStepsPtr;
      std::string jobResult;
      jobResult = vishnu::get_file_content(jobUpdateSerializedPath, false);
      if (! vishnu::parseEmfObject(jobResult, jobStepsPtr)) {
        LOG("[ERROR] sshexec: cannot parse result", LogErr);
        merrorInfo.clear();
      } else {
        TMS_Data::TMS_DataFactory_ptr ecoreFactory = TMS_Data::TMS_DataFactory::_instance();
        jobSteps = *(ecoreFactory->createListJobs());
        merrorInfo.append("\n").append(vishnu::get_file_content(stderrFilePath, false));
        for (unsigned int j = 0; j < jobStepsPtr->getJobs().size(); j++) {
          TMS_Data::Job_ptr job = ecoreFactory->createJob();
          job->setSubmitError(merrorInfo);
          *job = *jobStepsPtr->getJobs().get(j); //copy the content and not the pointer
          jobSteps.getJobs().push_back(job);
        }
        jobSteps.setNbJobs(jobStepsPtr->getJobs().size());
        jobSteps.setNbRunningJobs(jobStepsPtr->getNbRunningJobs());
        jobSteps.setNbWaitingJobs(jobStepsPtr->getNbWaitingJobs());
      }
    } else {
      throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, merrorInfo);
    }

  } else {
    TMS_Data::Job_ptr jobPtr = new TMS_Data::Job();
    JsonObject jobJson(mjobSerialized);
    *jobPtr = jobJson.getJob();
    jobPtr->setStatus(vishnu::STATE_CANCELLED);
    jobSteps = TMS_Data::ListJobs();
    jobSteps.getJobs().push_back(jobPtr);

  }

  if (! merrorInfo.empty()) {
    LOG(merrorInfo, LogInfo);
    merrorInfo.clear();
  }
  CLEANUP_SUBMITTING_DATA(mdebugLevel);
}

/**
     * \brief Function to execute a script remotely
     * \param scriptPath the path to script to submit
     * \param workDir The wordking directory of the job
     * \param scriptPid OUT script pid
     * \return raises an exception on error
     */
int
SSHJobExec::execRemoteScript(const std::string& scriptPath,
                             const std::string& workingDir,
                             int& scriptPid) {

  if (execCmd(scriptPath, workingDir, true, scriptPid) != 0) {
    scriptPid = -1;
    LOG(boost::str(boost::format("[WARN] execRemoteScript:: failed executing %1% on VM %2%")
                   % scriptPath
                   % mhostname), LogWarning);
  } else {
    LOG("[INFO] Submission completed. PID:"+scriptPid, LogInfo);
  }
  return scriptPid;
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
     * \param workingDir The working directory
     * \param background: Tell whether launch the script is background
     * \param pid: return value containing the pid of the of the running background process
     */
int
SSHJobExec::execCmd(const std::string& cmd,
                    const std::string& workingDir,
                    const bool & background,
                    int& pid) {

  std::string pidFile = boost::str(boost::format("%1%/PID") % workingDir);
  std::string sshCmd;
  if (background) {
    sshCmd =  boost::str(boost::format("ssh %1% %2%@%3% '%4% 1> %5%/stdout 2> %5%/stderr & echo $!' > %6%")
                         % DEFAULT_SSH_OPTIONS
                         % muser
                         % mhostname
                         % cmd
                         % workingDir
                         % pidFile);
  } else {
    sshCmd =  boost::str(boost::format("ssh %1% %2%@%3% %4%")
                         % DEFAULT_SSH_OPTIONS
                         % muser
                         % mhostname
                         % cmd);
  }

  LOG(boost::str(boost::format("[INFO][CMD] %1%") % sshCmd), LogInfo);

  int ret = 0;
  if (system(sshCmd.c_str())) {
    ret = -1;
  }

  if (background) {
    pid = vishnu::getStatusValue(pidFile);
    vishnu::deleteFile(pidFile.c_str());
    ret = 0;
  }
  return ret;
}

/**
     * \brief To mount a NFS directory to a remote server
     * \param host: The NFS server
     * \param point the mount point on the NFS server
     */
void
SSHJobExec::mountNfsDir(const std::string & host, const std::string point) {
  std::string remoteCmd = boost::str(boost::format("'mkdir -p %1% && mount -t nfs -o rw,nolock,vers=3 %2%:%1% %1%'") % point % host);
  int pid;
  if(execCmd(remoteCmd, point, false, pid)) { // run in foreground
    throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
                             "mountNfsDir:: failed to mount the directory "+point);
  }
}


/**
 * @brief Check if the ssh connexion is ready
 * @return true on success, false otherwise
 */
bool
SSHJobExec::isReadyConnection(void)
{
  bool isReady =  false;
  const std::string statusFile = boost::str(boost::format("/tmp/%1%.sshstatus") % mhostname);
  const std::string remoteCmd = boost::str(boost::format("exit; echo $? > %1% ") % statusFile);

  int pid;
  execCmd(remoteCmd, "", false, pid);
  int ret =  vishnu::getStatusValue(statusFile);
  if(ret == 0) {
    isReady = true;
  }
  vishnu::deleteFile(statusFile.c_str());

  return isReady;
}
