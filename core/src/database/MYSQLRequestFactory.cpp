#include "MYSQLRequestFactory.hpp"
#include "RequestFactory.hpp"
#include <map>

MYSQLRequestFactory::MYSQLRequestFactory():RequestFactory(){
  mrequest.insert (std::pair<int, std::string>(VR_UPDATE_ACCOUNT_WITH_USERS, "UPDATE account, users SET account.status='%1%' WHERE users.numuserid=account.users_numuserid and users.userid='%2%';"));
}

MYSQLRequestFactory::~MYSQLRequestFactory(){
}

std::string
MYSQLRequestFactory::get(const int key){
  return mrequest[key];
}

