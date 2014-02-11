
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
 * \param session The object which encapsulates the session information
 * \param job The job data structure
 * \param machineId The id of the machine. Optional
 * \brief Constructor
 */
JobProxy::JobProxy(const SessionProxy& session,
                   TMS_Data::Job& job,
                   const std::string& machineId)
  :msessionProxy(session), mjob(job), mmachineId(machineId)
{

}

/**
 * \brief Function to submit job
 * \param scriptContent the content of the script
 * \param options the options to submit job
 * \return raises an exception on error
 */
int
JobProxy::submitJob(const std::string& scriptContent,
                    const TMS_Data::SubmitOptions& options) {

  string sessionKey = msessionProxy.getSessionKey();
  TMS_Data::SubmitOptions& updatedOptions = const_cast<TMS_Data::SubmitOptions&>(options) ;

  // first check if it's an automatic submission
  // if yes, select a machine according to the load criterion
  if (mmachineId.empty() || mmachineId == AUTOM_KEYWORD) {
    if (updatedOptions.getCriterion()) {
      mmachineId = vishnu::findMachine(sessionKey, *(updatedOptions.getCriterion()));
    } else {
      TMS_Data::LoadCriterion loadCriterion;
      loadCriterion.setLoadType(NBWAITINGJOBS);
      mmachineId = vishnu::findMachine(sessionKey, loadCriterion);
    }
  }

  // now create and initialize the service profile
  string serviceName = std::string(SERVICES_TMS[JOBSUBMIT]) + "@";
  serviceName.append(mmachineId);
  diet_profile_t* profile = diet_profile_alloc(serviceName, 4);

  //IN Parameters
  diet_string_set(profile,0, sessionKey);
  diet_string_set(profile,1, mmachineId);
  diet_string_set(profile,2, scriptContent);

  // Send input files, if there is any one
  FMS_Data::CpFileOptions copts;
  copts.setIsRecursive(true) ;
  copts.setTrCommand(0);
  string inputFiles = vishnu::sendInputFiles(sessionKey, updatedOptions.getFileParams(), mmachineId, copts) ;
  updatedOptions.setFileParams(inputFiles);

  JsonObject jsonOptions(updatedOptions);
  jsonOptions.setProperty("owner");
  diet_string_set(profile,3, jsonOptions.encode());

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

  std::string errorInfo;

  std::string serviceName = (boost::format("%1%@%2%")
                             % SERVICES_TMS[JOBCANCEL]
                             % mmachineId
                             ).str();

  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);
  std::string  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "preparing sending data failed ";
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_string_set(profile,1, mmachineId)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  string serializedOptions =  _ser.serialize_str(const_cast<TMS_Data::CancelOptions_ptr>(&options));
  if (diet_string_set(profile,2, serializedOptions)) {
    msgErrorDiet += " on serializing options: "+serializedOptions;
    raiseCommunicationMsgException(msgErrorDiet);
  }

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }

  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);
  return 0;
}


/**
 * \brief Function to get job information
 * \param options the options to submit job
 * \return raises an exception on error
 */
TMS_Data::Job
JobProxy::getJobInfo() {

  std::string sessionKey = msessionProxy.getSessionKey();
  TMS_Data::LoadCriterion loadCriterion;
  loadCriterion.setLoadType(NBJOBS);
  mmachineId = vishnu::findMachine(sessionKey, loadCriterion);

  std::string serviceName = (boost::format("%1%@%2%")
                             % SERVICES_TMS[JOBINFO]
                             % mmachineId
                             ).str();

  // Now prepare the service call
  diet_profile_t* profile = diet_profile_alloc(serviceName, 3);
  std::string jobSerialized = JsonObject::serialize(mjob);

  // Set parameters
  diet_string_set(profile, 0, sessionKey);
  diet_string_set(profile, 1, mmachineId);
  diet_string_set(profile,2, jobSerialized);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_string_get(profile,1, jobSerialized);
  JsonObject jobJson(jobSerialized);

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
