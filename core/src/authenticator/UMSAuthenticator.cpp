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

#ifdef USE_SOCI
  std::string sqlCommand="SELECT numuserid FROM users where userid=:userid and pwd=:pwd";
  std::cerr<<sqlCommand<<std::endl;
  SOCISession sess=databaseVishnu->getSingleSession();
  int id;
  sess<<sqlCommand,soci::into(id),soci::use(user.getUserId()),soci::use(user.getPassword());
  bool gotId=sess.got_data();
  databaseVishnu->releaseSingleSession(sess);
  return gotId;

#else
  std::string sqlCommand = "SELECT numuserid FROM users "
                           "where userid='"+user.getUserId()+"'and pwd='"+user.getPassword()+"'";
  boost::scoped_ptr<DatabaseResult> result(databaseVishnu->getResult(sqlCommand.c_str()));


  return (result->getFirstElement().size() != 0);
#endif
}

