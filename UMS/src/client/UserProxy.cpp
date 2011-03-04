/**
 * \file UserProxy.cpp
 * \brief This file contains the VISHNU UserProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UserProxy.hpp"
#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"

/**
 * \fn  UserProxy(const std::string& userId, const std::string& password) 
 * \param userId the user identifier
 * \password the user password 
 * \brief Constructor, raises an exception on error
 */
UserProxy::UserProxy(const  std::string& userId, const std::string& password)
{
  muser.setUserId(userId);
  muser.setPassword(password);
  msessionProxy = NULL;
}

/**
 * \fn explicit UserProxy(SessionProxy session) 
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */
UserProxy::UserProxy(SessionProxy session):
  msessionProxy(&session)
{
}

/**
 * \fn explicit UserProxy(const UMS_Data::User& user) 
 * \param user The object which encapsulates the user information 
 * \brief Constructor, raises an exception on error
 */
UserProxy::UserProxy(const UMS_Data::User& user):
  muser(user)
{
  msessionProxy = NULL;
}

/**
 * \brief Function to add new user 
 * \fn  int add(UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int UserProxy::add(UMS_Data::User& user)
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* userToString;
  char* errorInfo;
  char* userInString;
  std::string msg = "call of function diet_string_set is rejected ";  

  profile = diet_profile_alloc("userCreate", 1, 1, 3);
  sessionKey = msessionProxy->getSessionKey();

  const char* name = "add";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the user object in to userToString 
  userToString =  strdup(_ser.serialize(const_cast<UMS_Data::User_ptr>(&user)).c_str());
  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    errMsg(msg);
    raiseDietMsgException(msg); 
  }
  if(diet_string_set(diet_parameter(profile,1), userToString, DIET_VOLATILE)) {
    msg += "with userToString parameter "+std::string(userToString);
    errMsg(msg);
    raiseDietMsgException(msg); 
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &userInString, NULL)){
      msg += " by receiving User serialized  message";
      errMsg(msg);
      raiseDietMsgException(msg);
    }
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      errMsg(msg);
      raiseDietMsgException(msg); 
    }
  }  
  else {
    raiseDietMsgException(" the function diet_call is rejected");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/ 
  raiseExceptionIfNotEmptyMsg(errorInfo);

  // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  //Parse the model
  ecorecpp::parser::parser parser;
  //To set the muser
  UMS_Data::User_ptr user_ptr = parser.load(std::string(userInString))->as< UMS_Data::User >();

  user = *user_ptr;
  muser = user;

  return 0;
}

/**
 * \brief Function to update user information 
 * \fn  int update(const UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */ 
int UserProxy::update(const UMS_Data::User& user)
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string userToString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("userUpdate", 1, 1, 2);

  sessionKey = msessionProxy->getSessionKey();

  const char* name = "addUserInformation";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the user object in to userToString 
  userToString =  _ser.serialize(const_cast<UMS_Data::User_ptr>(&user));

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    errMsg(msg);
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(userToString.c_str()), DIET_VOLATILE)) {
    msg += "with userToString parameter "+userToString;
    errMsg(msg);
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      errMsg(msg);
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException(" the function diet_call is rejected");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  return 0;
}

/**
 * \brief Function to remove user information 
 * \fn  int deleteUser(const UMS_Data::User& user) 
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int UserProxy::deleteUser(const UMS_Data::User& user)
{

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string userId;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("userDelete", 1, 1, 2);
  sessionKey = msessionProxy->getSessionKey();
  userId = user.getUserId();

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    errMsg(msg);
    raiseDietMsgException(msg);   
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(userId.c_str()), DIET_VOLATILE)) {
    msg += "with userId parameter "+userId;
    errMsg(msg);
    raiseDietMsgException(msg); 
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      errMsg(msg);
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException(" the function diet_call is rejected");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  return 0;
}

/**
 * \brief Function to change user password 
 * \fn  int changePassword(const std::string& password, const std::string& newPassword) 
 * \param oldPassword the old password of the user
 * \param newPassword the new password of the user 
 * \return raises an exception on error
 */
int UserProxy::changePassword(const std::string& password, const std::string& newPassword)
{

  diet_profile_t* profile = NULL;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("userPasswordChange", 2, 2, 3);

  //IN Parameters  
  if(diet_string_set(diet_parameter(profile,0), strdup((muser.getUserId()).c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
    errMsg(msg);
    raiseDietMsgException(msg); 
  }

  if(diet_string_set(diet_parameter(profile,1), strdup(password.c_str()), DIET_VOLATILE)) {
    msg += "with password parameter "+password;
    errMsg(msg);
    raiseDietMsgException(msg); 
  }

  if(diet_string_set(diet_parameter(profile,2), strdup(newPassword.c_str()), DIET_VOLATILE)) {
    msg += "with newPassword parameter "+newPassword;
    errMsg(msg);
    raiseDietMsgException(msg); 
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      errMsg(msg);
      raiseDietMsgException(msg);   
    }
  }
  else {
    raiseDietMsgException(" the function diet_call is rejected"); 
  }
  /*To raise a vishnu exception if the receiving message is not empty*/   
  raiseExceptionIfNotEmptyMsg(errorInfo);

  return 0;
}

/**
 * \brief Function to reset user password 
 * \fn  int resetPassword(UMS_Data::User& user)
 * \param user The object which encapsulates the user information 
 * \return raises an exception on error
 */ 
int UserProxy::resetPassword(UMS_Data::User& user)
{

  diet_profile_t* profile = NULL;
  char* tmpPassword;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("userPasswordReset", 1, 1, 3);

  //IN Parameters  
  if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy->getSessionKey()).c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
    errMsg(msg);
    raiseDietMsgException(msg); 
  }

  if(diet_string_set(diet_parameter(profile,1), strdup((user.getUserId()).c_str()), DIET_VOLATILE)) {
    msg += "with userId parameter "+user.getUserId();
    errMsg(msg);
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &tmpPassword, NULL)){
      msg += " by receiving tmpPassWord message";
      errMsg(msg);
      raiseDietMsgException(msg);     
    }
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += " by receiving errorInfo message";
      errMsg(msg);
      raiseDietMsgException(msg);     
    }
  }
  else {
    raiseDietMsgException(" the function diet_call is rejected"); 
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  /*To set the temporary password*/
  muser.setUserId(user.getUserId());
  muser.setPassword(std::string(tmpPassword));

  return 0;
}

/**
 * \brief Function get user information 
 * \fn  UMS_Data::User getData()
 * \return User object encapsulates the information of the user 
 * \return raises an exception on error
 */ 
UMS_Data::User UserProxy::getData() const
{
  return muser;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier 
 * \fn SessionProxy getSessionProxy() 
 * \return a SessionProy object which contains the VISHNU session information 
 * \return raises an exception on error
 */ 
SessionProxy UserProxy::getSessionProxy() const
{
  return  *msessionProxy;
}

/**
 * \fn ~UserProxy()
 * \brief Destructor, raises an exception on error
 */
UserProxy::~UserProxy()
{
}
