#include <string>
#include <iostream>
#include "JobOutPutProxy.hpp"
#include "utilsClient.hpp"
#include "utilsTMSClient.hpp"
#include "emfTMSUtils.hpp"
#include "DIET_Dagda.h"
#include <boost/filesystem.hpp>
#include "utilVishnu.hpp"

namespace bfs = boost::filesystem;
using namespace vishnu;

/**
* \param session The object which encapsulates the session information
* \param machine The object which encapsulates the machine information
* \param jobResult The job results data structure
* \param listJobResults the list of job results data structure
* \brief Constructor
*/
JobOutPutProxy::JobOutPutProxy( const SessionProxy& session,
                  const std::string& machineId)
:msessionProxy(session), mmachineId(machineId) {
}

/**
* \brief Function to get the job results
* \param jobId The Id of the
* \return The job results data structure
*/
TMS_Data::JobResult_ptr
JobOutPutProxy::getJobOutPut(const std::string& jobId) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* jobResultToString;
  char* jobResultInString;
  char* errorInfo = NULL;
  char* IDContainer = NULL;
  diet_container_t content;
  char* outputPath = NULL;
  char* errorPath = NULL;
  TMS_Data::JobResult_ptr outJobResult;

  TMS_Data::JobResult jobResult;
  jobResult.setJobId(jobId);

  std::string serviceName = "jobOutPutGetResult_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 2, 2, 5);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
   //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

   const char* name = "getJobOutPut";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the options object in to optionsInString
  jobResultToString =  strdup(_ser.serialize(const_cast<TMS_Data::JobResult_ptr>(&jobResult)).c_str());

  if (diet_string_set(diet_parameter(profile,2), jobResultToString, DIET_VOLATILE)) {
    msgErrorDiet += "with the job result parameter "+std::string(jobResultToString);
    raiseDietMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);
  diet_container_set(diet_parameter(profile,5), DIET_PERSISTENT_RETURN);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,3), &jobResultInString, NULL)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }

    if(diet_string_get(diet_parameter(profile,4), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }

     /*To raise a vishnu exception if the receiving message is not empty*/
     TMSUtils::raiseTMSExceptionIfNotEmptyMsg(errorInfo);

    //To parse JobResult object serialized
    if (!vishnu::parseTMSEmfObject(std::string(jobResultInString), outJobResult, "Error when receiving JobResult object serialized")) {
      throw UserException(ERRCODE_INVALID_PARAM);
    }

    IDContainer = (profile->parameters[5]).desc.id;
    dagda_get_container(IDContainer);
    dagda_get_container_elements(IDContainer, &content);

    if (content.size == 2) {
      //To get all files from the container
      dagda_get_file(content.elt_ids[0],&outputPath);
      dagda_get_file(content.elt_ids[1],&errorPath);

      std::string err =  outJobResult->getErrorPath();
      std::string out = outJobResult->getOutputPath();
      vishnu::copyDagdaFile(std::string(outputPath), out);
      vishnu::copyDagdaFile(std::string(errorPath), err);
    }

  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*if (content.size == 2) {
    for (unsigned int i = 0; i < content.size; i++) {
      dagda_delete_data(content.elt_ids[i]);
    }
  }*/

  dagda_delete_data(IDContainer);
  diet_profile_free(profile);
  return outJobResult;
}

/**
* \brief Function to get the results of all job submitted
* \return The list of the job results
*/
//TODO: faire un try catch Vishnu exception pour nettoyer l'id Dadga dans le cache puis throw e;
TMS_Data::ListJobResults_ptr
JobOutPutProxy::getAllJobsOutPut() {
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* listJobResultInString = NULL;
  char* errorInfo = NULL;
  TMS_Data::ListJobResults_ptr listJobResults_ptr = NULL;

  char* IDContainer = NULL;
  diet_container_t content;

  std::string serviceName = "jobOutPutGetAllResult_";
  serviceName.append(mmachineId);

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 4);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
   //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  if (diet_string_set(diet_parameter(profile,1), strdup(mmachineId.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with machineId parameter "+mmachineId;
    raiseDietMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
  diet_container_set(diet_parameter(profile,4), DIET_PERSISTENT_RETURN);

  if(!diet_call(profile)) {

    if(diet_string_get(diet_parameter(profile,2), &listJobResultInString, NULL)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }

    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }

    /*To raise a vishnu exception if the receiving message is not empty*/
    TMSUtils::raiseTMSExceptionIfNotEmptyMsg(errorInfo);

    IDContainer = (profile->parameters[4]).desc.id;
    dagda_get_container(IDContainer);
    dagda_get_container_elements(IDContainer, &content);

    try {
      if (content.size > 1) {
        //To get all files from the container
        for(unsigned int i = 0; i < content.size; i++) {
          char* path = NULL;
          dagda_get_file(content.elt_ids[i],&path);
          bfs::path filePath(path);
          //For moving dagda files from /tmp to the tmp directory of the current directory
          bfs::rename(filePath, bfs::path(bfs::current_path().string() / filePath.filename()));
        }
      }
    } //To catch boost exception and change it to UserException
    catch(std::exception& e) {
      /*if (content.size > 1) {
        for(unsigned int i = 0; i < content.size; i++) {
          dagda_delete_data(content.elt_ids[i]);
        }
      }*/
      dagda_delete_data(IDContainer);
      diet_profile_free(profile);
      throw UserException(ERRCODE_INVALID_PARAM, e.what());
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  dagda_delete_data(IDContainer);
  //To parse ListJobResult object serialized
  if (!vishnu::parseTMSEmfObject(std::string(listJobResultInString), listJobResults_ptr, "Error when receiving ListJobResult object serialized")) {
    throw UserException(ERRCODE_INVALID_PARAM);
  }

  /*if (content.size > 1) {
    for(unsigned int i = 0; i < content.size; i++) {
      dagda_delete_data(content.elt_ids[i]);
    }
  }*/
  dagda_delete_data(IDContainer);
  diet_profile_free(profile);
  return listJobResults_ptr;
}

/**
* \brief Destructor
*/

JobOutPutProxy::~JobOutPutProxy() {
}
