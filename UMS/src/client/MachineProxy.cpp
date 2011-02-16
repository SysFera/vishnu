#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UMSVishnuException.hh"
#include "MachineProxy.hpp"
#include "utilsClient.hpp"


MachineProxy::MachineProxy(const UMS_Data::Machine& machine, const SessionProxy& session):
mmachine(machine), msessionProxy(session)
{
}
 
int MachineProxy::_addMachineInformation(bool isNewMachine)
{

   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string machineToString;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   if(isNewMachine) profile = diet_profile_alloc("machineCreate", 1, 1, 2);
   else profile = diet_profile_alloc("machineUpdate", 1, 1, 2);

   sessionKey = msessionProxy.getSessionKey();

   const char* name = "addMachineInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   machineToString =  _ser.serialize(const_cast<UMS_Data::Machine_ptr>(&mmachine));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     ERRMSG(msg.c_str());
     sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(machineToString.c_str()), DIET_VOLATILE)) {
     msg += "with machineToString parameter "+machineToString;
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

int MachineProxy::add()
{
  return _addMachineInformation();
}
 
int MachineProxy::update()
{
  return _addMachineInformation(false);
}
 
int MachineProxy::deleteMachine()
{
   diet_profile_t* profile = NULL;
   std::string sessionKey;
   std::string machineId;
   char* errorInfo;
   std::string msg = "call of function diet_string_set is rejected ";

   profile = diet_profile_alloc("machineDelete", 1, 1, 2);
   sessionKey = msessionProxy.getSessionKey();
   machineId = mmachine.getMachineId();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
      msg += "with sessionKey parameter "+sessionKey;
      ERRMSG(msg.c_str());
      sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(machineId.c_str()), DIET_VOLATILE)) {
      msg += "with machineId parameter "+machineId;
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

SessionProxy MachineProxy::getSessionProxy()
{
  return msessionProxy;
}

UMS_Data::Machine MachineProxy::getData()
{
  return mmachine;
}
 
MachineProxy::~MachineProxy()
{
}
 
