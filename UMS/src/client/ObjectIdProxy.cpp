#include "ObjectIdProxy.hpp"
#include "UserException.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "UMSServices.hpp"
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
    defineCall(SERVICES_UMS[INT_DEFINEUSERIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad user format");
    throw (e);
  }
}

void
ObjectIdProxy::setMID(string fmt) {
  try {
    defineCall(SERVICES_UMS[INT_DEFINEMACHINEIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad machine format");
    throw (e);
  }
}

void
ObjectIdProxy::setTID(string fmt) {
  try {
    defineCall (SERVICES_UMS[INT_DEFINEJOBIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad task format");
    throw (e);
  }
}

void
ObjectIdProxy::setFID(string fmt) {
  try {
    defineCall (SERVICES_UMS[INT_DEFINETRANSFERIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad file transfert format");
    throw (e);
  }
}

void
ObjectIdProxy::setAID(string fmt) {
  try {
    defineCall (SERVICES_UMS[INT_DEFINEAUTHIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad authentication format");
    throw (e);
  }
}

void
ObjectIdProxy::setWID(string fmt) {
  try {
    defineCall (SERVICES_UMS[INT_DEFINEWORKIDENTIFIER], fmt);
  } catch (UserException& e) {
    e.appendMsgComp("Bad authentication format");
    throw (e);
  }
}

void
ObjectIdProxy::defineCall(string name, string fmt) {

  diet_profile_t* profile = diet_profile_alloc(name, 2);
  diet_string_set(profile,0, msessionProxy.getSessionKey());
  diet_string_set(profile,1, fmt);

  if (diet_call(profile)) {
    raiseCommunicationMsgException("RPC call failed");
  }

  raiseExceptionOnErrorResult(profile);
  diet_profile_free(profile);
}
