/**
 * \file machineUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU add
 * update and delte commands
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef MACHINEUTILS__HH
#define MACHNINEUTILS__HH


#include <boost/shared_ptr.hpp>
#include "common.hpp"


class Options;

/**
 * \brief A convenient function type taking a string parameter and
 * without return value
 */


typedef  boost::function1<void,std::string> StringcallBackType;


/**
 * \brief To build options for the VISHNU machine commands
 * \param pgName : The name of the command
 * \param configFile : Represents the VISHNU config file
 * \param fMachineId : The machine name option callback
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
                                              int type);


#endif
