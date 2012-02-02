/**
 * \file UMSAuthenticator.cpp
 * \brief This file implements the UMSauthenticator
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/12/11
 */

#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "UMSAuthenticator.hpp"
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

  std::string sqlCommand = "SELECT numuserid FROM users "
                           "where userid='"+user.getUserId()+"'and pwd='"+user.getPassword()+"'";
  boost::scoped_ptr<DatabaseResult> result(databaseVishnu->getResult(sqlCommand.c_str()));
  return (result->getFirstElement().size() != 0);
}

