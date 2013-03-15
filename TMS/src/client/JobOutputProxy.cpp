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
 * \param machine The object which encapsulates the machine information
 * \param outDir The output directory where the files will be stored
 * (default is current directory)
 * \brief Constructor
 */
JobOutputProxy::JobOutputProxy( const SessionProxy& session,
                                const std::string& machineId,
                                const std::string& outDir)
  :msessionProxy(session), mmachineId(machineId), moutDir(outDir) {
}

/**
 * \brief Function to get the job results
 * \param jobId The Id of the
 * \return The job results data structure
 */
//TMS_Data::JobResult_ptr
TMS_Data::JobResult
JobOutputProxy::getJobOutPut(const std::string& jobId) {

  std::string serviceName = std::string(SERVICES_TMS[JOBOUTPUTGETRESULT]) + "@";
  serviceName.append(mmachineId);

  diet_profile_t* getJobOutPutProfile = diet_profile_alloc(serviceName.c_str(), 3, 3, 4);
  string sessionKey = msessionProxy.getSessionKey();
  std::string msgErrorDiet = "call of function diet_string_set is rejected ";

  //IN Parameters
  if ( diet_string_set(getJobOutPutProfile,0, sessionKey.c_str()) ) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  if ( diet_string_set(getJobOutPutProfile,1, mmachineId.c_str()) ) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  TMS_Data::JobResult jobResult; jobResult.setJobId(jobId);
  ::ecorecpp::serializer::serializer _ser;
  string jobResultToString =  _ser.serialize_str(const_cast<TMS_Data::JobResult_ptr>(&jobResult));
  if ( diet_string_set(getJobOutPutProfile,2, jobResultToString.c_str()) ) {
    msgErrorDiet += "with the job result parameter " + jobResultToString;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  if (diet_string_set(getJobOutPutProfile,3, moutDir.c_str())) {
    msgErrorDiet += "with outDir parameter "+moutDir;
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
  if (!boost::starts_with(routputInfo, "/") ) {
    raiseExceptionIfNotEmptyMsg(routputInfo);
  }
  if( moutDir.size()==0 ) {
    moutDir = bfs::path(bfs::current_path()).string() + "/DOWNLOAD_" + jobId  + vishnu::createSuffixFromCurTime();
    vishnu::createOutputDir(moutDir);
  }
  jobResult.setOutputDir(moutDir);

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp
  try {
    vishnu::genericFileCopier(sessionKey, mmachineId, routputInfo, "", boost::filesystem::temp_directory_path().string(), copts);
    string line;
    istringstream fdescStream (vishnu::get_file_content(routputInfo, false));
    if(!getline(fdescStream, line)) {
      line = "";
    }
    boost::trim(line);
    ListStrings lineVec;
    boost::split(lineVec, line, boost::is_any_of(" "));
    int nbFiles = lineVec.size();
    std::string missingFiles = "";
    if (nbFiles > 0 && line.size() >0) {
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, moutDir, copts, missingFiles, 0);
      std::string fileName = bfs::basename(lineVec[0]) + bfs::extension(lineVec[0]);
      jobResult.setOutputPath(moutDir+"/"+fileName);
      jobResult.setErrorPath(moutDir+"/"+fileName);
    }
    if (!missingFiles.empty()) {
      vishnu::saveInFile(moutDir+"/MISSINGS", missingFiles);
    }
  } catch (FMSVishnuException &ex) {
    std::cerr << boost::format("Error on file %1%: %2%")%routputInfo%ex.what();
  }

  diet_profile_free(getJobOutPutProfile);
  return jobResult;
}

/**
 * \brief Function to get the results of all job submitted
 * \return The list of the job results
 */
TMS_Data::ListJobResults_ptr
JobOutputProxy::getCompletedJobsOutput() {
  diet_profile_t* getCompletedJobsOutputProfile = NULL;
  std::string sessionKey;
  TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;

  std::string serviceName = std::string(SERVICES_TMS[JOBOUTPUTGETCOMPLETEDJOBS]) + "@";
  serviceName.append(mmachineId);

  getCompletedJobsOutputProfile = diet_profile_alloc(serviceName.c_str(), 2, 2, 4);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(getCompletedJobsOutputProfile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(getCompletedJobsOutputProfile,1, mmachineId.c_str())) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(getCompletedJobsOutputProfile,2, moutDir.c_str())) {
    msgErrorDiet += "with outDir parameter "+moutDir;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(getCompletedJobsOutputProfile,3);	//OUT Parameters
  diet_string_set(getCompletedJobsOutputProfile,4);

  //Call the Server
  if( diet_call(getCompletedJobsOutputProfile)) {
    raiseCommunicationMsgException("VISHNU call failure");
  }
  std::string routputInfo;
  if (diet_string_get(getCompletedJobsOutputProfile,3, routputInfo) ){
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
  if (diet_string_get(getCompletedJobsOutputProfile,4, listJobResultInString)) {
    msgErrorDiet += " by receiving User serialized  message";
    raiseCommunicationMsgException(msgErrorDiet);
  }
  parseEmfObject(listJobResultInString, listJobResults_ptr); /*To build the listJobResults_ptr */

  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true);
  copts.setTrCommand(0); // for using scp

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
      std::string baseDir = (moutDir.size()!=0)? bfs::absolute(moutDir).string() : bfs::path(bfs::current_path()).string();
      std::string targetDir = baseDir + "/DOWNLOAD_" + lineVec[0] + vishnu::createSuffixFromCurTime();
      vishnu::createOutputDir(targetDir);
      vishnu::copyFiles(sessionKey, mmachineId, lineVec, targetDir, copts, missingFiles, 1);
      listJobResults_ptr->getResults().get(numJob++)->setOutputDir(targetDir);
      if (!missingFiles.empty()) {
        vishnu::saveInFile(targetDir+"/MISSINGS", missingFiles);
      }
    }
  } catch (FMSVishnuException &ex) {
    std::cerr << boost::format("Error on file %1%\n%2%") % routputInfo % ex.what();
  }
  diet_profile_free(getCompletedJobsOutputProfile);
  return listJobResults_ptr;
}

/**
 * \brief Destructor
 */

JobOutputProxy::~JobOutputProxy() {
}
