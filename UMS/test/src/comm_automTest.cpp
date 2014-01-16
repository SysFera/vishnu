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




BOOST_FIXTURE_TEST_SUITE( version, CommFixture )

using namespace std;
using namespace vishnu;
namespace bfs= boost::filesystem;

BOOST_AUTO_TEST_CASE( dispatcher_version )
{
  std::map<std::string, std::string> result;
  std::map<std::string, std::string>::iterator it;
  BOOST_MESSAGE(" Testing normal connection U1-B1" );
  {
    BOOST_CHECK  (getServerVersion(result, "dispatcher")==0);
    BOOST_CHECK (result.size() == 1);
    it = result.begin();
    std::string vers = it->second;
    BOOST_MESSAGE("Dispatcher version : " << vers );
    const char* car = vers.c_str(); // Use a char* because otherwize a **** invisible character is in the end of the string and fucks comparison
    std::string neo(car);
    BOOST_CHECK (neo == std::string(VISHNU_VERSION)); // Using VISHNU_VERSION -> VERSION OF THIS CLIENT -> NEED DISPATCHER OF SAME VERSION
  }
}
BOOST_AUTO_TEST_SUITE_END()

