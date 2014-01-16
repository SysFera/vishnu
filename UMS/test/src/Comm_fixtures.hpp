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
  std::string m_test_ums_authen_type;
  std::string m_test_ums_root_vishnu_login;
  std::string m_test_ums_root_vishnu_pwd;
  std::string m_test_ums_admin_vishnu_login;
  std::string m_test_ums_admin_vishnu_pwd;
  std::string m_test_ums_user_vishnu_login;
  std::string m_test_ums_user_vishnu_pwd;
  std::string m_test_ums_user_vishnu_machineid;
  UMS_Data::Configuration mconf ;

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

  }


  ~CommFixture() {

    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: UMSSeDFixture ==" );
    BOOST_TEST_MESSAGE( "== Test teardown [END]: UMSSeDFixture ==" );
  }
};



#endif  // _COMM_FIXTURES_HPP_
