/**
* \file utilVishnu.cpp
* \brief This file implements the utils functions of the vishnu system
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011
*/

#include "utilVishnu.hpp"

/**
* \brief Function to convert a string to int
* \fn    std::string convertToString(const T& val);
* \param  val a value to convert to string
* \return int value of the corresponding string
*/
int
vishnu::convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val);
  str >> intValue;
  return intValue;
}

std::string 
vishnu::cryptPassword(const std::string& salt, const std::string password) {
 
  std::string saltTmp="$6$"+salt+"$";
  std::string encryptedPassword=crypt(password.c_str(),saltTmp.c_str());

 return encryptedPassword.substr(saltTmp.size()); 
}
