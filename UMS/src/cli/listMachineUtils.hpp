/**
 * \file listMachineUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU list machine
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef LISTMACHINEUTILS__HH
#define LISTMACHINEUTILS__HH


//VISHNU Headers

#include "common.hpp"

//Boost Headers

#include<boost/function.hpp>




class Options;

/**
 * \brief A convenient function type taking a string parameter and
 * without return value
 */


typedef
boost::function1<void,std::string> StringcallBackType;

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
                       std::string & dietConfig,StringcallBackType& fMachineId);






#endif



