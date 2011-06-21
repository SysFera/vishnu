/**
 * \file utilsClient.cpp
 * \brief This file contains client utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date February 2011
 */
#include "utilsClient.hpp"
#include "utilVishnu.hpp"

/**
 * \brief Function to check if a text is empty
 * \param text The text to check
 * \param comMsg The complementary message to print
 * \param execpetionType The type of exception to raise
 * \return raises an exception on error
 */
void checkIfTextIsEmpty(const std::string& text, const std::string& compMsg, const int& execpetionType) {

  if(text.size()==0) {
    throw UMSVishnuException(execpetionType, compMsg);
  }

}

/**
 * \brief Function to check if an email is valid
 * \param mail The email to check
 * \return raises an exception on error
 */
void checkEmail(const std::string& mail) {

  if(mail.find("@")==std::string::npos) {
    throw UMSVishnuException(ERRCODE_INVALID_MAIL_ADRESS);
  }

}
