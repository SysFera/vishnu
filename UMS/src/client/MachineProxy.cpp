/**
 * \file MachineProxy.cpp
 * \brief This file contains the VISHNU MachineProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */

#include "MachineProxy.hpp"

#include <string>                       // for string, allocator, etc

#include "DIET_client.h"                // for diet_string_set, etc
#include "UserException.hpp"            // for ERRCODE_INVALID_PARAM
#include "ecorecpp/serializer/serializer.hpp"  // for serializer
#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc

#include "UMSVishnuException.hpp"
#include "UMSServices.hpp"

/**
 * \brief Constructor, raises an exception on error
 * \param machine The object which encapsulates the information of the machine
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 */
MachineProxy::MachineProxy(const UMS_Data::Machine& machine,
                           const SessionProxy& session):
  mmachine(machine), msessionProxy(session) {
}

/**
 * \brief Function to add a new machine
 * \return raises an exception on error
 */
int
MachineProxy::add() {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[MACHINECREATE], 2);

  ::ecorecpp::serializer::serializer _ser;
  std::string machineToString =  _ser.serialize_str(const_cast<UMS_Data::Machine_ptr>(&mmachine));

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy.getSessionKey());
  diet_string_set(profile, 1, machineToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  std::string machineInString;
  diet_string_get(profile,1, machineInString);

  // parse serialized machine object
  UMS_Data::Machine_ptr machine_ptr;
  parseEmfObject(machineInString, machine_ptr, "Error by receiving Machine object serialized");
  mmachine = *machine_ptr;
  delete machine_ptr;

  diet_profile_free(profile);

  return 0;

}

/**
 * \brief Function to update machine description
 * \return raises an exception on error
 */
int
MachineProxy::update() {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[MACHINEUPDATE], 2);

  // serialize the mmachine object in to machineToString
  ::ecorecpp::serializer::serializer _ser;
  std::string machineToString =  _ser.serialize_str(const_cast<UMS_Data::Machine_ptr>(&mmachine));

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy.getSessionKey());
  diet_string_set(profile, 1, machineToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to remove a machine
 * \return raises an exception on error
 */
int
MachineProxy::deleteMachine() {

  diet_profile_t* profile = diet_profile_alloc(SERVICES_UMS[MACHINEDELETE], 2);

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy.getSessionKey());
  diet_string_set(profile, 1, mmachine.getMachineId());

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
MachineProxy::getSessionProxy() {
  return msessionProxy;
}

/**
 * \brief Function get machine information
 * \return Machine object encapsulates the information of the machine
 * \return raises an exception on error
 */
UMS_Data::Machine
MachineProxy::getData() {
  return mmachine;
}

/**
 * \brief Destructor, raises an exception on error
 */
MachineProxy::~MachineProxy() {
}
