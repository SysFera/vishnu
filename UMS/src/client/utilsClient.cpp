/**
 * \file utilsClient.cpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "utilsClient.hpp"

#include <algorithm>                    // for min, fill
#include <boost/regex.hpp>
#include <vector>                       // for vector

#include "UMSVishnuException.hpp"       // for UMSVishnuException, etc
#include "constants.hpp"

/**
 * \brief Function to check if a text is empty
 * \param text The text to check
 * \param comMsg The complementary message to print
 * \param exceptionType The type of exception to raise
 * \return raises an exception on error
 */
void
checkIfTextIsEmpty(const std::string& text,
                   const std::string& compMsg,
                   const int& exceptionType) {
  if (text.empty()) {
    throw UMSVishnuException(exceptionType, compMsg);
  }
}

/**
 * \brief Function to check if an email is valid
 * \param mail The email to check
 * \return raises an exception on error
 */
void
checkEmail(const std::string& mail) {
  // Regex taken from http://www.regular-expressions.info/email.html
  boost::regex reg("^[[:alnum:]._%+-]+@[[:alnum:]-]+(?:\\.[[:alnum:]]+)*");

  if (!boost::regex_match(mail, reg)) {
    throw UMSVishnuException(ERRCODE_INVALID_MAIL_ADRESS);
  }
}

/**
 * @brief checkPrivilege: Check if a privilege is valid
 * @param privilege : the privilege value
 */
void
checkPrivilege(int privilege)
{
  if (privilege != vishnu::PRIVILEGE_USER && privilege != vishnu::PRIVILEGE_ADMIN) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM,
                             "Invalid Privilege value: valid options are 0 or 1");
  }
}


/**
 * @brief Function to check the vaidity of a component status
 * @param state The value
 * \return raises an exception on error
 */
void
checkEntityStatus(int state)
{
  if (state != vishnu::STATUS_LOCKED && state != vishnu::STATUS_ACTIVE) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM,
                             "Invalid state: valid options are 0 or 1");
  }
}
