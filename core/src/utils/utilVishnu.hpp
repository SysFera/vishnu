/**
* \file utilVishnu.hpp
* \brief This file presents the utils functions of the vishnu system
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011
*/

#ifndef _UTILVISHNU_H_
#define _UTILVISHNU_H_

#include <iostream>
#include <sstream>

namespace vishnu {

  /**
  * \brief Generic function to convert an object to string
  * \fn    std::string convertToString(const T& val);
  * \param T is a generic data to be transformed to string
  * \return the string version of T
  */
  template <class T>
  std::string convertToString(T val) {
    std::ostringstream out;
    out << val;
    return out.str();
  }

  /**
  * \brief Function to convert a string to int
  * \fn    std::string convertToString(const T& val)
  * \param  val a value to convert to string
  * \return int value of the corresponding string
  */
  int convertToInt(std::string val);

}

#endif // _UTILVISHNU_H_
