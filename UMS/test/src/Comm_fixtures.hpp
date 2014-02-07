/**
 * Comm_fixtures.hpp
 *
 * Author : kevin.coulomb@sysfera.com
 */
#ifndef _COMM_FIXTURES_HPP_
#define _COMM_FIXTURES_HPP_



#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/all.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "FileParser.hpp"
#include "api_ums.hpp"
#include "api_comm.hpp"

namespace ba = boost::assign;
namespace bf = boost::filesystem;
namespace bp = boost::process;
namespace bs = boost::system;


class CommFixture {

public:
  UMS_Data::Configuration mconf ;
  std::string mums_version;
  std::string mtms_version;
  std::string mfms_version;
  std::string mims_version;
  std::string mdis_version;
  std::string mmid;


  CommFixture() {
    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    int argc = 2;
    char* argv[argc];
    const std::string vishnuClientTestConfigPath = getenv("VISHNU_CLIENT_TEST_CONFIG_FILE");
    BOOST_REQUIRE( vishnuClientTestConfigPath.size() !=0  );
    argv[0] = const_cast<char*>("./automTest");
    argv[1] = const_cast<char*>(vishnuClientTestConfigPath.c_str());

    if (vishnu::vishnuInitialize(argv[1], argc, argv)) {
      BOOST_TEST_MESSAGE( "Error in diet_initialize... (using config: "
      << vishnuClientTestConfigPath << ")" );
    }

    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );
    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: LOADING SETUP ==" );
    std::string vishnuTestSetupPath = getenv("VISHNU_TEST_SETUP_FILE");
    BOOST_REQUIRE(vishnuTestSetupPath.size() !=0  );
    FileParser fileparser(vishnuTestSetupPath.c_str());
    std::map<std::string, std::string> setupConfig = fileparser.getConfiguration();
    mums_version = setupConfig.find("TEST_UMS_VERSION")->second ;
    mfms_version = setupConfig.find("TEST_FMS_VERSION")->second ;
    mtms_version = setupConfig.find("TEST_TMS_VERSION")->second ;
    mims_version = setupConfig.find("TEST_IMS_VERSION")->second ;
    mdis_version = setupConfig.find("TEST_DIS_VERSION")->second ;
    mmid = setupConfig.find("TEST_VISHNU_MACHINEID1")->second ;
  }


  ~CommFixture() {

    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: UMSSeDFixture ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: UMSSeDFixture ==" );
  }
};



#endif  // _COMM_FIXTURES_HPP_
