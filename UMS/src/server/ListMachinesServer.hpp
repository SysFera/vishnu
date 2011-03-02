/**
 * \file ListMachinesServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_MACHINES_SERVER_
#define _LIST_MACHINES_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "QueryServer.hpp"
#include "ListMachineOptions.hpp"
#include "ListMachines.hpp"

/**
 * \class ListMachinesServer
 * \brief ListMachinesServer class implementation
 */
class ListMachinesServer: public QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>
{

public:

  /**
   * \fn ListMachinesServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListMachinesServer(const SessionServer session):
  QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(session)
  {
    mcommandName = "vishnu_list_machine";
  }
  /**
   * \fn ListMachinesServer(UMS_Data::ListMachineOptions_ptr params,
   *                        const SessionServer& session)
   * \param params The object which encapsulates the information of ListMachinesServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListMachinesServer(UMS_Data::ListMachineOptions_ptr params, const SessionServer& session):
  QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(params, session)
  {
    mcommandName = "vishnu_list_machine";
  }

  /**
   * \brief Function to treat the ListMachinesServer options
   * \fn void processOptions(UserServer userServer,
   *                         const UMS_Data::ListMachineOptions_ptr& options,
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListMachinesServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListMachineOptions_ptr& options, std::string& sqlRequest)
  {
    std::string sqlListofMachinesWithJointure = "SELECT machineid, name, site, machine.status, lang, description, userid "
    " from machine, description, account, users where machine.nummachineid = description.machine_nummachineid "
    " and account.machine_nummachineid=machine.nummachineid and account.users_numuserid=users.numuserid";

    std::string sqlListofMachinesIntial =  sqlRequest;

    size_t userIdSize = options->getUserId().size();
    size_t machineIdSize = options->getMachineId().size();
    bool isListAll = options->isListAllmachine();

    if ((!userServer.isAdmin()) && userIdSize!=0) {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }

    if(!isListAll) {
       sqlRequest = sqlListofMachinesWithJointure;
       addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
    }

    //The admin option
    if(userIdSize!=0) {
      //To check if the user id is correct
      checkUserId(options->getUserId());

      sqlRequest = sqlListofMachinesWithJointure;
      addOptionRequest("userid", options->getUserId(), sqlRequest);
    }

    if(machineIdSize!=0) {
      //To check if the machine id is correct
      checkMachineId(options->getMachineId());

      if(!isListAll && userIdSize==0) {
         sqlRequest=sqlListofMachinesIntial;
      }
      addOptionRequest("machineid", options->getMachineId(), sqlRequest);
    }

 }

 /**
  * \brief Function to list machines information
  * \fn UMS_Data::ListMachines* list()
  * \return The pointer to the UMS_Data::ListMachines containing machines information
  * \return raises an exception on error
  */
 UMS_Data::ListMachines* list() {
    DatabaseResult *ListofMachines;
    std::string sqlListofMachines = "SELECT machineid, name, site, status, lang, description from machine, description "
    "where machine.nummachineid = description.machine_nummachineid";

    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListMachines();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {

      //To process options
      processOptions(userServer, mparameters, sqlListofMachines);

      ListofMachines = mdatabaseVishnu->getResult(sqlListofMachines.c_str());
      if (ListofMachines->getNbTuples() != 0){
        for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
          results.clear();
          results = ListofMachines->get(i);
          ii = results.begin();
          UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
          machine->setMachineId(*ii);
          machine->setName(*(++ii));
          machine->setSite(*(++ii));
          machine->setStatus(convertToInt(*(++ii)));
          machine->setLanguage(*(++ii));
          machine->setMachineDescription(*(++ii));

          mlistObject->getMachines().push_back(machine);
        }
      }

    } else {
      UMSVishnuException e (ERRCODE_UNKNOWN_USER);
      throw e;
    }
    return mlistObject;
  }

  /**
   * \brief Function to get the name of the ListMachinesServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListMachinesServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListMachinesServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListMachinesServer()
  {
  }

  private:

   /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The name of the ListMachinesServer command line
  */
  std::string mcommandName;

};

#endif
