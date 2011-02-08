#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UMSVishnuException.hh"
#include "debug.hh"
#include "MachineProxy.h"


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

   if(isNewMachine) profile = diet_profile_alloc("machineCreate", 1, 1, 2);
   else profile = diet_profile_alloc("machineUpdate", 1, 1, 2);

   sessionKey = msessionProxy.getSessionKey();

   const char* name = "addMachineInformation";
   ::ecorecpp::serializer::serializer _ser(name);
   machineToString =  _ser.serialize(const_cast<UMS_Data::Machine_ptr>(&mmachine));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };
   if(diet_string_set(diet_parameter(profile,1), strdup(machineToString.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }

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

   profile = diet_profile_alloc("machineDelete", 1, 1, 2);
   sessionKey = msessionProxy.getSessionKey();
   machineId = mmachine.getMachineId();

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };
   if(diet_string_set(diet_parameter(profile,1), strdup(machineId.c_str()), DIET_VOLATILE)) {
       ERRMSG("Error in diet_string_set");
   };

   //OUT Parameters
   if(diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE)) {
      ERRMSG("Error in diet_string_set");
   }

   if(!diet_call(profile)) {
       if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
        ERRMSG("Error in diet_string_set");
       };
       if(strlen(errorInfo) > 0) std::cout << "errorInfo=" << errorInfo << std::endl;
       else std::cout << "The service was performed successfull" << std::endl;
   }
   else {
       ERRMSG("Error in diet_string_get");
   }

   if(strlen(errorInfo) > 0 ) {
      UMSVishnuException e(1, errorInfo);
      throw e;
   }

   return (strlen(errorInfo) > 0);

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
 
