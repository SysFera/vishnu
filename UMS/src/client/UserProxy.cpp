/**
 * \file UserProxy.cpp
 * \brief This file contains the VISHNU UserProxy class.
 * \authors Daouda Traore (daouda.traore@sysfera.com)
 */
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

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
 * \fn explicit explicit UserProxy(SessionProxy session) 
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
 * \brief Function to combine add() and update() into one function 
 * \fn  int _addUserInformation(const UMS_Data::User& user, bool isNewUser=true)
 * \param user The object which encapsulates the user information 
 * \param isNewUser to select the call of add or update function 
 * \return raises an exception on error
 */ 
int UserProxy::_addUserInformation(const UMS_Data::User& user, bool isNewUser)
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string userToString;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";  

   if(isNewUser) profile = diet_profile_alloc("userCreate", 1, 1, 2);
   else profile = diet_profile_alloc("userUpdate", 1, 1, 2);
   sessionKey = msessionProxy->getSessionKey();

   const char* name = "addUserInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   //To serialize the user object in to userToString 
   userToString =  _ser.serialize(const_cast<UMS_Data::User_ptr>(&user));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     errMsg(msg);
     sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(userToString.c_str()), DIET_VOLATILE)) {
     msg += "with userToString parameter "+userToString;
     errMsg(msg);
     sendErrorMsg(msg); 
   }
   
   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
          msg += " by receiving errorInfo message";
          errMsg(msg);
          sendErrorMsg(msg); 
       }
       //Print successfull message if erroInfo is empty
       printSuccessMessage(errorInfo);
   }  
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }
  
   /*To check the receiving message error*/ 
    checkErrorMsg(errorInfo);

   return 0;
}

/**
 * \brief Function to add new user 
 * \fn  int add(const UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */
int UserProxy::add(const UMS_Data::User& user) {
  return _addUserInformation(user); 
}

/**
 * \brief Function to update user information 
 * \fn  int update(const UMS_Data::User& user)
 * \param user The object which encapsulates the user information
 * \return raises an exception on error
 */ 
int UserProxy::update(const UMS_Data::User& user)
{
   return _addUserInformation(user, false);
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
     sendErrorMsg(msg);   
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(userId.c_str()), DIET_VOLATILE)) {
     msg += "with userId parameter "+userId;
     errMsg(msg);
     sendErrorMsg(msg); 
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
      if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
         msg += " by receiving errorInfo message";
         errMsg(msg);
         sendErrorMsg(msg);
      }
      //Print successfull message if erroInfo is empty
      printSuccessMessage(errorInfo);
   }
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }
     /*To check the receiving message error*/
     checkErrorMsg(errorInfo);
   
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
   if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy->getSessionKey()).c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
      errMsg(msg);
      sendErrorMsg(msg); 
   }

   if(diet_string_set(diet_parameter(profile,1), strdup(password.c_str()), DIET_VOLATILE)) {
     msg += "with password parameter "+password;
     errMsg(msg);
     sendErrorMsg(msg); 
   }

   if(diet_string_set(diet_parameter(profile,2), strdup(newPassword.c_str()), DIET_VOLATILE)) {
     msg += "with newPassword parameter "+newPassword;
     errMsg(msg);
     sendErrorMsg(msg); 
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
          msg += " by receiving errorInfo message";
          errMsg(msg);
          sendErrorMsg(msg);   
       }
       //Print successfull message if erroInfo is empty
       printSuccessMessage(errorInfo);
   }
   else {
      sendErrorMsg(" the function diet_call is rejected"); 
   }
     /*To check the receiving message error*/   
     checkErrorMsg(errorInfo);

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
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("userPasswordReset", 1, 1, 2);

   //IN Parameters  
   if(diet_string_set(diet_parameter(profile,0), strdup((msessionProxy->getSessionKey()).c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+msessionProxy->getSessionKey();
      errMsg(msg);
      sendErrorMsg(msg); 
   }

   if(diet_string_set(diet_parameter(profile,1), strdup((user.getUserId()).c_str()), DIET_VOLATILE)) {
      msg += "with userId parameter "+user.getUserId();
      errMsg(msg);
      sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
          msg += " by receiving errorInfo message";
          errMsg(msg);
          sendErrorMsg(msg);     
       }
       //Print successfull message if erroInfo is empty
       printSuccessMessage(errorInfo);
   }
   else {
      sendErrorMsg(" the function diet_call is rejected"); 
   }
    /*To check the receiving message error*/
    checkErrorMsg(errorInfo);

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
