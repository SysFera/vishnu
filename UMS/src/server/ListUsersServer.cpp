#ifndef _QUERY_SERVER_H_
#define _QUERY_SERVER_H_

#include <string>
#include <vector>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "ListUsersServer.hpp"

//Constructors
ListUsersServer::ListUsersServer(const SessionServer session)
{
   mlistUsers = NULL;
   DbFactory factory;
   mdatabaseVishnu = factory.getDatabaseInstance();
}

ListUsersServer::ListUsersServer(std::string option, const SessionServer& session):
moption(option), msessionServer(session)
{
   mlistUsers = NULL;
   DbFactory factory;
   mdatabaseVishnu = factory.getDatabaseInstance();
}

void ListUsersServer::processOptions(UserServer userServer, const std::string& options, std::string& sqlRequest)
{
  if(!userServer.isAdmin()&&(options.size()!=0)) {
    UMSVishnuException e (ERRCODE_NO_ADMIN);
    throw e;
  }

  if(options.size()!=0) {
    sqlRequest.append(" and userid=");
    sqlRequest.append("'"+moption+"'");
    DatabaseResult *ListofUsers = mdatabaseVishnu->getResult(sqlRequest.c_str());
    if(ListofUsers->getNbTuples()==0) {
      UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
      throw e ;
    }
  }

}

//To list sessions
ListUsers* ListUsersServer::list()
{
  DatabaseResult *ListofUsers;
  std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users \
                              where not userid='"+ ROOTUSERNAME +"'";

  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mlistUsers = ecoreFactory->createListUsers();

  try {
    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {
        
      processOptions(userServer, moption, sqlListofUsers);
      //To get the list of users from the database
      ListofUsers = mdatabaseVishnu->getResult(sqlListofUsers.c_str());

      if (ListofUsers->getNbTuples() != 0){
        for (size_t i = 0; i < ListofUsers->getNbTuples(); ++i) {
          results.clear();
          results = ListofUsers->get(i);
          ii = results.begin();
          UMS_Data::User_ptr user = ecoreFactory->createUser();
          user->setUserId(*ii);
          user->setPassword(*(++ii));
          user->setFirstname(*(++ii));
          user->setLastname(*(++ii));
          user->setPrivilege(convertToInt(*(++ii)));
          user->setEmail(*(++ii));
          user->setStatus(convertToInt(*(++ii)));

          mlistUsers->getUsers().push_back(user);
         }
      }
   } else {
        UMSVishnuException e (ERRCODE_UNKNOWN_USER);
        throw e;
      }
  } 
  catch (VishnuException& e) {
      throw;
  }

  return mlistUsers;
}

  //Destructor
ListUsersServer::~ListUsersServer() {
}

#endif
