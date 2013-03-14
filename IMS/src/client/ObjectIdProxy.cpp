#include "ObjectIdProxy.hpp"
#include "UserException.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "IMSServices.hpp"
#include "DIET_client.h"

using namespace vishnu;
using namespace std;


ObjectIdProxy::ObjectIdProxy(const SessionProxy& session): msessionProxy(session) {
}

ObjectIdProxy::~ObjectIdProxy() {
}

void
ObjectIdProxy::setUID(string fmt) {
  try {
    defineCall(SERVICES_IMS[INT_DEFINEUSERIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad user format");
    throw (e);
  }
}

void
ObjectIdProxy::setMID(string fmt) {
  try {
    defineCall(SERVICES_IMS[INT_DEFINEMACHINEIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad machine format");
    throw (e);
  }
}

void
ObjectIdProxy::setTID(string fmt) {
  try {
    defineCall (SERVICES_IMS[INT_DEFINEJOBIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad task format");
    throw (e);
  }
}

void
ObjectIdProxy::setFID(string fmt) {
  try {
    defineCall (SERVICES_IMS[INT_DEFINETRANSFERIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad file transfert format");
    throw (e);
  }
}

void
ObjectIdProxy::setAID(string fmt) {
  try {
    defineCall (SERVICES_IMS[INT_DEFINEAUTHIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad authentication format");
    throw (e);
  }
}

void
ObjectIdProxy::setWID(string fmt) {
  try {
    defineCall (SERVICES_IMS[INT_DEFINEWORKIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad authentication format");
    throw (e);
  }
}

void
ObjectIdProxy::defineCall(string name, string fmt) {
  diet_profile_t* profile = NULL;
  string sessionKey;
  string errorInfo;
  string serviceName = name;
  string msgErrorDiet;

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }
  if (diet_string_set(profile,1, fmt.c_str())) {
    msgErrorDiet += "with format parameter "+fmt;
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(profile,2);
  if(!diet_call(profile)) {
    if(diet_string_get(profile,2, errorInfo)){
      msgErrorDiet += " by receiving User serialized  message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }
  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);
}
