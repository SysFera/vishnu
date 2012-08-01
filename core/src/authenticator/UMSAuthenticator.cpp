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
  SOCIDatabase* databaseVishnu = factory.getDatabaseInstance();

  //To encrypt the clear password
  user.setPassword(vishnu::cryptPassword(user.getUserId(), user.getPassword()));

  std::string sqlCommand="SELECT numuserid FROM users where userid=:userid and pwd=:pwd";
  SOCISession sess=databaseVishnu->getSingleSession();
  int id;
  sess.execute(sqlCommand).into(id).use(user.getUserId()).use(user.getPassword());
  bool gotId=sess.got_data();
  databaseVishnu->releaseSingleSession(sess);
  return gotId;

}

