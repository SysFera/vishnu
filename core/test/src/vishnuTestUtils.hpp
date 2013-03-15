/**
 * \file vishnuTestUtils.hpp
 * \brief This file contains a few utilities for tests.
 * \author  Benjamin Isnard (benjamin.isnard@sysfera.com)
 * \date May 2011
 */
#ifndef VISHNUTESTUTILS_HPP
#define VISHNUTESTUTILS_HPP

#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "DIET_client.h"
#include "UMSServices.hpp"
#include "utilClient.hpp"

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

/**
 * \brief Function to initialize the database
 * \fn  int restore()
 * \return an error code
 */
int
restore(const std::string& filePath) {
  int READSIZE = 1000;
  diet_profile_t* profile = NULL;
  std::ifstream file(filePath.c_str(), std::ios::in);
  std::string errorInfo;

  if (!file) {
    return -1;
  }

  // While all has not been read
  while (-1 != file.tellg()) {
    char* tmp = new char[READSIZE];
    file.getline(tmp, READSIZE);
    if (strcmp(tmp, "") == 0){
      break;
    }

    profile = diet_profile_alloc(SERVICES_UMS[RESTORE], 0, 0, 1);
    //IN Parameters
    diet_string_set(profile, 0, tmp);
    //OUT Parameters
    diet_string_set(profile, 1);
    delete [] tmp;

    if (!diet_call(profile)) {
      if (diet_string_get(profile, 1, errorInfo)) {
        raiseCommunicationMsgException("VISHNU call failure");
      }
    } else {
      raiseCommunicationMsgException("VISHNU call failure");
    }
    raiseExceptionIfNotEmptyMsg(errorInfo);
  }
  return 0;
}

#endif  // VISHNUTESTUTILS_HPP
