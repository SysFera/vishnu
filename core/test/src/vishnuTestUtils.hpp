#ifndef VISHNUTESTUTILS_HPP
#define VISHNUTESTUTILS_HPP

#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"

class VishnuConnection {

public:
  VishnuConnection(const std::string& uid,
                   const std::string& upwd,
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

  std::string getSessionKey() {
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

template<int FILESIZE>
int createFile(const std::string& filePath) {
  static const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE + 1];
  for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = (char)(i%8 + 'a');
  }
  buffer[BUFFER_SIZE] = '\0';
  std::ofstream outFile(filePath.c_str());
  for (unsigned int i = 0; i < FILESIZE; i++) {
    outFile << buffer;
  }
  outFile.close();

  return 0;
}

#endif
