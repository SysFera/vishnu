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
using namespace vishnu;
using namespace std;

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
  string sessionKey = msessionProxy.getSessionKey();

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

  std::string routputInfo;
  diet_string_get(profile,1, routputInfo);

  if (routputInfo.empty()) {
    throw TMSVishnuException(ERRCODE_INVDATA, "Weird behavior: no output to retrieve");
  }

  if (! boost::starts_with(routputInfo, "/") ) {
    raiseExceptionIfNotEmptyMsg(routputInfo);
  }
  if (outputDir.empty()) {
    outputDir = bfs::path(bfs::current_path()).string() + "/DOWNLOAD_" + jobId  + vishnu::createSuffixFromCurTime();
    vishnu::createOutputDir(outputDir);
  }

  jobResult.setOutputDir(outputDir);

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp
  try {
    vishnu::genericFileCopier(sessionKey, mmachineId, routputInfo, "", outputDir, copts);
    std::string infoFile = (boost::format("%1%/%2%") % outputDir % bfs::basename(routputInfo)).str();
    istringstream fdescStream(vishnu::get_file_content(infoFile, false));
    string line;
    if(! getline(fdescStream, line)) {
      line = "";
    }
    boost::trim(line);
    ListStrings lineVec;
    boost::split(lineVec, line, boost::is_any_of(" "));
    int nbFiles = lineVec.size();

    std::string missingFileContent = "";
    if (! line.empty() && nbFiles > 0) {
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, outputDir, copts, missingFileContent, 0);
      std::string fileName = bfs::basename(lineVec[0]) + bfs::extension(lineVec[0]);
      jobResult.setOutputPath(outputDir+"/"+fileName);
      std::string fileName2 = bfs::basename(lineVec[1]) + bfs::extension(lineVec[1]);
      jobResult.setErrorPath(outputDir+"/"+fileName2);
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
  std::string routputInfo = jsonData.getStringProperty("infofile");
  std::string jobListSerialized = jsonData.getStringProperty("joblist");

  if (routputInfo.empty()) {
    throw TMSVishnuException(ERRCODE_INVDATA, "Weird behavior: no output to retrieve");
  }
  if (!boost::starts_with(routputInfo, "/") ) {
    raiseExceptionIfNotEmptyMsg(routputInfo);
  }

  TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;
  parseEmfObject(jobListSerialized, listJobResults_ptr); /*To build the listJobResults_ptr */

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp

  std::string outputDir = options.getOutputDir();
  try {
    vishnu::genericFileCopier(sessionKey, mmachineId, routputInfo, "", boost::filesystem::temp_directory_path().string(), copts);
    istringstream fdescStream (vishnu::get_file_content(routputInfo, false));
    int numJob = 0;
    string line;
    ListStrings lineVec;
    string missingFiles;
    while (getline(fdescStream, line)) {
      if (line.empty()) continue;
      boost::trim(line);
      boost::split(lineVec, line, boost::is_any_of(" "));
      std::string baseDir = (! outputDir.empty())? bfs::absolute(outputDir).string() : bfs::path(bfs::current_path()).string();
      std::string targetDir = baseDir + "/DOWNLOAD_" + lineVec[0] + vishnu::createSuffixFromCurTime();
      vishnu::createOutputDir(targetDir);
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, targetDir, copts, missingFiles, 1);
      listJobResults_ptr->getResults().get(numJob++)->setOutputDir(targetDir);
      if (!missingFiles.empty()) {
        vishnu::saveInFile(targetDir+"/MISSINGFILES", missingFiles);
      }
    }
  } catch (FMSVishnuException &ex) {
    vishnu::saveInFile(outputDir+"/ERROR",
                       (boost::format("File %1%: %2%")%routputInfo%ex.what()).str());
  }
  diet_profile_free(profile);
  return listJobResults_ptr;
}

/**
 * \brief Destructor
 */

JobOutputProxy::~JobOutputProxy() {
}
