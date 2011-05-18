#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/all.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "imsTestUtils.hpp"

using namespace std;
namespace bp = boost::process;

namespace ba = boost::assign;
namespace bf = boost::filesystem;
namespace bs = boost::system;

VishnuConnexion::VishnuConnexion(const string& uid, const string& upwd, const UMS_Data::ConnectOptions& co):muid(uid),mupwd(upwd),mco(co),open(false){

}

VishnuConnexion::~VishnuConnexion(){
  if(true==open){
    try {
      BOOST_REQUIRE_EQUAL(vishnu::close(msession.getSessionKey()),0);
      BOOST_TEST_MESSAGE("The session is closed");
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

string VishnuConnexion::getConnexion(){
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
}





