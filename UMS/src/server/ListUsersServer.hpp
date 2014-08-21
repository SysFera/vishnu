/**
 * \file ListUsersServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_USERS_H_SERVER_
#define _LIST_USERS_H_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "QueryServer.hpp"
#include "ListUsersOptions.hpp"
#include "ListUsers.hpp"


/**
 * \class ListUsersServer
 * \brief ListUsersServer class implementation
 */
class ListUsersServer: public QueryServer<UMS_Data::ListUsersOptions, UMS_Data::ListUsers>
{

public:

  /**
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListUsersServer(const SessionServer& session)
    : QueryServer<UMS_Data::ListUsersOptions, UMS_Data::ListUsers>(),
      mcommandName("vishnu_list_users"),
      msessionServer(session)
  {
  }

  /**
   * \brief Function to treat the ListUsersServer options
   * \param userServer the object which encapsulates user information
   * \param options The object which encapsulates the information of ListUsersServer  options
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(UserServer userServer, const UMS_Data::ListUsersOptions_ptr& options, std::string& sqlRequest) {


    if(! userServer.isAdmin()) {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }
    std::string  userId = options->getUserId();
    if(userId.size()!=0) {
      //To check if the user id is correct
      getNumUser(options->getUserId());
      addOptionRequest("userid", userId, sqlRequest);
    }

    std::string  authSystemId = options->getAuthSystemId();
    if (! authSystemId.empty()) {
      checkAuthSystemId(authSystemId);
      std::string luserCmd = boost::str(boost::format("SELECT DISTINCT userid "
                                                      " FROM authsystem, users, authaccount "
                                                      " WHERE authaccount.authsystem_numauthsystemid=authsystem.numauthsystemid"
                                                      " AND authaccount.users_numuserid=users.numuserid"
                                                      " AND authsystemid='%1%'"
                                                      " AND authsystem.status!=%2%"
                                                      " AND users.status!=%2%"
                                                      " AND authaccount.status!=%2%"
                                                      )
                                        % mdatabase->escapeData(authSystemId)
                                        % vishnu::STATUS_DELETED);
      sqlRequest.append(" AND userid IN ("+mdatabase->escapeData(luserCmd)+")");
    }

  }

  /**
  * \brief Function to list users information
  * \return The pointer to the UMS_Data::ListUsers containing users information
  * \return raises an exception on error
  */
  UMS_Data::ListUsers* list(UMS_Data::ListUsersOptions_ptr option) {

    std::vector<std::string>::iterator dbResultIter;
    std::vector<std::string> results;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListUsers();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (! userServer.exist()) {
      throw UMSVishnuException (ERRCODE_UNKNOWN_USER);
    }

    std::string sqlQuery = "";
    if ( userServer.isAdmin()) {
      // query all users
      sqlQuery = boost::str(boost::format(
                              "SELECT userid, pwd, firstname, lastname, privilege, email, status"
                              " FROM users"
                              " WHERE users.status<>%1%") % vishnu::STATUS_DELETED);
    } else {
      sqlQuery = boost::str(boost::format(
                              "SELECT userid, pwd, firstname, lastname, privilege, email, status"
                              " FROM users"
                              " WHERE userid != '%1%'"
                              " AND users.status != %2%")
                            % vishnu::ROOTUSERNAME
                            % vishnu::STATUS_DELETED);
    }

    processOptions(userServer, option, sqlQuery);

    sqlQuery.append(" ORDER BY userid");

    //To get the list of users from the database
    boost::scoped_ptr<DatabaseResult> ListofUsers (mdatabase->getResult(sqlQuery));

    if (ListofUsers->getNbTuples() != 0){
      for (size_t resultIndex = 0; resultIndex < ListofUsers->getNbTuples(); ++resultIndex) {
        results.clear();
        results = ListofUsers->get(resultIndex);
        dbResultIter = results.begin();
        UMS_Data::User_ptr user = ecoreFactory->createUser();
        user->setUserId(*dbResultIter);
        user->setPassword(*(++dbResultIter));
        user->setFirstname(*(++dbResultIter));
        user->setLastname(*(++dbResultIter));
        user->setPrivilege(vishnu::convertToInt(*(++dbResultIter)));
        user->setEmail(*(++dbResultIter));
        user->setStatus(vishnu::convertToInt(*(++dbResultIter)));

        mlistObject->getUsers().push_back(user);
      }
    }

    return mlistObject;


  }

  /**
   * \brief Function to get the name of the ListUsersServer command line
   * \return The the name of the ListUsersServer command line
   */
  std::string
  getCommandName()
  {
    return mcommandName;
  }



  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListUsersServer() { }

private:
  /**
  * \brief The name of the ListMachinesServer command line
  */
  std::string mcommandName;

  /**
  * \brief The SessionServer object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionServer msessionServer;

#if 0
  /**
  * \brief The Object containing users information
  */
  ListUsers* mlistUsers;
  /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseInstance;

#endif

};

#endif
