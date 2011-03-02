/**
 * \file utils.hpp
 * \brief this file contains a declaration of helper functions used by the command line interface 
 * \authors Daouda Traore (daouda.traore@sysfera.com) and Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */


#ifndef CLIUTILS_HPP
#define CLIUTILS_HPP

#include "UMS_Data.hpp"
#include <iostream>
class Options;

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


/**
 * \brief Display a '-' caracter 
 * \param size: The number of '-' to diplay
 * \param os The output stream in which the display will be done.
 */

void 
setFill(int size, std::ostream& os) ;

/**
 * \brief Helper function to display a session
 * \param os: An output stream in which the session will be printed 
 * \param session: The session to display
 * \return The output stream in which the session has been printed
 */



std::ostream&
operator<<(std::ostream& os, const UMS_Data::Session_ptr& session);

/**
 * \brief Helper function to display a list of session
 * \param os: The output stream in which the list will be printed 
 * \param listSession: The list to display
 * \return The output stream in which the list of sessions has been printed
 */

std::ostream&
operator<<(std::ostream& os, UMS_Data::ListSessions& listSession);

/**
 * \brief Helper function to display a local account
 * \param os: The output stream in which the local account will be printed 
 * \param account: The local account  to display
 * \return The output stream in which the local account has been printed
 */


std::ostream&
operator<<(std::ostream& os, const UMS_Data::LocalAccount_ptr& account);

/**
 * \brief Helper function to display a list of local accounts
 * \param os: The output stream in which the list will be printed 
 * \param lsLocalAccounts: The list to display
 * \return The output stream in which the list of local accounts has been printed
 */


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListLocalAccounts& lsLocalAccounts);


/**
 * \brief Helper function to display a machine 
 * \param os: The output stream in which the machine will be printed 
 * \param machine: The machine  to display
 * \return The output stream in which the machine has been printed
 */



std::ostream&
operator<<(std::ostream& os, const UMS_Data::Machine_ptr& machine);


/**
 * \brief Helper function to display a list of machines
 * \param os: The output stream in which the list will be printed 
 * \param lsMachine: The list to display
 * \return The output stream in which the list of machines has been printed
 */

std::ostream&
operator<<(std::ostream& os, UMS_Data::ListMachines& lsMachine);


/**
 * \brief Helper function to display a command 
 * \param os: The output stream in which the command will be printed 
 * \param command: The command  to display
 * \return The output stream in which the command has been printed
 */


std::ostream&
operator<<(std::ostream& os, const UMS_Data::Command_ptr& command);

/**
 * \brief Helper function to display a list of command
 * \param os: The output stream in which the list will be printed 
 * \param lsCommand: The list to display
 * \return The output stream in which the list of command has been printed
 */


std::ostream& 
operator<<(std::ostream& os, UMS_Data::ListCommands& lsCommand);

/**
 * \brief Helper function to display an option value
 * \param os: The output stream in which the option value will be printed 
 * \param optionValue: The option value to display
 * \return The output stream in which the option value has been printed
 */



std::ostream&
operator<<(std::ostream& os,const UMS_Data::OptionValue_ptr& optionValue);

/**
 * \brief Helper function to display a list of options
 * \param os: The output stream in which the list will be printed 
 * \param lsOptions: The list to display
 * \return The output stream in which the list of options has been printed
 */


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListOptionsValues& lsOptions);

/**
 * \brief Helper function to display a user
 * \param os: The output stream in which the user will be printed 
 * \param user: The user to display
 * \return The output stream in which the user has been printed
 */

std::ostream&
operator<<(std::ostream& os, const UMS_Data::User_ptr& user);

/**
 * \brief Helper function to display a list of users
 * \param os: The output stream in which the list will be printed 
 * \param lsUsers: The list to display
 * \return The output stream in which the list of users has been printed
 */

std::ostream&
operator<<(std::ostream& os, UMS_Data::ListUsers& lsUsers);



#endif
