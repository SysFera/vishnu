#include "utils.hh"

using namespace std;


#define DATE_SIZE 20


void printSession(UMS_Data::Session_ptr session) {

  ostringstream os_dc;
  ostringstream os_dcl;
  ostringstream os_s;
  ostringstream os_c;
  ostringstream os_t;

  char* date_dlc = (char*) malloc(DATE_SIZE*sizeof(char));
  char* date_dc = (char*) malloc(DATE_SIZE*sizeof(char));
  char* date_dcl = (char*) malloc(DATE_SIZE*sizeof(char));
  struct tm *date_tm;


  std::string sessionId = session->getSessionId();
  std::string userId = session->getUserId();
  std::string sessionKey = session->getSessionKey();
  long dateLastConnect = session->getDateLastConnect();
  date_tm = localtime(&dateLastConnect); strftime(date_dlc, DATE_SIZE, "%m/%d %H:%M", date_tm);
  long dateCreation = session->getDateCreation();
  date_tm = localtime(&dateCreation); strftime(date_dc, DATE_SIZE, "%m/%d %H:%M", date_tm);
  long dateClosure = session->getDateClosure();
  date_tm = localtime(&dateClosure); strftime(date_dcl, DATE_SIZE, "%m/%d %H:%M", date_tm);
  int state = session->getStatus(); os_s << state;
  int closePolicy = session->getClosePolicy(); os_c << closePolicy ;
  long timeOut = session->getTimeout(); os_t << timeOut;


  printf("%-12s %-12s %-15s %-15s %-15s %-15s %-2s %-11s %-7s\n",
  sessionId.c_str(), userId.c_str(), sessionKey.c_str(), date_dlc, date_dc, date_dcl,
    (os_s.str()).c_str(), (os_c.str()).c_str(), (os_t.str()).c_str());
}

void printLocalAccount(UMS_Data::LocalAccount_ptr account) {

  std::string userId = account->getUserId();
  std::string machineId = account->getMachineId();
  std::string acLogin = account->getAcLogin();
  std::string sshKeyPath = account->getSshKeyPath();
  std::string homeDir = account->getHomeDirectory();

  printf("%-15s %-15s %-15s %-20s %-20s\n", userId.c_str(), machineId.c_str(), acLogin.c_str(), sshKeyPath.c_str(), homeDir.c_str());

}

void printMachine(UMS_Data::Machine_ptr machine) {

  std::string name = machine->getName();
  std::string machineId = machine->getMachineId();
  std::string site = machine->getSite();
  std::string descr = machine->getMachineDescription();
  std::string language = machine->getLanguage();

  printf("%-15s %-15s %-15s %-20s %-15s\n", machineId.c_str(), name.c_str(), site.c_str(), descr.c_str(), language.c_str());
}

void printCommand(UMS_Data::Command_ptr command) {

  std::string commandId = command->getCommandId();
  std::string sessionId = command->getSessionId();
  std::string machineId = command->getMachineId();
  std::string descr = command->getCmdDescription();

  char* date_startTime = (char*) malloc(DATE_SIZE*sizeof(char));
  char* date_endTime = (char*) malloc(DATE_SIZE*sizeof(char));
  struct tm *date_tm;

  long  startTime = command->getCmdStartTime();
  date_tm = localtime(&startTime); strftime(date_startTime, DATE_SIZE, "%m/%d %H:%M", date_tm);
  long  endTime = command->getCmdEndTime();
  date_tm = localtime(&endTime); strftime(date_endTime, DATE_SIZE, "%m/%d %H:%M", date_tm);

  printf("%-15s %-15s %-15s %-20s %-15s %-15s\n", commandId.c_str(), sessionId.c_str(), machineId.c_str(), descr.c_str(), date_startTime, date_endTime);

}

void printOptionValue(UMS_Data::OptionValue_ptr optionValue) {

  std::string name = optionValue->getOptionName();
  std::string value = optionValue->getValue();

  printf("%-15s %-15s\n", name.c_str(), value.c_str());
}

void printUser(UMS_Data::User_ptr user) {

  ostringstream os_privilege;

  std::string firstName = user->getFirstname();
  std::string lastName = user->getLastname();
  int privilege = user->getPrivilege(); os_privilege << privilege;
  std::string email = user->getEmail();
  std::string userId = user->getUserId();

  printf("%-15s %-15s %-9s %-30s %-15s\n", firstName.c_str(), lastName.c_str(), (os_privilege.str()).c_str(), email.c_str(), userId.c_str());
}


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
