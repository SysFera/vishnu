/**
 * \file MYSQLRequestFactory.hpp
 * \brief This file implements the request factory for mysql
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 04/06/13
 */

#ifndef _MYSQLREQUESTFACTORY_H_
#define _MYSQLREQUESTFACTORY_H_

#include "RequestFactory.hpp"
#include <string>

class MYSQLRequestFactory : RequestFactory {
public:
  MYSQLRequestFactory();

  ~MYSQLRequestFactory();

  virtual std::string
  get(const int key);

};

#endif // MYSQLREQUESTFACTORY

