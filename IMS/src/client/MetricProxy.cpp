#include "MetricProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"

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
  std::string serviceName = "int_setUpdateFrequency";
  string msgErrorDiet;

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }
  if (diet_string_set(diet_parameter(profile,1), convertToString(freq).c_str())) {
    msgErrorDiet += "with frequency parameter "+freq;
    raiseDietMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL);
  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), errorInfo)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);
}

int
MetricProxy::getUpFreq() {
  diet_profile_t* profile = NULL;
  string sessionKey;
  string errorInfo;
  std::string serviceName = "int_getUpdateFrequency";
  string str;
  string msgErrorDiet;

  profile = diet_profile_alloc(serviceName.c_str(), 0, 0, 2);
  sessionKey = msessionProxy.getSessionKey();
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

   //OUT Parameters
  diet_string_set(diet_parameter(profile,1), NULL);
  diet_string_set(diet_parameter(profile,2), NULL);
  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,1), str)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
    if(diet_string_get(diet_parameter(profile,2), errorInfo)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("DIET call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);
  return convertToInt(str);
}
