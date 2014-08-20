#include <string>
#include <iostream>
#include "JobOutputProxy.hpp"
#include "utilClient.hpp"
#include "DIET_client.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "utilVishnu.hpp"
#include "api_fms.hpp"
#include "tmsUtils.hpp"
#include "TMSServices.hpp"
#include "TMSVishnuException.hpp"

namespace bfs = boost::filesystem;

/**
 * \param session The object which encapsulates the session information
 * \param machineId The target machine
 * \brief Constructor
 */
JobOutputProxy::JobOutputProxy( const SessionProxy& session, const std::string& machineId)
  : msessionProxy(session), mmachineId(machineId)
{
}

/**
 * \brief Function to get the job results
 * \param jobId The Id of the
 * \param options Object containing the user-provided options
 * \return The job results data structure
 */
TMS_Data::JobResult
JobOutputProxy::getJobOutPut(const std::string& jobId, const TMS_Data::JobOutputOptions& options) {

  std::string serviceName = boost::str(boost::format("%1%@%2%") % SERVICES_TMS[JOBOUTPUTGETRESULT]  %mmachineId);

  diet_profile_t* profile = diet_profile_alloc(serviceName, 4);
  std::string sessionKey = msessionProxy.getSessionKey();

  //IN Parameters
  TMS_Data::JobResult jobResult; jobResult.setJobId(jobId);
  std::string outputDir = options.getOutputDir();
  JsonObject optionsData(options);

  diet_string_set(profile,0, sessionKey);
  diet_string_set(profile,1, mmachineId);
  diet_string_set(profile,2, optionsData.encode());
  diet_string_set(profile, 3, jobId);

  //Call the Server
  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string remoteOutputInfo;
  diet_string_get(profile,1, remoteOutputInfo);

  if (remoteOutputInfo.empty()) {
    throw TMSVishnuException(ERRCODE_INVDATA, "Weird behavior: no output to retrieve");
  }

  if (! boost::starts_with(remoteOutputInfo, "/") ) {
    raiseExceptionIfNotEmptyMsg(remoteOutputInfo);
  }

  if (outputDir.empty()) {
    outputDir = (boost::format("%1%/VISHNU_DOWNLOAD_%2%")
                 % bfs::path(bfs::current_path()).string()
                 % vishnu::generatedUniquePatternFromCurTime(jobId)).str();
    vishnu::createOutputDir(outputDir);
  }

  jobResult.setOutputDir(outputDir);

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp
  try {
    std::string downloadInfoFile = boost::str(
                                     boost::format("%1%/%2%")
                                     % outputDir
                                     % boost::filesystem::unique_path("vishnu-%%%%%%.dinfo").string());

    vishnu::genericFileCopier(sessionKey, mmachineId, remoteOutputInfo, "", downloadInfoFile, copts);

    std::istringstream dataStream(vishnu::get_file_content(downloadInfoFile, false));

    std::string line;
    if(! getline(dataStream, line)) {
      line = "";
    }
    boost::trim(line);
    std::vector<std::string> lineVec;
    boost::split(lineVec, line, boost::is_any_of(" "));
    int nbFiles = lineVec.size();

    std::string missingFileContent;
    if (! line.empty() && nbFiles > 0) {
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, outputDir, copts, missingFileContent, 0);
      std::string outputPath = "";
      std::string errorPath = "";
      if (nbFiles >= 1) {
        outputPath = (boost::format("%1%/%2%%3%")
                      % outputDir % bfs::basename(lineVec[0]) % bfs::extension(lineVec[0])).str();
      }
      if (nbFiles >= 2) {
        errorPath = (boost::format("%1%/%2%%3%")
                     % outputDir % bfs::basename(lineVec[1]) % bfs::extension(lineVec[0])).str();
      }
      jobResult.setOutputPath(outputPath);
      jobResult.setErrorPath(errorPath);
    }

    if (! missingFileContent.empty()) {
      std::string missingFileName = (boost::format("%1%/MISSINGFILES_%2%") % outputDir % jobId).str();
      vishnu::saveInFile(missingFileName, missingFileContent);
    }
  } catch (VishnuException &ex) {
    std::string errorFileName = (boost::format("%1%/ERROR_%2%") % outputDir % jobId).str();
    vishnu::saveInFile(errorFileName, ex.what());
  }

  diet_profile_free(profile);
  return jobResult;
}

/**
 * \brief Function to get the results of all job submitted
 * \return The list of the job results
 */
TMS_Data::ListJobResults_ptr
JobOutputProxy::getCompletedJobsOutput(const TMS_Data::JobOutputOptions& options) {

  std::string serviceName = boost::str(boost::format("%1%@%2%")
                                       % SERVICES_TMS[JOBOUTPUTGETCOMPLETEDJOBS]
                                       % mmachineId);

  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);
  std::string sessionKey = msessionProxy.getSessionKey();
  JsonObject optionsData(options);

  //IN Parameters
  if (diet_string_set(profile,0, sessionKey)) {
    raiseCommunicationMsgException("Exception setting sessionkey parameter");
  }

  if (diet_string_set(profile, 1, mmachineId)) {
    raiseCommunicationMsgException("Exception setting machineid parameter");
  }

  if (diet_string_set(profile, 2, optionsData.encode())) {
    raiseCommunicationMsgException("Exception setting option parameter");
  }

  //Call the Server
  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  // Get output, which is a json object. See internalApi.cpp
  std::string data;
  diet_string_get(profile,1, data);

  // Treat the json object
  JsonObject jsonData(data);
  std::string remoteOutputInfo = jsonData.getStringProperty("infofile");
  std::string jobListSerialized = jsonData.getStringProperty("joblist");

  if (remoteOutputInfo.empty()
      || ! boost::starts_with(remoteOutputInfo, "/")) {
    throw TMSVishnuException(ERRCODE_INVDATA, boost::str(boost::format("Weird output info file [%1%]"
                                                                       ) % remoteOutputInfo));
  }
  TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;
  parseEmfObject(jobListSerialized, listJobResults_ptr);

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp

  std::string outputDir = options.getOutputDir();
  try {
    std::string downloadInfoFile = boost::str(boost::format("%1%/%2%")
                                              % boost::filesystem::temp_directory_path().string()
                                              % boost::filesystem::unique_path("vishnu-%%%%%%.dinfo").string());
    vishnu::genericFileCopier(sessionKey, mmachineId, remoteOutputInfo, "", downloadInfoFile, copts);
    std::istringstream downloadInfoStream (vishnu::get_file_content(downloadInfoFile, false));
    int numJob = 0;
    std::string line;
    std::vector<std::string> lineVec;
    std::string missingFiles;
    while (getline(downloadInfoStream, line)) {
      if (line.empty()) continue;
      boost::trim(line);
      boost::split(lineVec, line, boost::is_any_of(" "));
      std::string baseDir = (! outputDir.empty())? bfs::absolute(outputDir).string() : bfs::path(bfs::current_path()).string();
      std::string targetDir = boost::str(boost::format("%1%/VISHNU_DOWNLOAD_%2%")
                                         % baseDir
                                         % vishnu::generatedUniquePatternFromCurTime(lineVec[0]));
      vishnu::createOutputDir(targetDir);
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, targetDir, copts, missingFiles, 1);
      listJobResults_ptr->getResults().get(numJob++)->setOutputDir(targetDir);
      if (! missingFiles.empty()) {
        vishnu::saveInFile(targetDir+"/MISSINGFILES", missingFiles);
      }
    }
  } catch (FMSVishnuException &ex) {
    vishnu::saveInFile(outputDir+"/ERROR",
                       boost::str(boost::format("File %1%: %2%") % remoteOutputInfo % ex.what()));
  }
  diet_profile_free(profile);
  return listJobResults_ptr;
}

/**
 * \brief Destructor
 */

JobOutputProxy::~JobOutputProxy() {
}
