/**
 * \file WorkProxy.cpp
 * \brief This file contains the VISHNU WorkProxy class.
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 * \date February 2011
 */
#include "TMSVishnuException.hpp"
#include "WorkProxy.hpp"
#include "utilsClient.hpp"

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
int WorkProxy::add()
{

  diet_profile_t* addProfile = NULL;
  std::string sessionKey;
  std::string workToString;
  char* workInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  addProfile = diet_profile_alloc("addWork", 1, 1, 3);
  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the work object in to workToString
  workToString =  _ser.serialize_str(const_cast<TMS_Data::Work_ptr>(&mwork));

  //IN Parameters
  if(diet_string_set(diet_parameter(addProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(addProfile,1), strdup(workToString.c_str()), DIET_VOLATILE)) {
    msg += "with workToString parameter "+workToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(addProfile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(addProfile,3), NULL, DIET_VOLATILE);

  if(!diet_call(addProfile)) {
    if(diet_string_get(diet_parameter(addProfile,2), &workInString, NULL)){
      msg += "by receiving Work serialized  message";
      raiseDietMsgException(msg);
    }
    if(diet_string_get(diet_parameter(addProfile,3), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  TMS_Data::Work_ptr work_ptr;

  //To parse work object serialized
  parseEmfObject(std::string(workInString), work_ptr, "Error by receiving Work object serialized");

  mwork = *work_ptr;
  delete work_ptr;

  diet_profile_free(addProfile);

  return 0;

}

/**
 * \brief Function to update work description
 * \return raises an exception on error
 */
int WorkProxy::update()
{
  diet_profile_t* updateProfile = NULL;
  std::string sessionKey;
  std::string workToString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  updateProfile = diet_profile_alloc("workUpdate", 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the work object in to workToString
  workToString =  _ser.serialize_str(const_cast<TMS_Data::Work_ptr>(&mwork));

  //IN Parameters
  if(diet_string_set(diet_parameter(updateProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(updateProfile,1), strdup(workToString.c_str()), DIET_VOLATILE)) {
    msg += "with workToString parameter "+workToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(updateProfile,2), NULL, DIET_VOLATILE);

  if(!diet_call(updateProfile)) {
    if(diet_string_get(diet_parameter(updateProfile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(updateProfile);

  return 0;
}

/**
 * \brief Function to remove a work
 * \return raises an exception on error
 */
int WorkProxy::deleteWork()
{
  diet_profile_t* deleteProfile = NULL;
  std::string sessionKey;
  std::string workId;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  deleteProfile = diet_profile_alloc("workDelete", 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();
  workId = mwork.getWorkId();

  //IN Parameters
  if(diet_string_set(diet_parameter(deleteProfile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(deleteProfile,1), strdup(workId.c_str()), DIET_VOLATILE)) {
    msg += "with workId parameter "+workId;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(deleteProfile,2), NULL, DIET_VOLATILE);

  if(!diet_call(deleteProfile)) {
    if(diet_string_get(diet_parameter(deleteProfile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(deleteProfile);

  return 0;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy WorkProxy::getSessionProxy()
{
  return msessionProxy;
}

/**
 * \brief Function get work information
 * \return Work object encapsulates the information of the work
 * \return raises an exception on error
 */
TMS_Data::Work WorkProxy::getData()
{
  return mwork;
}

/**
 * \fn ~WorkProxy()
 * \brief Destructor, raises an exception on error
 */
WorkProxy::~WorkProxy()
{
}

