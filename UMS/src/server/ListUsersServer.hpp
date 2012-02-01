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
   * \fn ListUsersServer(const SessionServer& session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
   ListUsersServer(const SessionServer& session):
   QueryServer<UMS_Data::ListUsersOptions, UMS_Data::ListUsers>(session), mcommandName("vishnu_list_users")
  {

    // mcommandName ="vishnu_list_users";
    }
  /**
   * \fn ListUsersServer(const std::string& option,
   *                     const SessionServer& session)
   * \param option The ListUsersServer option 
   * \param params The object which encapsulates the information of ListUsersServer  options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListUsersServer(UMS_Data::ListUsersOptions_ptr params, const SessionServer& session): 
  QueryServer<UMS_Data::ListUsersOptions, UMS_Data::ListUsers>(params,session), mcommandName("vishnu_list_users")
  {


     //mcommandName ="vishnu_list_users";

  }

  /**
   * \brief Function to treat the ListUsersServer options 
   * \fn void processOptions(UserServer userServer,
   *                         const std::string& options,
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options The object which encapsulates the information of ListUsersServer  options
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListUsersOptions_ptr& options, std::string& sqlRequest) {


  if(!userServer.isAdmin()) {
    UMSVishnuException e (ERRCODE_NO_ADMIN);
    throw e;
  }
  std::string  userId = options->getUserId();
  if(userId.size()!=0) {
    //To check if the user id is correct

    checkUserId(options->getUserId());


    addOptionRequest("userid", userId, sqlRequest);


    /*sqlRequest.append(" and userid=");
    sqlRequest.append("'"+userId+"'");
    boost::scoped_ptr<DatabaseResult> ListofUsers (mdatabaseVishnu->getResult(sqlRequest.c_str()));
    if(ListofUsers->getNbTuples()==0) {
      UMSVishnuException e(ERRCODE_UNKNOWN_USERID);
      throw e ;
    }     */



  }


  }

 /**
  * \brief Function to list machines information 
  * \fn UMS_Data::ListUsers* list()
  * \return The pointer to the UMS_Data::ListUsers containing users information
  * \return raises an exception on error
  */
  UMS_Data::ListUsers* list() {

  std::string sqlListofUsers = "SELECT userid, pwd, firstname, lastname, privilege, email, status from users "
                              "where not userid='"+ ROOTUSERNAME +"'";

  std::vector<std::string>::iterator ii;
  std::vector<std::string> results;
  UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
  mlistObject = ecoreFactory->createListUsers();

  //Creation of the object user
  UserServer userServer = UserServer(msessionServer);
  userServer.init();
  //if the user exists
  if (userServer.exist()) {

    processOptions(userServer, mparameters, sqlListofUsers);
    sqlListofUsers.append(" order by userid");
    //To get the list of users from the database
    boost::scoped_ptr<DatabaseResult> ListofUsers (mdatabaseVishnu->getResult(sqlListofUsers.c_str()));
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

        mlistObject->getUsers().push_back(user);
        }
    }
  }
  else {
    UMSVishnuException e (ERRCODE_UNKNOWN_USER);
    throw e;
  }
  return mlistObject;


  }
  
  /**
   * \brief Function to get the name of the ListUsersServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListUsersServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }



  /**
   * \fn ~ListUsersServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListUsersServer() { } 

 private:

    /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListMachinesServer command line
  */
  std::string mcommandName;

#if 0
 /**
  * \brief The ListUsersServer option 
  */
  UMS_Data::ListUsersOptions_ptr moption;
 /**
  * \brief The SessionServer object containing the encrypted identifier of the session
  *  generated by VISHNU
  */
  SessionServer msessionServer;
 /**
  * \brief The Object containing users information 
  */
  ListUsers* mlistUsers;
 /**
  * \brief An instance of vishnu database
  */
  Database *mdatabaseVishnu;

#endif

};

#endif
