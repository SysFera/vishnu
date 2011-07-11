
#include <iostream>
#include <string>
#include "ThresholdProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "QueryProxy.hpp"

using namespace vishnu;

/**
* \param session The object which encapsulates the session information
* \brief Constructor
*/
ThresholdProxy::ThresholdProxy(const SessionProxy& session): msessionProxy(session) {
}


/**
* \brief Function to set a system threshold
* \param systemThreshold the system threshold
* \return raises an exception on error
*/
void
ThresholdProxy::setSystemThreshold(IMS_Data::Threshold systemThreshold) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  char* errorInfo = NULL;
  std::string objectToString;

  std::string serviceName = "int_setSystemThreshold";

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  objectToString =  strdup(_ser.serialize_str(const_cast<IMS_Data::Threshold_ptr>(&systemThreshold)).c_str());

  if (diet_string_set(diet_parameter(profile,1), strdup(objectToString.c_str()),  DIET_VOLATILE)) {
    msgErrorDiet += "with SystemInfo parameter ";
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);

  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);
}

/**
* \brief Function to get a system threshold
* \param options the options data structure to get system threshold
* \param listSysThreshold the list of system threshold returned
* \return raises an exception on error
*/
int
ThresholdProxy::getSystemThreshold(IMS_Data::ListThreshold& listSysThreshold,
                   const IMS_Data::ThresholdOp& options) {

  std::string name = "int_getSystemThreshold";
  QueryProxy<IMS_Data::ThresholdOp, IMS_Data::ListThreshold>
  query(options, msessionProxy, name);

  IMS_Data::ListThreshold_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
    for(unsigned int i = 0; i < li->getThreshold().size(); i++) {
      IMS_Data::Threshold_ptr threshold = ecoreFactory->createThreshold();
       //To copy the content and not the pointer
      *threshold = *li->getThreshold().get(i);
      listSysThreshold.getThreshold().push_back(threshold);
    }
    delete li;
  }
  return 0;
}

/**
* \brief Destructor
*/
ThresholdProxy::~ThresholdProxy() {

}
