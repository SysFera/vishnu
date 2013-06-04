#include "MYSQLRequestFactory.hpp"
#include "RequestFactory.hpp"
#include <map>

MYSQLRequestFactory::MYSQLRequestFactory():RequestFactory(){
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_ACCOUNT_WITH_USERS, "UPDATE account, users SET account.status='%1%' WHERE users.numuserid=account.users_numuserid and users.userid='%2%';"));
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_ACCOUNT_WITH_MACHINE, "UPDATE account, machine SET account.status='%1%' WHERE machine.nummachineid=account.machine_nummachineid and machine.machineid='%2%';"));
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_AUTHACCOUNT_WITH_AUTHSYSTEM, "UPDATE authaccount, authsystem SET authaccount.status='%1%' WHERE authsystem.numauthsystemid=authaccount.authsystem_authsystemid and authsystem.authsystemid='%2%';"));
}

MYSQLRequestFactory::~MYSQLRequestFactory(){
}

std::string
MYSQLRequestFactory::get(const int key){
  return mrequest[key];
}

