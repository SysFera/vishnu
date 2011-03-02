/**
 * \file listMachineUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU list machine
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#include "Options.hpp"

#include "connectUtils.hpp"
#include "listMachineUtils.hpp"
#include<iostream>
using namespace std;


/**
 * \brief To build options for the VISHNU list machine command
 * \param pgName : The name of the command
 * \param fUserId: The user identifier option callback
 * \param dietConfig: Represents the VISHNU config file
 * \param fMachineId : The callback for the machine identifier
 * \return The description of all options allowed by the command
 */




boost::shared_ptr<Options>
makeListMachineOptions(std::string pgName,StringcallBackType& fUserId,
                       std::string & dietConfig,StringcallBackType& fMachineId){

  boost::shared_ptr<Options> opt(new Options(pgName));

  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  opt->add("userId,u",
           "An admin option for listing machines in which a specific user has a local configuration",
           CONFIG,
           fUserId);


  opt->add("machineId,i",
           "An option for listing information about a  specific machine",
           CONFIG,
           fMachineId);


  return opt;



}






