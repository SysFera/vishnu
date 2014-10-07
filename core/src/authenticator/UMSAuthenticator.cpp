/**
 * \file UMSAuthenticator.cpp
 * \brief This file implements the UMSauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include "UMSAuthenticator.hpp"

#include <string>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include "DatabaseResult.hpp"
#include "DbFactory.hpp"
#include "utilVishnu.hpp"

UMSAuthenticator::UMSAuthenticator(){
}

UMSAuthenticator::~UMSAuthenticator(){
}

bool
UMSAuthenticator::authenticate(UMS_Data::User& user) {
  DbFactory factory;
  Database* databaseVishnu = factory.getDatabaseInstance();

  //To encrypt the clear password
  user.setPassword(vishnu::cryptPassword(user.getUserId(), user.getPassword()));
  std::string sqlCommand = boost::str(boost::format("SELECT userid"
                                                    " FROM users"
                                                    " WHERE userid='%1%'"
                                                    "   AND pwd='%2%'"
                                                    "   AND users.status=%3%"
                                                    )
                                      % databaseVishnu->escapeData(user.getUserId())
                                      % databaseVishnu->escapeData(user.getPassword())
                                      % vishnu::STATUS_ACTIVE);
  boost::scoped_ptr<DatabaseResult> result(databaseVishnu->getResult(sqlCommand));
  return (result->getFirstElement().size() != 0);
}
