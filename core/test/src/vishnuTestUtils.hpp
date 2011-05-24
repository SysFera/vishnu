#ifndef VISHNUTESTUTILS_HPP
#define VISHNUTESTUTILS_HPP

#include <iostream>
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"

using namespace std;

class VishnuConnection {

public:
  VishnuConnection(const string& uid,
                   const string& upwd,
                   const UMS_Data::ConnectOptions& co = UMS_Data::ConnectOptions())
  :muid(uid), mupwd(upwd), mco(co), open(false) {};

  ~VishnuConnection() {
    if(true==open) {
      try {
        BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
        BOOST_TEST_MESSAGE("The session is closed");
      } catch (VishnuException& e) {
        BOOST_MESSAGE(e.what());
        BOOST_CHECK(false);
      }
    }
  };

  string getSessionKey() {
    try {
      BOOST_REQUIRE(vishnu::connect(muid,mupwd,msession,mco)==0);
      open=true;
      BOOST_TEST_MESSAGE("The session is open");
      return msession.getSessionKey();
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
    return "";
  };

private:
  UMS_Data::Session msession;
  UMS_Data::ConnectOptions mco;
  std::string muid;
  std::string mupwd;
  bool open;
};

#endif
