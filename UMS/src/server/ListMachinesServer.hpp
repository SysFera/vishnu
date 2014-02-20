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
    std::string sqlJoinLstMachines = (boost::format("SELECT machineid, name, site, machine.status,"
                                                    "        lang, description, userid "
                                                    " FROM machine, description, account, users"
                                                    " WHERE machine.nummachineid = description.machine_nummachineid"
                                                    " AND account.machine_nummachineid = machine.nummachineid "
                                                    " AND account.users_numuserid = users.numuserid "
                                                    " AND machine.status != %1%"
                                                    " AND account.status != %1%"
                                                    " AND users.status != %1%")%vishnu::STATUS_DELETED).str();
    std::string sqlListofMachinesIntial =  sqlRequest;
    size_t userIdSize = options->getUserId().size();
    size_t machineIdSize = options->getMachineId().size();
    bool isListAll = options->isListAllMachine();

    if ((!userServer.isAdmin()) && userIdSize!=0) {
      UMSVishnuException e (ERRCODE_NO_ADMIN);
      throw e;
    }

    if(!isListAll) {
      sqlRequest = sqlJoinLstMachines;
      addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
    }

    //The admin option
    if(userIdSize!=0) {
      //To check if the user id is correct
      checkUserId(options->getUserId());

      sqlRequest = sqlJoinLstMachines;
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
  * \return The pointer to the UMS_Data::ListMachines containing machines information
  * \return raises an exception on error
  */
  UMS_Data::ListMachines* list(UMS_Data::ListMachineOptions_ptr option) {
    std::string sqlListofMachines = (boost::format("SELECT machineid, name, site, status, lang, description"
                                                   " FROM machine, description"
                                                   " WHERE machine.nummachineid = description.machine_nummachineid"
                                                   " AND machine.status != %1%")%vishnu::STATUS_DELETED).str();
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
      processOptions(userServer, option, sqlListofMachines);

      boost::scoped_ptr<DatabaseResult> ListofMachines (mdatabaseInstance->getResult(sqlListofMachines.c_str()));
      if (ListofMachines->getNbTuples() != 0){
        for (size_t i = 0; i < ListofMachines->getNbTuples(); ++i) {
          results.clear();
          results = ListofMachines->get(i);
          ii = results.begin();
          UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
          machine->setMachineId(*ii);
          machine->setName(*(++ii));
          machine->setSite(*(++ii));
          machine->setStatus(vishnu::convertToInt(*(++ii)));
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
