#include "Options.hpp"

#ifndef CLIUTILSTMS_HPP
#define CLIUTILSTMS_HPP

/**
 * \brief Contains a generic diet error message
 */
const std::string dietErrorMsg="DIET initialization failed !";

/**
 * \brief Contains a generic message to display the current session key
 */
const std::string currentSessionKeyMsg="the current sessionkey is: ";

/**
 * \struct env_name_mapper
 * \brief a environment variable name mapper
 *
 * For each environment variable, the name_mapper function
 * is called to obtain the option name. If it returns empty string
 * the variable is ignored.
 */
struct env_name_mapper{

/**
 * Mapper operator for environnement variable
 * \param name The environment variable name
 * \return The name used in the program
 */

	std::string operator ()(std::string name){

		std::string result= "";

                if (name=="VISHNU_CONFIG_FILE") {

                        result="dietConfig";
                }


		return result;
	}
};

/**
 * \enum ErrorType
 * \brief a convenient type to classify error type
 */
typedef enum{
PARAMERROR,/*!< for any invalid parameter error */
EXECERROR /*!<  for any runtime error */
}ErrorType;

/**
 * \brief helper function to display help about a specific command 
 * \param opt: describes all options allowed by the command
 * \param signature: defines the usage of the command 
 */
void 
helpUsage (const Options& opt,const std::string& signature);

/**
 * \brief helper function to display error about a specific command 
 * \param cli   :The name of the command
 * \param errMsg: The error message to display 
 * \param err   : The error type 
 */

void
errorUsage (const std::string& cli,const std::string& errMsg,const ErrorType& err=PARAMERROR);

/**
 * \brief Helper function to display information (error or usage) about a specific command 
 * \param opt   :describes all options allowed by the command
 * \param mess: The help usage message to display 
 * \param ewhat   : The error message to display
 * \return 0 if an help is required or 1 if an error must me displayed
 */

int
usage (const Options& opt,const std::string& mess,const std::string& ewhat);


/**
 * \brief A helper function which check if VISHNU_CONFIG_FILE is set
 *\param opt: describes all options allowed by the command
 *\exception raise a runtime exception if the VISHNU_CONFIG_FILE is not set
 */
void 
checkVishnuConfig(const Options& opt);

#endif
