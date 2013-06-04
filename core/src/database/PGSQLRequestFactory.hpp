/**
 * \file PGSQLRequestFactory.hpp
 * \brief This file implements the request factory for mysql
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 04/06/13
 */

#ifndef _PGSQLREQUESTFACTORY_H_
#define _PGSQLREQUESTFACTORY_H_

#include "RequestFactory.hpp"
#include <string>

class PGSQLRequestFactory : RequestFactory {
public:
  PGSQLRequestFactory();

  ~PGSQLRequestFactory();

  virtual std::string
  get(const int key);

};

#endif // PGSQLREQUESTFACTORY

