/**
 * \file MachineProxy.cpp
 * \brief This file contains the VISHNU MachineProxy class.
 * \authors Daouda Traore (daouda.traore@sysfera.com)
 */
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "UMSVishnuException.hh"
#include "MachineProxy.hpp"
#include "utilsClient.hpp"

/**
 * \fn MachineProxy(const UMS_Data::Machine& machine,
 *                       const SessionProxy& session)
 * \param machine The object which encapsulates the information of the machine 
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 * \brief Constructor, raises an exception on error
 */
MachineProxy::MachineProxy(const UMS_Data::Machine& machine, const SessionProxy& session):
mmachine(machine), msessionProxy(session)
{
}

/**
 * \brief Function to combine add() and update() into one function 
 * \fn  int _addMachineInformation(bool isNewMachine=true); 
 * \param isNewMachine to select the call of add or update function 
 * \return raises an exception on error
 */ 
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
   //To serialize the mmachine object in to machineToString 
   machineToString =  _ser.serialize(const_cast<UMS_Data::Machine_ptr>(&mmachine));

   //IN Parameters
   if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
     msg += "with sessionKey parameter "+sessionKey;
     errMsg(msg);
     sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(machineToString.c_str()), DIET_VOLATILE)) {
     msg += "with machineToString parameter "+machineToString;
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
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
   }
   else {
      sendErrorMsg(" the function diet_call is rejected");
   }

    /*To check the receiving message error*/
    checkErrorMsg(errorInfo);

  return 0;
}

/**
 * \brief Function to add a new machine 
 * \fn  int add()
 * \return raises an exception on error
 */
int MachineProxy::add()
{
  return _addMachineInformation();
}

/**
 * \brief Function to update machine description 
 * \fn  int update()
 * \return raises an exception on error
 */ 
int MachineProxy::update()
{
  return _addMachineInformation(false);
}

/**
 * \brief Function to remove a machine 
 * \fn  int deleteMachine()
 * \return raises an exception on error
 */ 
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
      errMsg(msg);
      sendErrorMsg(msg); 
   }
   if(diet_string_set(diet_parameter(profile,1), strdup(machineId.c_str()), DIET_VOLATILE)) {
      msg += "with machineId parameter "+machineId;
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
       if(strlen(errorInfo)==0) std::cout << "The service was performed successfull" << std::endl;
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
SessionProxy MachineProxy::getSessionProxy()
{
  return msessionProxy;
}

/**
 * \brief Function get machine information 
 * \fn  UMS_Data::Machine getData()
 * \return Machine object encapsulates the information of the machine 
 * \return raises an exception on error
 */
UMS_Data::Machine MachineProxy::getData()
{
  return mmachine;
}

/**
 * \fn ~MachineProxy()
 * \brief Destructor, raises an exception on error
 */ 
MachineProxy::~MachineProxy()
{
}
 
