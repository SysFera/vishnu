#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>


// EMF INCLUDE
//#include <ecore.hpp> // Ecore metamodel
//#include <ecorecpp.hpp> // EMF4CPP utils
// Generated code
#include <iostream>
#include <string>
#include <map>
#include "Comm_fixtures.hpp"
#include "api_comm.hpp"
#include"DIET_client.h"
#include "vishnu_version.hpp"

#define UNAVAILABLE "Server unavailable"


BOOST_FIXTURE_TEST_SUITE( version, CommFixture )

using namespace std;
using namespace vishnu;
namespace bfs= boost::filesystem;

BOOST_AUTO_TEST_CASE( dispatcher_version )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing dispatcher get version" );
  {
    BOOST_CHECK  (getServerVersion(result, "dispatcher")==0);
    BOOST_CHECK (result.size() == 1);
    it = result.begin();
    std::string vers = it->second;
    BOOST_MESSAGE("Dispatcher version : " << vers );
    const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
    std::string neo(car);
    BOOST_CHECK (neo == std::string(mdis_version));
  }
}

BOOST_AUTO_TEST_CASE( xms_version )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing UMS get version" );
  {
    if (mums_version!=std::string("NO")){
      BOOST_CHECK  (getServerVersion(result, "xmssed")==0);
      BOOST_CHECK (result.size() == 1);
      it = result.begin();
      std::string vers = it->second;
      boost::algorithm::trim(vers);
      const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
      std::string neo(car);
      BOOST_CHECK (neo == mums_version);
    } else {
      BOOST_CHECK_THROW  (getServerVersion(result, "xmssed"), VishnuException);
    }
  }

}


BOOST_AUTO_TEST_CASE( all_version )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  int cpt = 0;
  BOOST_MESSAGE(" Testing all servers version" );
  {
    BOOST_CHECK  (getServerVersion(result)==0);
    if (mums_version!=std::string("NO")){
      cpt ++;
    }
    if (mtms_version!=std::string("NO")){
      cpt ++;
    }
    if (mfms_version!=std::string("NO")){
      cpt ++;
    }
    if (mdis_version!=std::string("NO")){
      cpt ++;
    }
    BOOST_CHECK (result.size() == cpt);
  }
}

BOOST_AUTO_TEST_CASE( all_list )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  int cpt = 0;
  BOOST_MESSAGE(" Testing list all servers " );
  {
    BOOST_CHECK  (listServers(result)==0);
    if (mums_version!=std::string("NO")){
      cpt ++;
    }
    if (mtms_version!=std::string("NO")){
      cpt ++;
    }
    if (mfms_version!=std::string("NO")){
      cpt ++;
    }
    BOOST_CHECK (result.size() == cpt);
  }

}

BOOST_AUTO_TEST_CASE( machine_list )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  int cpt = 0;
  BOOST_MESSAGE(" Testing list all servers " );
  {
    BOOST_CHECK  (listServers(result, mmid)==0); // Suppose all servers on same machine
    if (mums_version!=std::string("NO")){
      cpt ++;
    }
    if (mtms_version!=std::string("NO")){
      cpt ++;
    }
    if (mfms_version!=std::string("NO")){
      cpt ++;
    }
    BOOST_CHECK (result.size() == cpt);
  }

}



BOOST_AUTO_TEST_CASE( dispatcher_ping )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing dispatcher ping" );
  {
    BOOST_CHECK  (ping(result, "dispatcher")==0);
    BOOST_CHECK (result.size() == 1);
    it = result.begin();
    std::string vers = it->second;
    const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
    std::string neo(car);
    BOOST_CHECK (neo != UNAVAILABLE);
  }
}

BOOST_AUTO_TEST_CASE( ums_ping )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing UMS ping" );
  {
    if (mums_version!=std::string("NO")){
      BOOST_CHECK  (ping(result, "umssed")==0);
      BOOST_CHECK (result.size() == 1);
      it = result.begin();
      std::string vers = it->second;
      boost::algorithm::trim(vers);
      const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
      std::string neo(car);
      BOOST_CHECK (neo != UNAVAILABLE);
    } else {
      BOOST_CHECK_THROW  (ping(result, "umssed"), VishnuException);
    }
  }

}

BOOST_AUTO_TEST_CASE( fms_ping )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing FMS ping" );
  {
    if (mfms_version!=std::string("NO")){
      BOOST_CHECK  (ping(result, "fmssed")==0);
      BOOST_CHECK (result.size() == 1);
      it = result.begin();
      std::string vers = it->second;
      boost::algorithm::trim(vers);
      const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
      std::string neo(car);
      BOOST_CHECK (neo != UNAVAILABLE);
    } else {
      BOOST_CHECK_THROW  (ping(result, "fmssed"), VishnuException);
    }
  }

}

BOOST_AUTO_TEST_CASE( tms_ping )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing TMS ping" );
  {
    if (mtms_version!=std::string("NO")){
      BOOST_CHECK  (ping(result, "tmssed")==0);
      BOOST_CHECK (result.size() == 1);
      it = result.begin();
      std::string vers = it->second;
      boost::algorithm::trim(vers);
      const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
      std::string neo(car);
      BOOST_CHECK (neo != UNAVAILABLE);
    } else {
      BOOST_CHECK_THROW  (ping(result, "tmssed"), VishnuException);
    }
  }

}


BOOST_AUTO_TEST_CASE( all_ping )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  int cpt = 0;
  BOOST_MESSAGE(" Testing all servers version" );
  {
    BOOST_CHECK  (ping(result)==0);
    if (mums_version!=std::string("NO")){
      cpt ++;
    }
    if (mtms_version!=std::string("NO")){
      cpt ++;
    }
    if (mfms_version!=std::string("NO")){
      cpt ++;
    }
    if (mdis_version!=std::string("NO")){
      cpt ++;
    }
    BOOST_CHECK (result.size() == cpt);
  }
}


BOOST_AUTO_TEST_SUITE_END()

