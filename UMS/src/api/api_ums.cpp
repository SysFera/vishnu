
#include "api_ums.h"

using namespace std;

int connect(const string& userId, const string& password, string& sessionKey, const UMS_Data::ConnectOptions& connectOpt) {

  UserProxy userProxy(userId, password);
  SessionProxy sessionProxy;
  int res = sessionProxy.connect(userProxy, connectOpt);

  sessionKey = sessionProxy.getData().getSessionKey();

  std::string envValue = "export VISHNU_SESSION_KEY="+sessionKey;
  std::cout << envValue << endl;
  if(system(envValue.c_str())) {
     std::cerr << "export error.." << std::endl;
  }; 
 
 return res;
}

int reconnect(const string& userId, const string& password, const string& sessionId, string& sessionKey) {

  UserProxy userProxy(userId, password);
  UMS_Data::Session session;
  session.setSessionId(sessionId);
  SessionProxy sessionProxy(session);

  int res = sessionProxy.reconnect(userProxy);

  sessionKey = sessionProxy.getData().getSessionKey();

  std::string envValue = "export VISHNU_SESSION_KEY="+sessionKey;
  std::cout << envValue << endl;
  if(system(envValue.c_str())) {
     std::cerr << "export error.." << std::endl;
  };

 return res;
}

int close(const string&  sessionKey) {

  UMS_Data::Session session;
  SessionProxy sessionProxy(sessionKey);
  int  res = sessionProxy.close();

 return res;
}

int addVishnuUser(const string& sessionKey, const UMS_Data::User& user) {
 
  SessionProxy sessionProxy(sessionKey);
  UserProxy userProxy(sessionProxy);

  int res = userProxy.add(user);

 return res;
}

int updateUser(const string& sessionKey, const UMS_Data::User& user) {

   SessionProxy sessionProxy(sessionKey);
   UserProxy userProxy(sessionProxy);

  int res = userProxy.update(user);

  return res;
}

int deleteUser(const string& sessionKey, const string& userId) {


  UMS_Data::User user;
  user.setUserId(userId);
  SessionProxy sessionProxy(sessionKey);
  UserProxy userProxy(sessionProxy);

  int res = userProxy.deleteUser(user);

 return res;
}
