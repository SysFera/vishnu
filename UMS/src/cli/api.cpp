
#include "api.h"

using namespace std;

int connect(string userId, string password, string& sessionKey, UMS_Data::ConnectOptions connectOpt) {

  UserProxy userProxy(userId, password);
  SessionProxy sessionProxy;
  int res = sessionProxy.connect(userProxy, connectOpt);

  sessionKey = sessionProxy.getData().getSessionKey();

  std::string envValue = "export VISHNU_SESSION_KEY="+sessionKey;
  std::cout << envValue << endl;
  std::string test = "echo '"+envValue+"'"+" >> /home/traore/.bashrc";
  std::cout << test << endl;
  //if(system(envValue.c_str())) {
  if(system(test.c_str())) {
     std::cerr << "export error.." << std::endl;
  }; 
 
 return res;
}

int reconnect(string userId, string password, string sessionId, string& sessionKey) {

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

int close(string  sessionKey) {

  UMS_Data::Session session;
  SessionProxy sessionProxy(sessionKey);
  int  res = sessionProxy.close();

 return res;
}

