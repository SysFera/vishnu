/**
 * \file configureOptionUtils.hpp
 * \brief This file declare useful functions and types for the VISHNU configure 
 * option command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#ifndef CONFIGUREOPTIONUTILS__HH
#define CONFIGUREOPTIONUTILS__HH

//VISHNU Headers

#include "cliUtil.hpp"
#include "common.hpp"

//Boost Headers
#include <boost/function.hpp>


/**
 * \brief A convenient function type taking a string parameter and
 * without return value
 */


typedef
boost::function1<void,std::string> StringcallBackType;


/**
 * \brief Another convenient function type to handle the callback 
 * 
 */


typedef  
boost::function2<int,const std::string&,const UMS_Data::OptionValue&> ConfigureCallBackType;

/**
 * \brief To build options for the VISHNU configure option command
 * \param opt: Describes all allowed options for the VISHNU configure option
 * command
 * \param dietConfig: Represents the VISHNU config file
 * \param fOptionName : A callback for the option name
 * \param fValue      : A callback for the option value
 */

void
makeConfigureOptions(boost::shared_ptr<Options> opt,std::string& dietConfig,
		                      StringcallBackType& fOptionName,StringcallBackType& fValue
                         );
/**
 * \brief A common function betwenn the VISHNU configure option command
 * and the VISHNU configure default option command
 * \param opt:  Describes all allowed options for the command
 * \param ac :  The number of parameters the command takes
 * \param av : The name of all of that parameters
 * \param conf_func: The VISHNU API function, configureOption our
 * ConfigureDefaultOption
 */
void
commonConfigure(boost::shared_ptr<Options> opt, int ac, char* av[] , const ConfigureCallBackType& conf_func);



#endif



