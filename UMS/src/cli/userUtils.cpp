/**
 * \file userUtils.cpp
 * \brief This file defines useful functions and types for the VISHNU user commands
 *  command
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 */

#include "userUtils.hpp"
#include <sstream>                      // for basic_stringbuf<>::int_type, etc
#include "Options.hpp"

using namespace std;

/**
 * \brief To build options for the VISHNU user commands
 * \param pgName : The name of the command
 * \param configFile: Represents the VISHNU config file
 * \param fUserId: The user id option callback
 * \param fStatus: The status option callback
 * \param fPrivilege: The privilege option callback
 * \param ffirstname: The user first name option callback
 * \param fLastName: The user last name option callback
 * \param fEmail: The user email option callback
 * \param type: Indicates if it is an add or an update command, 0 means update, and 1 means add
 * \return The description of all options allowed by the command
 */


boost::shared_ptr<Options>
makeUserOptions(std::string pgName,
                std::string& configFile,
                StringcallBackType& fUserId,
                StatusCallBackType& fStatus,
                PrivilegeCallBackType& fPrivilege,
                StringcallBackType& fFirstname,
                StringcallBackType& fLastname,
                StringcallBackType& fEmail,
                int type)
{
  boost::shared_ptr<Options> opt(new Options(pgName));

  opt->add("configFile,c",
           "VISHNU configuration file",
           ENV,
           configFile);

  opt->add("userId,u",
           "The user identifier",
           HIDDEN,
           fUserId,
           1);
  opt->setPosition("userId", 1);

  opt->add("status,s",
           "The user status. 0 => inactive, 1 => active",
           CONFIG,
           fStatus);

  opt->add("firstname,f",
           "The user firstname",
           CONFIG,
           fFirstname,
           type);

  opt->add("lastname,l",
           "The user lastname",
           CONFIG,
           fLastname,
           type);

  opt->add("privilege,p",
           "The user privilege. 1 => admin, 0 => regular user",
           CONFIG,
           fPrivilege,
           type);

  opt->add("email,m",
           "The user email address",
           CONFIG,
           fEmail,
           type);

  return opt;
}
