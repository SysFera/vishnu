
#include "api_ums.h"

using namespace std;

///////////////////////////////////////A ENLEVER APRES C'EST JUSTE POUR DES TESTS//////////////////////////////
void print(UMS_Data::User_ptr user, int id) {

  cout << "============ User " << id << " Content ===========" << endl;
  cout << "    firstname=" << user->getFirstname() << endl;
  cout << "    lastname=" << user->getLastname() << endl;
  cout << "    privilege=" << user->getPrivilege() << endl;
  cout << "    mail=" << user->getEmail() << endl;
  cout << "    userId=" << user->getUserId() << endl;
  cout << "=======================================" << endl;
}

void print(UMS_Data::Machine_ptr machine, int id) {

  cout << "============ machine " << id << " Content ===========" << endl;
  cout << "    machineId=" << machine->getMachineId() << endl;
  cout << "    name=" << machine->getName() << endl;
  cout << "    site=" << machine->getSite() << endl;
  cout << "    machineDescription=" << machine->getMachineDescription() << endl;
  cout << "    language=" << machine->getLanguage() << endl;
  cout << "=======================================" << endl;
}

void  print(UMS_Data::LocalAccount_ptr localAccount, int id) {

  cout << "============ localAccount " << id << " Content ===========" << endl;
  cout << "    UserId=" << localAccount->getUserId() << endl;
  cout << "    machineId=" << localAccount->getMachineId() << endl;
  cout << "    acLogin=" << localAccount->getAcLogin() << endl;
  cout << "    sshKeyPath=" << localAccount->getSshKeyPath() << endl;
  cout << "    homeDirectory=" << localAccount->getHomeDirectory() << endl;
  cout << "=======================================" << endl;

}
//////////////////////////////////////////FIN A ENLEVER////////////////////////////////////////////////////////

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

int saveConfiguration(const std::string& sessionKey, const std::string filePath, UMS_Data::Configuration& config) {
  
   SessionProxy sessionProxy(sessionKey);
   ConfigurationProxy configurationProxy(filePath, sessionProxy);

   int res = configurationProxy.save();
   UMS_Data::Configuration configData = configurationProxy.getData();
   
   //To set the file path 
   config.setFilePath(configData.getFilePath());
  //To set the user list 
  for(int i = 0; i < configData.getListConfUsers().size(); i++) {
      UMS_Data::User_ptr user = configData.getListConfUsers().get(i);
      config.getListConfUsers().push_back(user);
  }
  //To set the machine list 
  for(int i = 0; i < configData.getListConfMachines().size(); i++) {
      UMS_Data::Machine_ptr machine = configData.getListConfMachines().get(i);
      config.getListConfMachines().push_back(machine);
  }
  //To set the LocalAccounts list 
  for(int i = 0; i < configData.getListConfLocalAccounts().size(); i++) {
      UMS_Data::LocalAccount_ptr localAccount = configData.getListConfLocalAccounts().get(i);
      config.getListConfLocalAccounts().push_back(localAccount);
  }


 //A enlever apres, c'est juste pour les tests
    //To set the file path 
  std::cout << "config filePath=" << config.getFilePath() << std::endl;
  //To set the user list 
  for(int i = 0; i < config.getListConfUsers().size(); i++) {
      UMS_Data::User_ptr user = config.getListConfUsers().get(i);
      print(user, i);
  }
  //To set the machine list 
  for(int i = 0; i < config.getListConfMachines().size(); i++) {
      UMS_Data::Machine_ptr machine = config.getListConfMachines().get(i);
      print(machine, i);
  }
  //To set the LocalAccounts list 
  for(int i = 0; i < config.getListConfLocalAccounts().size(); i++) {
      UMS_Data::LocalAccount_ptr localAccount = config.getListConfLocalAccounts().get(i);
      print(localAccount, i);
  }

  return res;
}

int saveConfiguration(const std::string& sessionKey, const UMS_Data::Configuration& config) {

   SessionProxy sessionProxy(sessionKey);
   ConfigurationProxy configurationProxy(config.getFilePath(), sessionProxy);

   int  res = configurationProxy.restoreFromFile();
 return res;
}

