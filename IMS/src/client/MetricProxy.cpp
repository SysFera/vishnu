#include "MetricProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "IMSServices.hpp"
#include "DIET_client.h"

using namespace std;
using namespace vishnu;

MetricProxy::MetricProxy(const SessionProxy& session): msessionProxy(session) {
}

MetricProxy::~MetricProxy() {
}

void
MetricProxy::setUpFreq(int freq) {
  diet_profile_t* profile = NULL;
  string sessionKey;
  string errorInfo;
  std::string serviceName = SERVICES_IMS[INT_SETUPDATEFREQUENCY];
  string msgErrorDiet;

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }
  if (diet_string_set(profile,1, convertToString(freq).c_str())) {
    msgErrorDiet += "with frequency parameter "+freq;
    raiseCommunicationMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(profile,2);
  if(!diet_call(profile)) {
    if(diet_string_get(profile,2, errorInfo)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseCommunicationMsgException(msgErrorDiet);
    }
  }
  else {
    raiseCommunicationMsgException("VISHNU call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);
}

int
MetricProxy::getUpFreq() {
  diet_profile_t* profile = NULL;
  string sessionKey;
  string errorInfo;
  std::string serviceName = SERVICES_IMS[INT_GETUPDATEFREQUENCY];
  string str;
  string msgErrorDiet;

  profile = diet_profile_alloc(serviceName.c_str(), 0, 0, 2);
  sessionKey = msessionProxy.getSessionKey();
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseCommunicationMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(profile,1);
  diet_string_set(profile,2);
  if(!diet_call(profile)) {
    if(diet_string_get(profile,1, str)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseCommunicationMsgException(msgErrorDiet);
    }
    if(diet_string_get(profile,2, errorInfo)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseCommunicationMsgException(msgErrorDiet);
    }
  }
  else {
    raiseCommunicationMsgException("VISHNU call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);
  return convertToInt(str);
}
