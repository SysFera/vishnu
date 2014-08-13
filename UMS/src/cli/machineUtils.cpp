/**
 * \file machineUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU add
 * update and delte commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include <boost/shared_ptr.hpp>  // for shared_ptr
#include <string>                       // for string

#include "Options.hpp"
#include "localAccountUtils.hpp"

using namespace std;


/**
 * \brief To build options for the VISHNU machine commands
 * \param pgName : The name of the command
 * \param configFile : Represents the VISHNU config file
 * \param fMachineId: The machine ID option callback
 * \param fAddress : Callback to bind host address
 * \param fMachineDescription : The machine description option callback
 * \param type: 1 for create, 0 for update
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options> makeMachineOptions(std::string pgName,
                                              std::string& configFile,
                                              StringcallBackType& fMachineId,
                                              StringcallBackType& fAddress,
                                              StringcallBackType& fMachineDescription,
                                              int type){

  boost::shared_ptr<Options> opt(new Options(pgName));


  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  opt->add("machineId,n",
           "A shortname identifying the machine",
           HIDDEN,
           fMachineId,
           type);

  if (type != 0) {
    opt->setPosition("machineId",1);
  }

  opt->add("address,a",
           "The fully qualified domain name of the machine",
           CONFIG,
           fAddress,
           type);

  opt->add("description,d",
           "The description of the machine",
           CONFIG,
           fMachineDescription,
           0);


  return opt;
}
