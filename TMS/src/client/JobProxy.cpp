
#include <iostream>
#include "JobProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilClient.hpp"
#include "api_fms.hpp"
#include "api_ums.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/filesystem.hpp>
#include "tmsUtils.hpp"
#include "TMSServices.hpp"
#include "DIET_client.h"
#include "TMSVishnuException.hpp"

using namespace std;
namespace bfs=boost::filesystem;
/**
 * \param sessionKey The object which encapsulates the session information
 * \param machineId The id of the machine. Optional
 * \brief Constructor
 */
JobProxy::JobProxy(const std::string& sessionKey,
                   const std::string& machineId)
  : msessionKey(sessionKey), mmachineId(machineId)
{
}

/**
 * \brief Function to submit job
 * \param scriptPath the local path of the script
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \return raises an exception on error
 */
int
JobProxy::submitJob(const std::string& scriptPath,
                    const std::string& scriptContent,
                    const TMS_Data::SubmitOptions& options) {

  JsonObject optionsData(options);

  // select a machine if not machine set
  if (mmachineId.empty() || mmachineId == AUTOM_KEYWORD) {
    TMS_Data::LoadCriterion loadCriterion;
    int criterion = optionsData.getIntProperty("criterion");
    if (criterion < 0 ) {
      loadCriterion.setLoadType(criterion);
    } else {
      loadCriterion.setLoadType(NBWAITINGJOBS);
    }
    mmachineId = vishnu::findMachine(msessionKey, loadCriterion);
  }

  // now create and initialize the service profile
  string serviceName = (boost::format("%1%@%2%")% SERVICES_TMS[JOBSUBMIT] % mmachineId).str();

  // Send input files
  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true) ;
  copts.setTrCommand(0);
  string inputFiles = vishnu::sendInputFiles(msessionKey,
                                             options.getFileParams(),
                                             mmachineId,
                                             copts);
  optionsData.setProperty("fileparams", inputFiles);
  optionsData.setProperty("scriptpath", scriptPath);

  // Set RPC pameters
  diet_profile_t* profile = diet_profile_alloc(serviceName, 4);
  diet_string_set(profile,0, msessionKey);
  diet_string_set(profile,1, mmachineId);
  diet_string_set(profile,2, scriptContent);
  diet_string_set(profile,3, optionsData.encode());

  // FIXME: do it before setting parameter 3
  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string jobSerialized;
  diet_string_get(profile,1, jobSerialized);

  JsonObject job(jobSerialized);
  mjob = job.getJob();

  diet_profile_free(profile);
  return 0;
}

/**
 * \brief Function to cancel job
 * \param options An object containing options
 * \return raises an exception on error
 */
int
JobProxy::cancelJob(const TMS_Data::CancelOptions& options) {

  std::string serviceName = (boost::format("%1%@%2%")
                             % SERVICES_TMS[JOBCANCEL]
                             % mmachineId
                             ).str();

  // Set RPC parameters
  JsonObject optionsData(options);
  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);
  diet_string_set(profile,0, msessionKey);
  diet_string_set(profile,1, mmachineId);
  diet_string_set(profile,2, optionsData.encode());

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }

  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);
  return 0;
}


/**
 * \brief Function to get job information
 * \param jobId the identifier of the job
 * \return raises an exception on error
 */
TMS_Data::Job
JobProxy::getJobInfo(const std::string& jobId) {

  TMS_Data::LoadCriterion loadCriterion;
  loadCriterion.setLoadType(NBJOBS);
  mmachineId = vishnu::findMachine(msessionKey, loadCriterion);

  std::string serviceName = (boost::format("%1%@%2%")
                             % SERVICES_TMS[JOBINFO]
                             % mmachineId
                             ).str();

  // Now prepare the service call
  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);
  diet_string_set(profile,0, msessionKey);
  diet_string_set(profile,1, mmachineId);
  diet_string_set(profile,2, jobId);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string jobData;
  diet_string_get(profile,1, jobData);
  JsonObject jobJson(jobData);

  diet_profile_free(profile);
  return jobJson.getJob();
}

/**
 * \brief Function to get job information
 * \return The job data structure
 */
TMS_Data::Job
JobProxy::getData() const{
  return mjob;
}

/**
 * \brief Destructor
 */
JobProxy::~JobProxy() {
}
