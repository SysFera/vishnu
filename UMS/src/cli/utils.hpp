/**
 * \file utils.hpp
 * \brief this file contains helper functions used by the command line interface 
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
 * \param[in]opt describes all options allowed by the command
 * \param[in] signature defines 
 */
void 
helpUsage (const Options& opt,const std::string& signature);

void
errorUsage (const std::string&,const std::string&,const ErrorType& err=PARAMERROR);


void 
checkVishnuConfig(const Options &);


int
usage (const Options &,const std::string&,const std::string&);



std::string 
takePassword(const std::string&);

std::ostream&
operator<<(std::ostream& os, const UMS_Data::Session_ptr& session);


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListSessions& listSession);


std::ostream&
operator<<(std::ostream& os, const UMS_Data::LocalAccount_ptr& account);


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListLocalAccounts& lsLocalAccounts);


std::ostream&
operator<<(std::ostream& os, const UMS_Data::Machine_ptr& machine);


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListMachines& lsMachine);


std::ostream&
operator<<(std::ostream& os, const UMS_Data::Command_ptr& command);


std::ostream& 
operator<<(std::ostream& os, UMS_Data::ListCommands& lsCommand);


std::ostream&
operator<<(std::ostream& os,const UMS_Data::OptionValue_ptr& optionValue);


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListOptionsValues& lsOptions);


std::ostream&
operator<<(std::ostream& os, const UMS_Data::User_ptr& user);


std::ostream&
operator<<(std::ostream& os, UMS_Data::ListUsers& lsUsers);



#endif
