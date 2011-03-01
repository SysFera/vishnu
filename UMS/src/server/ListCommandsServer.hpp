/**
 * \file ListCommandsServer.hpp
 * \brief This file contains the VISHNU QueryServer class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */
#ifndef _LIST_COMMANDS_SERVER
#define _LIST_COMMANDS_SERVER

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include "SessionServer.hpp"
#include "ListCmdOptions.hpp"
#include "UMSMapper.hpp"
#include "MapperRegistry.hpp"

/**
 * \class ListCommandsServer
 * \brief ListCommandsServer class implementation
 */
class ListCommandsServer: public QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>
{

public:

  /**
   * \fn ListCommandsServer(const SessionServer session)
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListCommandsServer(const SessionServer session):
    QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>(session)
  {
    mcommandName ="vishnu_list_history_cmd";
  }
  /**
   * \fn ListCommandsServer(const UMS_Data::ListSessionOptions_ptr params,
   *                        const SessionServer& session)
   * \param params The object which encapsulates the information of ListCommandsServer options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListCommandsServer(UMS_Data::ListCmdOptions_ptr params, const SessionServer& session):
    QueryServer<UMS_Data::ListCmdOptions, UMS_Data::ListCommands>(params, session)
  {
    mcommandName ="vishnu_list_history_cmd";
  }

  /**
   * \brief Function to treat the ListCommandsServer options
   * \fn void processOptions(UserServer userServer,
   *                         const UMS_Data::ListCmdOptions_ptr& options
   *                         std::string& sqlRequest)
   * \param userServer the object which encapsulates user information
   * \param options the object which contains the ListCommandsServer options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void processOptions(UserServer userServer, const UMS_Data::ListCmdOptions_ptr& options, std::string& sqlRequest)
  {
    boost::posix_time::ptime pt;
    size_t userIdSize = options->getUserId().size();
    bool listAll = options->isAdminListOption();

    if ((!userServer.isAdmin()) && (userIdSize!=0 || listAll)) {
       UMSVishnuException e (ERRCODE_NO_ADMIN);
       throw e;
    }

    if(userIdSize!=0) {
       //To check if the user id is correct
      checkUserId(options->getUserId());

      addOptionRequest("userid", options->getUserId(), sqlRequest);
    } else {
            if(!listAll) {
               addOptionRequest("userid", userServer.getData().getUserId(), sqlRequest);
            }
    }

    if(options->getSessionId().size()!=0) {
      //To check if the session id is correct
      checkSessionId(options->getSessionId());

      addOptionRequest("vsessionid", options->getSessionId(), sqlRequest);
    }

    long startDate = options->getStartDateOption();
    if(startDate!=-1) {
      pt =  boost::posix_time::from_time_t(startDate);
      std::string startDateStr =  boost::posix_time::to_simple_string(pt);
      addTimeRequest("starttime", startDateStr, sqlRequest, ">=");
    }

    long endDate = options->getEndDateOption();
    if(endDate!=-1) {
      pt =  boost::posix_time::from_time_t(endDate);
      std::string endDateStr =  boost::posix_time::to_simple_string(pt);
      addTimeRequest("endtime", endDateStr, sqlRequest, "<=");
    }
  }

  /**
   * \brief Function to list commands information
   * \fn UMS_Data::ListCommands* list()
   * \return The pointer to the UMS_Data::ListCommands containing commands information
   * \return raises an exception on error
   */
  UMS_Data::ListCommands* list()
  {
    DatabaseResult *ListOfCommands;
    std::string sqlListOfCommands;
    std::vector<std::string>::iterator ii;
    std::vector<std::string> results;
    std::string description;

    //MAPPER CREATION
    Mapper* mapper = MapperRegistry::getInstance()->getMapper(utilServer::UMSMAPPERNAME);

    sqlListOfCommands = "SELECT ctype, vsessionid, name, description, starttime, endtime, userid from "
                         " vsession, clmachine, command, users where vsession.numsessionid=command.vsession_numsessionid and "
                         " vsession.clmachine_numclmachineid=clmachine.numclmachineid and  vsession.users_numuserid=users.numuserid";


    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createListCommands();

    //Creation of the object user
    UserServer userServer = UserServer(msessionServer);
    userServer.init();
    //if the user exists
    if (userServer.exist()) {

      processOptions(userServer, mparameters, sqlListOfCommands);

      //To get the list of commands from the database
      ListOfCommands = mdatabaseVishnu->getResult(sqlListOfCommands.c_str());
      if (ListOfCommands->getNbTuples() != 0){
        for (size_t i = 0; i < ListOfCommands->getNbTuples(); ++i) {
          results.clear();
          results = ListOfCommands->get(i);
          ii = results.begin();

          UMS_Data::Command_ptr command = ecoreFactory->createCommand();
          command->setCommandId(*ii);
          command->setSessionId(*(++ii));
          command->setMachineId(*(++ii));

          description = mapper->decode(*(++ii));
          std::cout << "DESCRIPTION  " << description << std::endl;
          command->setCmdDescription(description);
          command->setCmdStartTime(convertToTimeType(*(++ii)));
          command->setCmdEndTime(convertToTimeType(*(++ii)));

          mlistObject->getCommands().push_back(command);
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
   * \brief Function to get the name of the ListCommandsServer command line
   * \fn std::string getCommandName()
   * \return The the name of the ListCommandsServer command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \fn ~ListCommandsServer()
   * \brief Destructor, raises an exception on error
   */
  ~ListCommandsServer()
  {
  }

  private:

   /////////////////////////////////
  // Attributes
  /////////////////////////////////

  /**
  * \brief The name of the ListCommandsServer command line
  */
  std::string mcommandName;
};

#endif
