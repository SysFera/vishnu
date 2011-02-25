#ifndef _LIST_USERS_H_SERVER_
#define _LIST_USERS_H_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "ListUsers.hpp"

class ListUsersServer
{

public:
  //Constructors
  explicit ListUsersServer(const SessionServer session);
  ListUsersServer(std::string option, const SessionServer& session);

  //To process options
  void processOptions(UserServer userServer, const std::string& options, std::string& sqlRequest);

  //To list users 
  UMS_Data::ListUsers* list();

  //Destructor
  ~ListUsersServer(); 

 private:

  std::string moption;
  SessionServer msessionServer;
  ListUsers* mlistUsers;

  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;


};

#endif
