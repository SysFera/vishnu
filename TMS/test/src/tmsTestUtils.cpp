#include "tmsTestUtils.hpp"
using namespace std;




//addUser();
//changePassword();
//addMachine();
VishnuConnexion::VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co):muid(uid),mupwd(upwd),mco(co){

}

VishnuConnexion::~VishnuConnexion(){
  if(false==msession.getSessionKey().empty()){
  BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
  BOOST_TEST_MESSAGE("The session is closed");
  }
}

string VishnuConnexion::getConnexion(){

  BOOST_REQUIRE_EQUAL(vishnu::connect(muid,mupwd,msession,mco),0);
   BOOST_TEST_MESSAGE("The session is open");
   return msession.getSessionKey();
}





