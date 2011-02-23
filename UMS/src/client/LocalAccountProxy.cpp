/**
 * \file LocalAccountProxy.cpp
 * \brief This file contains the VISHNU LocalAccountProxy class.
 * \authors Daouda Traore (daouda.traore@sysfera.com)
 */
#include <string>
#include <vector>
#include <list>
#include <iostream>

#include "UMSVishnuException.hpp"
#include "utilsClient.hpp"
#include "LocalAccountProxy.hpp"

/**
 * \fn LocalAccountProxy(const UMS_Data::LocalAccount& localAccount,
 *                       const SessionProxy& session)
 * \param localAccount The object which encapsulates the user Acccount information 
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */
LocalAccountProxy::LocalAccountProxy(const UMS_Data::LocalAccount& localAccount, const SessionProxy& session):
mlocalAccount(localAccount), msessionProxy(session)
{
 msshPublicKey = "";
}

/**
 * \brief Function to combine add() and update() into one function 
 * \fn  int _addLocalAccountInformation(bool isNewMachine=true); 
 * \param isNewLocalAccount to select the call of add or update function 
 * \return raises an exception on error
 */
int LocalAccountProxy::_addLocalAccountInformation(bool isNewLocalAccount) {

   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string localAccountToString;
   char* sshPublicKey;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   if(isNewLocalAccount) { 
     profile = diet_profile_alloc("localAccountCreate", 1, 1, 3);
   }
   else {
     profile = diet_profile_alloc("localAccountUpdate", 1, 1, 2);
   }
              
   sessionKey = msessionProxy.getSessionKey();

   const char* name = "addLocalAccountInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   //To serialize the mlocalAccount object in to localAccountToString 
   localAccountToString =  _ser.serialize(const_cast<UMS_Data::LocalAccount_ptr>(&mlocalAccount));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     errMsg(msg);
     sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(localAccountToString.c_str()), DIET_VOLATILE)) {
     msg += "with localAccountToString parameter "+localAccountToString;
     errMsg(msg);
     sendErrorMsg(msg); 
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
   if(isNewLocalAccount) {
     diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);
   }

   if(!diet_call(profile)) {
       if(isNewLocalAccount) {
         if(diet_string_get(diet_parameter(profile,2), &sshPublicKey, NULL)){
           msg += " by receiving sshPluciKey content";
           errMsg(msg);
           sendErrorMsg(msg);
         }
         if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
           msg += " by receiving errorInfo message";
           errMsg(msg);
           sendErrorMsg(msg);
         }
      }
      else {
         if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
           msg += " by receiving errorInfo message";
           errMsg(msg);
           sendErrorMsg(msg);
         }
      }
      msshPublicKey = sshPublicKey;
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
 * \brief Function to add a new local user configuration 
 * \fn  int add()
 * \return raises an exception on error
 */
int LocalAccountProxy::add()
{
  return _addLocalAccountInformation();
}

/**
 * \brief Function to update a new local user configuration 
 * \fn  int update()
 * \return raises an exception on error
 */ 
int LocalAccountProxy::update()
{
  return _addLocalAccountInformation(false);
}

/**
 * \brief Function to removes a local user configuration (for a given user on a given machine) from VISHNU 
 * \fn  int deleteLocalAccount()
 * \return raises an exception on error
 */ 
int LocalAccountProxy::deleteLocalAccount()
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string userId;
   std::string machineId;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("localAccountDelete", 2, 2, 3);
   sessionKey = msessionProxy.getSessionKey();
   userId = mlocalAccount.getUserId();
   machineId = mlocalAccount.getMachineId();

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
   if(diet_string_set(diet_parameter(profile,2), strdup(machineId.c_str()), DIET_VOLATILE)) {
      msg += "with machineId parameter "+machineId;
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
 * \brief Function get SessionProxy object which contains the VISHNU session identifier 
 * \fn SessionProxy getSessionProxy() 
 * \return a SessionProy object which contains the VISHNU session information 
 * \return raises an exception on error
 */ 
SessionProxy LocalAccountProxy::getSessionProxy() const
{
  return msessionProxy;
}

/**
 * \brief Function get the VISHNU ssh public key  
 * \fn std::string getSshPublicKey() const 
 * \return the VISHNU ssh public Key 
 * \return raises an exception on error
 */
std::string LocalAccountProxy::getSshPublicKey() const {
  return msshPublicKey;
}

/**
 * \fn ~LocalAccountProxy()
 * \brief Destructor, raises an exception on error
 */
LocalAccountProxy::~LocalAccountProxy()
{
}
