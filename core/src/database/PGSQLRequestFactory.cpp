#include "PGSQLRequestFactory.hpp"
#include "RequestFactory.hpp"
#include <map>

PGSQLRequestFactory::PGSQLRequestFactory():RequestFactory(){
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_ACCOUNT_WITH_USERS, "update account set status=%1% from users where users.numuserid=account.users_numuserid and users.userid='%2%';"));
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_ACCOUNT_WITH_MACHINE, "update account set status=%1% from machine where machine.nummachineid=account.machine_nummachineid and machine.machineid='%2%';"));
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_AUTHACCOUNT_WITH_AUTHSYSTEM, "update authaccount set status=%1% from authsystem where authsystem.numauthsystemid=authaccount.authsystem_numauthsystemid and authsystem.authsystemid='%2%';"));
}

PGSQLRequestFactory::~PGSQLRequestFactory(){
}

std::string
PGSQLRequestFactory::get(const int key){
  return mrequest[key];
}

