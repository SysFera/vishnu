#include "utils.hh"

using namespace std;

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
