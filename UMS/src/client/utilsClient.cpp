/**
 * \file utilsClient.cpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "utilsClient.hpp"
#include <boost/regex.hpp>
#include "utilVishnu.hpp"

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
  boost::regex reg("^[A-Z0-9._%+-]+@(?:[A-Z0-9-]+\\.)+[A-Z]{2,6}?", boost::regex::icase);

  if (!boost::regex_match(mail, reg)) {
    throw UMSVishnuException(ERRCODE_INVALID_MAIL_ADRESS);
  }
}
