/**
 * \file utilsTMSClient.hpp
 * \brief This file contains the TMS utils functions for VISHNU client Proxy classes
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * and Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date February 2011
 */

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "utilsTMSClient.hpp"
#include "utilVishnu.hpp"

/**
 * \brief Function to split the receiving message into error code and message error
 * \fn void raiseTMSExceptionIfNotEmptyMsg(const std::string& msg)
 * \param msg to split
 * \return raises an exception on error
 */
void
TMSUtils::raiseTMSExceptionIfNotEmptyMsg(const std::string& msg) {

  if(msg.size() > 0 ) {
    size_t pos = msg.find('#');
    if(pos!=std::string::npos) {
      std::string codeInString = msg.substr(0,pos);
      if(codeInString.size()!=0) {
        std::istringstream isCode(codeInString);
        int code;
        isCode >> code;
        std::string message = msg.substr(pos+1);

        if (code < 10){
          throw SystemException(code, message);
        } //if the exception is an UMSVishnuException
        else if (code < 100){
          throw UMSVishnuException(code, message);
        }//if the exception is an TMSVishnuException
        else if ((code >= 100) && (code < 200)) {
          throw TMSVishnuException(code, message);
        } else {
          std::string tmp = "Invalid code: ";
          tmp += vishnu::convertToString(code);
          tmp += " with message: ";
          tmp += message;
          throw SystemException(ERRCODE_INVEXCEP, tmp);
        }
      }
    } else {
      throw SystemException(ERRCODE_INVEXCEP, msg);
    }
  }

}

/**
* \brief Simple function to read the content of a regular file
* \param filePath: the path to the file
* \return The content of the file
*/

std::string
TMSUtils::getFileContent(const std::string& filePath) {

  boost::filesystem::path file (filePath);

  if (((false==boost::filesystem::exists(file)) || (true==boost::filesystem::is_empty(file)))
    || (false==boost::filesystem::is_regular_file(file))) {
    throw UserException(ERRCODE_INVALID_PARAM, "can not read the file: " + filePath);
  }

  boost::filesystem::ifstream ifs (file);
  std::stringstream ss;
  ss << ifs.rdbuf();
  return ss.str();

}
