#include "ObjectIdProxy.hpp"
#include "UserException.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
using namespace vishnu;

ObjectIdProxy::ObjectIdProxy(const SessionProxy& session): msessionProxy(session) {
}

ObjectIdProxy::~ObjectIdProxy() {
}

void
ObjectIdProxy::setUID(string fmt) {
  try {
    defineCall("int_defineUserIdentifier", fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad user format");
    throw (e);
  }
}

void
ObjectIdProxy::setMID(string fmt) {
  try {
    defineCall("int_defineMachineIdentifier", fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad machine format");
    throw (e);
  }
}

void
ObjectIdProxy::setTID(string fmt) {
  try {
    defineCall ("int_defineJobIdentifier", fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad task format");
    throw (e);
  }
}

void
ObjectIdProxy::setFID(string fmt) {
  try {
    defineCall ("int_defineTransferIdentifier", fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad file transfert format");
    throw (e);
  }
}

void
ObjectIdProxy::defineCall(string name, string fmt) {
  diet_profile_t* profile = NULL;
  string sessionKey;
  char* errorInfo = NULL;
  string serviceName = name;
  string msgErrorDiet;

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();
  //IN Parameters
  if (diet_string_set(diet_parameter(profile,0), strdup(sessionKey.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }
  if (diet_string_set(diet_parameter(profile,1), strdup(fmt.c_str()), DIET_VOLATILE)) {
    msgErrorDiet += "with format parameter "+fmt;
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(diet_parameter(profile,2), NULL, DIET_VOLATILE);
  if(!diet_call(profile)) {
    if(diet_string_get(diet_parameter(profile,2), &errorInfo, NULL)){
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
