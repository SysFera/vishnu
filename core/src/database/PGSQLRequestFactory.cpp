#include "PGSQLRequestFactory.hpp"
#include "RequestFactory.hpp"
#include <map>

PGSQLRequestFactory::PGSQLRequestFactory():RequestFactory(){
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_ACCOUNT_WITH_USERS, "update account set status=%1 from users where users.numuserid=account.users_numuserid and users.userid='%2';"));
}

PGSQLRequestFactory::~PGSQLRequestFactory(){
}

std::string
PGSQLRequestFactory::get(const int key){
  return mrequest[key];
}

