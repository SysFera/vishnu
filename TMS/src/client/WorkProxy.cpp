/**
 * \file WorkProxy.cpp
 * \brief This file contains the VISHNU WorkProxy class.
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 * \date February 2011
 */
#include <boost/scoped_ptr.hpp>
#include "TMSVishnuException.hpp"
#include "WorkProxy.hpp"
#include "utilsClient.hpp"
#include "TMS_Data.hpp"
#include "TMSServices.hpp"
#include "DIET_client.h"
#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc

/**
 * \param work The object which encapsulates the information of the work
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */
WorkProxy::WorkProxy(const TMS_Data::Work& work, const SessionProxy& session):
  mwork(work), msessionProxy(session)
{
}

/**
 * \brief Function to add a new work
 * \return raises an exception on error
 */
int
WorkProxy::add(const TMS_Data::AddWorkOptions& addop) {

  std::string sessionKey;
  std::string workToString;
  std::string workInString;
  std::string errorInfo;

  std::string msg = "call of function diet_string_set is rejected ";

  diet_profile_t* addProfile = diet_profile_alloc(SERVICES_TMS[ADDWORK], 3);
  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the work object in to workToString
  workToString = _ser.serialize_str(const_cast<TMS_Data::Work_ptr>(&mwork));

  ::ecorecpp::serializer::serializer _ser2;
  //To serialize the options object in to optionsInString
  std::string optionsToString =
      _ser2.serialize_str(const_cast<TMS_Data::AddWorkOptions_ptr>(&addop));
  //tmp = _ser2.serialize_str(const_cast<TMS_Data::AddWorkOptions_ptr>((TMS_Data::AddWorkOptions*)(&addop)));

  //IN Parameters
  if (diet_string_set(addProfile,0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msg);
  }
  if (diet_string_set(addProfile,1, workToString)) {
    msg += "with workToString parameter "+workToString;
    raiseCommunicationMsgException(msg);
  }
  if (diet_string_set(addProfile,2, optionsToString)) {
    msg += "with workToString parameter "+workToString;
    raiseCommunicationMsgException(msg);
  }

  if (!diet_call(addProfile)) {
    if (diet_string_get(addProfile,0, workInString)){
      msg += "by receiving Work serialized  message";
      raiseCommunicationMsgException(msg);
    }
    if (diet_string_get(addProfile,1, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseCommunicationMsgException(msg);
    }
  }
  else {
    raiseCommunicationMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  TMS_Data::Work_ptr work_ptr;

  //To parse work object serialized
  parseEmfObject(workInString, work_ptr, "Error by receiving Work object serialized");

  mwork = *work_ptr;
  delete work_ptr;

  diet_profile_free(addProfile);

  return 0;
}

/**
 * \brief Function to update work description
 * \return raises an exception on error
 */
int
WorkProxy::update() {
  diet_profile_t* updateProfile = NULL;
  std::string sessionKey;
  std::string workToString;
  std::string errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  updateProfile = diet_profile_alloc(SERVICES_TMS[WORKUPDATE], 2);
  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the work object in to workToString
  workToString =  _ser.serialize_str(const_cast<TMS_Data::Work_ptr>(&mwork));

  //IN Parameters
  if (diet_string_set(updateProfile,0, sessionKey)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msg);
  }
  if (diet_string_set(updateProfile,1, workToString)) {
    msg += "with workToString parameter "+workToString;
    raiseCommunicationMsgException(msg);
  }

  if (!diet_call(updateProfile)) {
    if (diet_string_get(updateProfile,0, errorInfo)){
      msg += "by receiving errorInfo message";
      raiseCommunicationMsgException(msg);
    }
  } else {
    raiseCommunicationMsgException("VISHNU call failure");
  }
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(updateProfile);

  return 0;
}

/**
 * \brief Function to remove a work
 * \return raises an exception on error
 */
int
WorkProxy::deleteWork() {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_TMS[WORKDELETE], 2);
  std::string sessionKey = msessionProxy.getSessionKey();
  std::string workId = mwork.getWorkId();

  // Set parameters
  diet_string_set(profile,0, sessionKey);
  diet_string_set(profile,1, workId);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy
WorkProxy::getSessionProxy() {
  return msessionProxy;
}

/**
 * \brief Function get work information
 * \return Work object encapsulates the information of the work
 * \return raises an exception on error
 */
TMS_Data::Work
WorkProxy::getData() {
  return mwork;
}

/**
 * \brief Destructor, raises an exception on error
 */
WorkProxy::~WorkProxy() {
}
