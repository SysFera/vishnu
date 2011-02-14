#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UMSVishnuException.hh"
#include "utilsClient.hpp"
#include "LocalAccountProxy.h"


LocalAccountProxy::LocalAccountProxy(const UMS_Data::LocalAccount& localAccount, const SessionProxy& session):
mlocalAccount(localAccount), msessionProxy(session)
{
}

int LocalAccountProxy::_addLocalAccountInformation(bool isNewLocalAccount) {

   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string localAccountToString;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   if(isNewLocalAccount) profile = diet_profile_alloc("localAccountCreate", 1, 1, 2);
   else profile = diet_profile_alloc("localAccountUpdate", 1, 1, 2);
              
   sessionKey = msessionProxy.getSessionKey();

   const char* name = "addLocalAccountInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   localAccountToString =  _ser.serialize(const_cast<UMS_Data::LocalAccount_ptr>(&mlocalAccount));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(localAccountToString.c_str()), DIET_VOLATILE)) {
     msg += "with localAccountToString parameter "+localAccountToString;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
         msg += " by receiving errorInfo message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg);
       }
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }

   /*To check the receiving message error*/
    checkErrorMsg(errorInfo);
 
  return 0;
}
 
int LocalAccountProxy::add()
{
  return _addLocalAccountInformation();
}
 
int LocalAccountProxy::update()
{
  return _addLocalAccountInformation(false);
}
 
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
     ERRMSG(msg.c_str());
     sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(userId.c_str()), DIET_VOLATILE)) {
     msg += "with userId parameter "+userId;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg);
   }
   if(diet_string_set(diet_parameter(profile,2), strdup(machineId.c_str()), DIET_VOLATILE)) {
      msg += "with machineId parameter "+machineId;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg);
   }

   //OUT Parameters
   diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
         msg += " by receiving errorInfo message";
         ERRMSG(msg.c_str());
         sendErrorMsg(msg); 
       }
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
     sendErrorMsg(" the function diet_call is rejected");
   }

   /*To check the receiving message error*/
    checkErrorMsg(errorInfo);

  return 0;
}
 
SessionProxy LocalAccountProxy::getSessionProxy()
{
  return msessionProxy;
}

LocalAccountProxy::~LocalAccountProxy()
{
}
