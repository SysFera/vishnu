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
JobOutputProxy::getJobOutPut(const std::string& jobId, const TMS_Data::JobOuputOptions& options) {

  std::string serviceName = (boost::format("%1%@%2%") % SERVICES_TMS[JOBOUTPUTGETRESULT]  %mmachineId).str();

  diet_profile_t* getJobOutPutProfile = diet_profile_alloc(serviceName, 3, 3, 4);
  string sessionKey = msessionProxy.getSessionKey();
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";

  //IN Parameters
  if ( diet_string_set(getJobOutPutProfile,0, sessionKey) ) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  if ( diet_string_set(getJobOutPutProfile,1, mmachineId) ) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  TMS_Data::JobResult jobResult; jobResult.setJobId(jobId);
  ::ecorecpp::serializer::serializer _ser;
  string jobResultToString =  _ser.serialize_str(const_cast<TMS_Data::JobResult_ptr>(&jobResult));
  if ( diet_string_set(getJobOutPutProfile,2, jobResultToString) ) {
    msgErrorDiet += "with the job result parameter " + jobResultToString;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  std::string outputDir = options.getOutputDir();
  if (diet_string_set(getJobOutPutProfile, 3, outputDir)) {
    msgErrorDiet += "with output directory parameter "+outputDir;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  //OUT Parameter
  diet_string_set(getJobOutPutProfile,4);

  //Call the Server
  if (diet_call(getJobOutPutProfile)) {
    raiseCommunicationMsgException("VISHNU call failure");
  }
  std::string routputInfo;
  if (diet_string_get(getJobOutPutProfile,4, routputInfo)){
    msgErrorDiet += " by receiving outputInfo";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  if (routputInfo.empty()) {
    throw TMSVishnuException(ERRCODE_INVDATA, "Weird behavior: no output to retrieve");
  }
  if (! boost::starts_with(routputInfo, "/") ) {
    raiseExceptionIfNotEmptyMsg(routputInfo);
  }
  if (outputDir.empty()) {
    outputDir = bfs::path(bfs::current_path()).string() + "/DOWNLOAD_" + jobId  + vishnu::createSuffixFromCurTime();
    vishnu::createOutputDir(outputDir);
    jobResult.setOutputDir(outputDir);
  }

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp
  try {
    vishnu::genericFileCopier(sessionKey, mmachineId, routputInfo, "", boost::filesystem::temp_directory_path().string(), copts);
    string line;
    istringstream fdescStream (vishnu::get_file_content(routputInfo, false));
    if(! getline(fdescStream, line)) {
      line = "";
    }
    boost::trim(line);
    ListStrings lineVec;
    boost::split(lineVec, line, boost::is_any_of(" "));
    int nbFiles = lineVec.size();
    std::string missingFiles = "";
    if (nbFiles > 0 && ! line.empty()) {
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, outputDir, copts, missingFiles, 0);
      std::string fileName = bfs::basename(lineVec[0]) + bfs::extension(lineVec[0]);
      jobResult.setOutputPath(outputDir+"/"+fileName);
      std::string fileName2 = bfs::basename(lineVec[1]) + bfs::extension(lineVec[1]);
      jobResult.setErrorPath(outputDir+"/"+fileName2);
    }
    if (! missingFiles.empty()) {
      vishnu::saveInFile(outputDir+"/MISSINGFILES", missingFiles);
    }
  } catch (FMSVishnuException &ex) {
    vishnu::saveInFile(outputDir+"/ERROR",
                       (boost::format("File %1%: %2%") % routputInfo % ex.what()).str());
  }

  diet_profile_free(getJobOutPutProfile);
  return jobResult;
}

/**
 * \brief Function to get the results of all job submitted
 * \return The list of the job results
 */
TMS_Data::ListJobResults_ptr
JobOutputProxy::getCompletedJobsOutput(const TMS_Data::JobOuputOptions& options) {
  diet_profile_t* getCompletedJobsOutputProfile = NULL;
  std::string sessionKey;
  TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;

  std::string serviceName = (boost::format("%1%@%2%") % SERVICES_TMS[JOBOUTPUTGETCOMPLETEDJOBS]  %mmachineId).str();

  getCompletedJobsOutputProfile = diet_profile_alloc(serviceName, 2, 2, 4);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(getCompletedJobsOutputProfile,0, sessionKey)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(getCompletedJobsOutputProfile, 1, mmachineId)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _serializer;
  string optionsSerialized = _serializer.serialize_str(const_cast<TMS_Data::JobOuputOptions_ptr>(&options));
  if (diet_string_set(getCompletedJobsOutputProfile, 2, optionsSerialized)) {
    msgErrorDiet += "with sending serialized options";
    raiseCommunicationMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(getCompletedJobsOutputProfile, 3);	//OUT Parameters
  diet_string_set(getCompletedJobsOutputProfile, 4);

  //Call the Server
  if( diet_call(getCompletedJobsOutputProfile)) {
    raiseCommunicationMsgException("VISHNU call failure");
  }
  std::string routputInfo;
  if (diet_string_get(getCompletedJobsOutputProfile, 3, routputInfo) ){
    msgErrorDiet += " by receiving outputInfo";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  if (routputInfo.empty()) {
    throw TMSVishnuException(ERRCODE_INVDATA, "Weird behavior: no output to retrieve");
  }
  if (!boost::starts_with(routputInfo, "/") ) {
    raiseExceptionIfNotEmptyMsg(routputInfo);
  }

  std::string listJobResultInString;
  if (diet_string_get(getCompletedJobsOutputProfile, 4, listJobResultInString)) {
    msgErrorDiet += " by receiving User serialized  message";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  parseEmfObject(listJobResultInString, listJobResults_ptr); /*To build the listJobResults_ptr */

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
  diet_profile_free(getCompletedJobsOutputProfile);
  return listJobResults_ptr;
}

/**
 * \brief Destructor
 */

JobOutputProxy::~JobOutputProxy() {
}
