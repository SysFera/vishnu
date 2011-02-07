
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

int addVishnuUser(const string& sessionKey, const UMS_Data::User& newUser) {
 
  SessionProxy sessionProxy(sessionKey);
  UserProxy userProxy(sessionProxy);

  int res = userProxy.add(newUser);

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

int changePassword(const std::string& userId, const std::string& password, const std::string& passwordNew) {

  UMS_Data::User user;
  user.setUserId(userId);
  user.setPassword(password);
  UserProxy userProxy(user);

  int res = userProxy.changePassword(passwordNew);
 return res;
}

int resetPassword(const std::string& sessionKey, const std::string& userId) {

  UMS_Data::User user;
  user.setUserId(userId);
  SessionProxy sessionProxy(sessionKey);
  UserProxy userProxy(sessionProxy);
  int res = userProxy.resetPassword(user);

 return res;
}

int addMachine(const std::string& sessionKey, const UMS_Data::Machine& newMachine) {

  SessionProxy sessionProxy(sessionKey);
  MachineProxy machineProxy(newMachine, sessionProxy);

  int res = machineProxy.add();

 return res;
}

int updateMachine(const std::string& sessionKey, const UMS_Data::Machine& machine) {

  SessionProxy sessionProxy(sessionKey);
  MachineProxy machineProxy(machine, sessionProxy);

  int res = machineProxy.update();

 return res;
}

int deleteMachine(const std::string& sessionKey, const std::string& machineId) {

  UMS_Data::Machine machine;
  machine.setMachineId(machineId);
  SessionProxy sessionProxy(sessionKey);
  MachineProxy machineProxy(machine, sessionProxy);

  int res = machineProxy.deleteMachine();
 
 return res;
}

int addLocalAccount(const std::string& sessionKey, const UMS_Data::LocalAccount& newLocalAccount) {

  
  SessionProxy sessionProxy(sessionKey);
  LocalAccountProxy localAccountProxy(newLocalAccount, sessionProxy);

  int res = localAccountProxy.add();
 
 return res;
}

int updateLocalAccount(const std::string& sessionKey, const UMS_Data::LocalAccount& localAccount) {

 SessionProxy sessionProxy(sessionKey);
 LocalAccountProxy localAccountProxy(localAccount, sessionProxy);

 int res = localAccountProxy.update();

 return res;
}

int deleteLocalAccount(const std::string& sessionKey, const std::string& userId, const std::string& machineId) {

  UMS_Data::LocalAccount localAccount;
  localAccount.setUserId(userId);
  localAccount.setMachineId(machineId);
  SessionProxy sessionProxy(sessionKey);
  LocalAccountProxy localAccountProxy(localAccount, sessionProxy);

  int res = localAccountProxy.deleteLocalAccount();

 return res;
}
