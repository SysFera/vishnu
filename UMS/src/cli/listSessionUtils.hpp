/**
 * \file listSessionUtils.hpp
 * \brief This file declares useful functions and types for the VISHNU list session
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */



#ifndef LISTSESSIONUTILS__HH
#define LISTSESSIONUTILS__HH

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
 * \brief The close policy option callback type
 */

typedef  
boost::function1<void,UMS_Data::SessionCloseType> ClosePolicycallBackType;

/**
 * \brief The status option callback type
 */

typedef
boost::function1<void,UMS_Data::UMS_Data::StatusType> StatuscallBackType;

/**
 * \brief A convenient function type taking a long integer parameter and
 * without return value
 */


typedef 
boost::function1<void,long> LongcallBackType;

/**
 * \brief A convenient function type taking a integer parameter and
 * without return value
 */

typedef
boost::function1<void,int> IntcallBackType;


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
                       LongcallBackType& fStartDateOption, LongcallBackType& fEndDateOption);




#endif



