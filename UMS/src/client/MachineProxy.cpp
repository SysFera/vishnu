/**
 * \file MachineProxy.cpp
 * \brief This file contains the VISHNU MachineProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "UMSVishnuException.hpp"
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
 * \brief Function to add a new machine 
 * \fn  int add()
 * \return raises an exception on error
 */
int MachineProxy::add()
{

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string machineToString;
  char* machineInString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("machineCreate", 1, 1, 3);
  sessionKey = msessionProxy.getSessionKey();

  const char* name = "addMachineInformation";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the mmachine object in to machineToString 
  machineToString =  _ser.serialize(const_cast<UMS_Data::Machine_ptr>(&mmachine));

  std::string sshKeyFilePath = mmachine.getSshPublicKey();
  if(sshKeyFilePath.find("\"")!=std::string::npos) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, " The machine SshPublicKey contains special character double quote \"");
  }
  if(sshKeyFilePath.find("<")!=std::string::npos) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, " The machine SshPublicKey contains special character < ");
  }
  if(sshKeyFilePath.find(">")!=std::string::npos) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, " The machine SshPublicKey contains special character > ");
  }

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg); 
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(machineToString.c_str()), DIET_VOLATILE)) {
    msg += "with machineToString parameter "+machineToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  diet_string_set(diet_parameter(profile,3), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &machineInString, NULL)){
      msg += "by receiving Machine serialized  message";
      raiseDietMsgException(msg);
    }
    if(diet_string_get(diet_parameter(profile,3), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  // CREATE DATA MODEL
  UMS_Data::UMS_DataPackage_ptr ecorePackage = UMS_Data::UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

  //Parse the model
  ecorecpp::parser::parser parser;
  //To set the mconfiguration 
  UMS_Data::Machine_ptr machine_ptr = parser.load(machineInString)->as< UMS_Data::Machine >();

  mmachine = *machine_ptr;

  return 0;

}

/**
 * \brief Function to update machine description 
 * \fn  int update()
 * \return raises an exception on error
 */ 
int MachineProxy::update()
{
  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string machineToString;
  char* errorInfo;
  std::string msg = "call of function diet_string_set is rejected ";

  profile = diet_profile_alloc("machineUpdate", 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();

  const char* name = "addMachineInformation";
  ::ecorecpp::serializer::serializer _ser(name);
  //To serialize the mmachine object in to machineToString 
  machineToString =  _ser.serialize(const_cast<UMS_Data::Machine_ptr>(&mmachine));

  //IN Parameters
  if(diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msg += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msg);
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(machineToString.c_str()), DIET_VOLATILE)) {
    msg += "with machineToString parameter "+machineToString;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  return 0;
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
    raiseDietMsgException(msg); 
  }
  if(diet_string_set(diet_parameter(profile,1), strdup(machineId.c_str()), DIET_VOLATILE)) {
    msg += "with machineId parameter "+machineId;
    raiseDietMsgException(msg);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msg += "by receiving errorInfo message";
      raiseDietMsgException(msg); 
    }
  }
  else {
    raiseDietMsgException("DIET call failure"); 
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

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

