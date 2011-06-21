/**
 * \file listSessionUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU list session
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#include "Options.hpp"

#include "connectUtils.hpp"
#include "listMachineUtils.hpp"
#include<iostream>
using namespace std;


/**
 * \brief To build options for the VISHNU list session command
 * \param pgName : The name of the command
 * \param fUserId: The user identifier option callback
 * \param dietConfig: Represents the VISHNU config file
 * \param fMachineId: The machine identifier option callback
 * \param fSessionId: The session identifier option callback
 * \param fStatus: The session status option callback
 * \param fClosePolicy: The session close policy option callback
 * \param fSessionInactivityDelay: The session inactivity delay option callback
 * \param fStartDateOption: The session start date option callback
 * \param fEndDateOption: The session end date option callback
 * \return The description of all options allowed by the command
 */

boost::shared_ptr<Options>
makeListSessionOptions(std::string pgName,StringcallBackType& fUserId,
                       std::string & dietConfig,  StringcallBackType& fMachineId,
                       StringcallBackType& fSessionId, StatuscallBackType& fStatus,
                       ClosePolicycallBackType& fClosePolicy, IntcallBackType& fSessionInactivityDelay,
                       LongcallBackType& fStartDateOption, LongcallBackType& fEndDateOption){


  boost::shared_ptr<Options> opt(new Options(pgName));

  return opt;


}






