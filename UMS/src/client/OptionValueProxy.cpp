/**
 * \file OptionValueProxy.cpp
 * \brief This file contains the VISHNU OptionValueProxy class.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "OptionValueProxy.hpp"

#include <string>                       // for allocator, string, etc
#include "DIET_client.h"                // for diet_string_set, etc
#include "ecorecpp/serializer/serializer.hpp"  // for serializer
#include "utilClient.hpp"               // for raiseCommunicationMsgException, etc
#include "UMSServices.hpp"

/**
 * \brief Constructor, raises an exception on error
 * \param optionValue The object which encapsulates the information of the option
 * \param session The object which encapsulates the session information (ex: identifier of the session)
 */
OptionValueProxy::OptionValueProxy(const UMS_Data::OptionValue& optionValue, const SessionProxy& session):
  moptionValue(optionValue), msessionProxy(session)
{
}

/**
 * \brief Function to combine configureOption() and configureDefaultOption() into one function
 * \param defaultValue to select the call of configureOption() or configureOption() function
 * \return raises an exception on error
 */
int
OptionValueProxy::setOptionValue(bool defaultValue) {

  diet_profile_t* profile = NULL;
  if (defaultValue) {
    profile = diet_profile_alloc(SERVICES_UMS[OPTIONVALUESETDEFAULT], 2);
  } else {
    profile = diet_profile_alloc(SERVICES_UMS[OPTIONVALUESET], 2);
  }

  //To serialize the moptionValue object in to optionValueToString
  ::ecorecpp::serializer::serializer _ser;
  std::string optionValueToString =  _ser.serialize_str(const_cast<UMS_Data::OptionValue_ptr>(&moptionValue));

  //IN Parameters
  diet_string_set(profile, 0, msessionProxy.getSessionKey());
  diet_string_set(profile, 1, optionValueToString);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }
  raiseExceptionOnErrorResult(profile);

  diet_profile_free(profile);

  return 0;
}

/**
 * \brief Function to configure an option of the user
 * \return raises an exception on error
 */
int
OptionValueProxy::configureOption() {
  return setOptionValue(false);
}

/**
 * \brief Function to configure a default option value
 * \return raises an exception on error
 */
int
OptionValueProxy::configureDefaultOption() {
  return setOptionValue();
}

/**
 * \brief Function get option information
 * \return OptionValue object encapsulates the information of the machine
 * \return raises an exception on error
 */
UMS_Data::OptionValue
OptionValueProxy::getData() {
  return moptionValue;
}

/**
 * \brief Function get SessionProxy object which contains the VISHNU session identifier
 * \return a SessionProy object which contains the VISHNU session information
 * \return raises an exception on error
 */
SessionProxy
OptionValueProxy::getSessionProxy() {
  return msessionProxy;
}

/**
 * \brief Destructor, raises an exception on error
 */
OptionValueProxy::~OptionValueProxy()
{
}
