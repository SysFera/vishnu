
#include "api_ums.h"

using namespace std;

int vishnuInitialize(char* cfg, int argc, char** argv) {

  if (diet_initialize(cfg, argc, argv)) {
     return 1;
   }
 return 0;
}

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

int changePassword(const std::string& sessionKey, const std::string& password, const std::string& passwordNew) {

  SessionProxy sessionProxy(sessionKey);
  UserProxy userProxy(sessionProxy);
  int res = userProxy.changePassword(password, passwordNew);
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
   UMS_Data::Configuration *configData = configurationProxy.getData();
 
   UMS_Data::User_ptr user;
  
  //To set the user list 
  for(int i = 0; i < configData->getListConfUsers().size(); i++) {
      user = configData->getListConfUsers().get(i);
      config.getListConfUsers().push_back(user);
  }
  //To set the machine list
  UMS_Data::Machine_ptr machine; 
  for(int i = 0; i < configData->getListConfMachines().size(); i++) {
      machine = configData->getListConfMachines().get(i);
      config.getListConfMachines().push_back(machine);
  }
  //To set the LocalAccounts list
  UMS_Data::LocalAccount_ptr localAccount; 
  for(int i = 0; i < configData->getListConfLocalAccounts().size(); i++) {
      localAccount = configData->getListConfLocalAccounts().get(i);
      config.getListConfLocalAccounts().push_back(localAccount);
  }

  return res;
}

int restoreConfiguration(const std::string& sessionKey, const std::string filePath) {

   SessionProxy sessionProxy(sessionKey);
   ConfigurationProxy configurationProxy(filePath, sessionProxy);

   int  res = configurationProxy.restoreFromFile();
 return res;
}

int configureOption(const std::string& sessionKey, const UMS_Data::OptionValue& optionValue) {
 
  SessionProxy sessionProxy(sessionKey);
  OptionValueProxy optionValueProxy(optionValue, sessionProxy);

  int res = optionValueProxy.configureOption();

 return res;
}

int configureDefaultOption(const std::string& sessionKey, const UMS_Data::OptionValue& optionValue) {

  SessionProxy sessionProxy(sessionKey);
  OptionValueProxy optionValueProxy(optionValue, sessionProxy);

  int res = optionValueProxy.configureDefaultOption();
 return res;
}

int listSessions(const std::string& sessionKey, 
                 UMS_Data::ListSessions& listSession,
                 const UMS_Data::ListSessionOptions& options) {
   
  SessionProxy sessionProxy(sessionKey);
  QueryProxy<UMS_Data::ListSessionOptions, UMS_Data::ListSessions> query(options, sessionProxy, "sessionList");

  UMS_Data::ListSessions* listSession_ptr = query.list();  

  UMS_Data::Session_ptr session;
  for(int i = 0; i < listSession_ptr->getSessions().size(); i++) {
      session = listSession_ptr->getSessions().get(i);
      listSession.getSessions().push_back(session);
  }
 return 0;
}

int listLocalAccount(const std::string& sessionKey, 
                     UMS_Data::ListLocalAccounts& listLocalAcc,
                     const UMS_Data::ListLocalAccOptions& options)
{

  SessionProxy sessionProxy(sessionKey);
  QueryProxy<UMS_Data::ListLocalAccOptions, UMS_Data::ListLocalAccounts> query(options, sessionProxy, "localAccountList");

  UMS_Data::ListLocalAccounts* listLocalAcc_ptr = query.list();

  UMS_Data::LocalAccount_ptr account;
  for(int i = 0; i < listLocalAcc_ptr->getAccounts().size(); i++) {
      account = listLocalAcc_ptr->getAccounts().get(i);
      listLocalAcc.getAccounts().push_back(account);
  }
  return 0;
}

int listMachine(const std::string& sessionKey, 
                UMS_Data::ListMachines& listMachine,
                const UMS_Data::ListMachineOptions& options)
{

  SessionProxy sessionProxy(sessionKey);
  QueryProxy<UMS_Data::ListMachineOptions, UMS_Data::ListMachines> query(options, sessionProxy, "machineList");

  UMS_Data::ListMachines* listMachine_ptr = query.list();

  UMS_Data::Machine_ptr machine;
  for(int i = 0; i < listMachine_ptr->getMachines().size(); i++) {
      machine = listMachine_ptr->getMachines().get(i);
      listMachine.getMachines().push_back(machine);
  }

  return 0;
}

int listHistoryCmd(const std::string& sessionKey, 
                   UMS_Data::ListCommands& listCommands,
                   const UMS_Data::ListCmdOptions& options)
{

  SessionProxy sessionProxy(sessionKey);
  QueryProxy<UMS_Data::ListCmdOptions, UMS_Data::ListCommands> query(options, sessionProxy, "commandList");

  UMS_Data::ListCommands* listCommands_ptr = query.list();

  UMS_Data::Command_ptr command;
  for(int i = 0; i < listCommands_ptr->getCommands().size(); i++) {
      command = listCommands_ptr->getCommands().get(i);
      listCommands.getCommands().push_back(command);
  }

  return 0;
}

int listOptions(const std::string& sessionKey, 
                UMS_Data::ListOptionsValues& listOptValues,
                const UMS_Data::ListOptOptions& options)
{

  SessionProxy sessionProxy(sessionKey);
  QueryProxy<UMS_Data::ListOptOptions, UMS_Data::ListOptionsValues> query(options, sessionProxy, "optionValueList");

  UMS_Data::ListOptionsValues* listOptValues_ptr = query.list();

  UMS_Data::OptionValue_ptr optionValue;
  for(int i = 0; i < listOptValues_ptr->getOptionValues().size(); i++) {
      optionValue = listOptValues_ptr->getOptionValues().get(i);
      listOptValues.getOptionValues().push_back(optionValue);
  }

  return 0;
}

int listUsers(const std::string& sessionKey, 
              UMS_Data::ListUsers& listUsers, 
              const std::string& userIdOption)
{

  SessionProxy sessionProxy(sessionKey);
  QueryProxy<std::string, UMS_Data::ListUsers> query(userIdOption, sessionProxy, "userList");

  UMS_Data::ListUsers* listUsers_ptr = query.listWithParamsString();

  UMS_Data::User_ptr user;
  for(int i = 0; i < listUsers_ptr->getUsers().size(); i++) {
      user = listUsers_ptr->getUsers().get(i);
      listUsers.getUsers().push_back(user);
  }

  return 0;
}

