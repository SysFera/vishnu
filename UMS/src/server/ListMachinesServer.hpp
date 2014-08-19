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
   * \brief Constructor, raises an exception on error
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   */
  ListMachinesServer(const SessionServer session)
    : QueryServer<UMS_Data::ListMachineOptions, UMS_Data::ListMachines>(),
      mcommandName("vishnu_list_machines"),
      msessionServer(session)
  {
  }

  /**
   * \brief Function to treat the ListMachinesServer options
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListMachinesServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(UserServer userServer, const UMS_Data::ListMachineOptions_ptr& options, std::string& sqlRequest)
  {
    std::string sqlJoinLstMachines = boost::str(boost::format("SELECT DISTINCT machineid, address, machine.status, description, userid "
                                                              " FROM machine, account, users"
                                                              " WHERE account.machine_nummachineid = machine.nummachineid "
                                                              " AND account.users_numuserid = users.numuserid "
                                                              " AND machine.status != %1%"
                                                              " AND account.status != %1%"
                                                              " AND users.status   != %1%"
                                                              ) % vishnu::STATUS_DELETED);
    std::string sqlListofMachinesIntial = sqlRequest;
    bool isListAll = options->isListAllMachine();

    if (! userServer.isAdmin() && ! options->getUserId().empty()) {
      throw UMSVishnuException (ERRCODE_NO_ADMIN);
    }

    if(! isListAll) {
      sqlRequest = sqlJoinLstMachines;
      addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
    }

    //The admin option
    if (! options->getUserId().empty()) {
      sqlRequest = sqlJoinLstMachines;
      addOptionRequest("users.numuserid", getNumUser(options->getUserId()), sqlRequest);
    }

    if (! options->getMachineId().empty()) {
      getNumMachine( options->getMachineId() );

      if(! isListAll && options->getUserId().empty()) {
        sqlRequest = sqlListofMachinesIntial;
      }

      addOptionRequest("machineid", options->getMachineId(), sqlRequest);
    }
  }

  /**
  * \brief Function to list machines information
  * \return The pointer to the UMS_Data::ListMachines containing machines information
  * \return raises an exception on error
  */
  UMS_Data::ListMachines* list(UMS_Data::ListMachineOptions_ptr option) {
    std::string sqlListofMachines = boost::str(boost::format("SELECT machineid, address, status, description"
                                                             " FROM machine"
                                                             " WHERE machine.status != %1%") % vishnu::STATUS_DELETED);
    std::vector<std::string>::iterator dbResultIter;
    std::vector<std::string> dbResults;
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListMachines();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {

      //To process options
      processOptions(userServer, option, sqlListofMachines);

      boost::scoped_ptr<DatabaseResult> ListofMachines (mdatabase->getResult(sqlListofMachines.c_str()));
      if (ListofMachines->getNbTuples() != 0){
        for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
          dbResults.clear();
          dbResults = ListofMachines->get(i);
          dbResultIter = dbResults.begin();
          UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
          machine->setMachineId(*dbResultIter);
          machine->setAddress(*(++dbResultIter));
          machine->setStatus(vishnu::convertToInt(*(++dbResultIter)));
          machine->setDescription(*(++dbResultIter));

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
   * \return The the name of the ListMachinesServer command line
   */
  std::string
  getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListMachinesServer()
  {
  }

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
};

#endif
