/**
 * UMS_fixtures.hpp
 *
 * Author : bisnard
 */

//#include "diet_fixtures.hpp"
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/all.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>
#include "UMS_testconfig.h"
#include "diet_config_tests.h"
#include "diet_fixtures.hpp"
#include "utils.hpp"

namespace ba = boost::assign;
namespace bf = boost::filesystem;
namespace bp = boost::process;
namespace bs = boost::system;

// must not be static
// should be a primitive type with an identifier name
char UMSSeD[] = "umssed";
char ConfigUMSSeD[] = UMSSEDCONF;
char BinDirUMSSeD[] = UMSSEDBINDIR;

template <const char *name, const char *binDir, const char *config>
class UMSSEDFixtureTemplate : public DispatcherFixture
{
  boost::scoped_ptr<bp::child> UMSSeD;

public:
  UMSSEDFixtureTemplate() {
    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Launching "
                        <<  "umssed" << " ==");

    std::string exec;
    try {
      exec = bp::find_executable_in_path(name, binDir);
    } catch (bs::system_error& e) {
      BOOST_TEST_MESSAGE( "can't find " << UMSSeD << ": "
                          << e.what() );
      BOOST_TEST_MESSAGE( "search path: " << BinDirUMSSeD );
      return;
    }

    BOOST_TEST_MESSAGE( "SeD found: " << exec );

    // setup SeD environment
    bp::context ctx;
    ctx.process_name = name;
    bp::environment::iterator i_c;
    i_c = ctx.env.find(ENV_LIBRARY_PATH_NAME);
    if (i_c != ctx.env.end()) {
      i_c->second = std::string(ENV_LIBRARY_PATH) + i_c->second;
    } else {
      ctx.env[ENV_LIBRARY_PATH_NAME] = ENV_LIBRARY_PATH;
    }
#ifndef DEBUG_TESTS
    // redirect output to /dev/null
    ctx.streams[bp::stdout_id] = bp::behavior::null();
    ctx.streams[bp::stderr_id] = bp::behavior::null();
#endif
    
    // setup SeD arguments
    std::vector<std::string> args = ba::list_of(std::string(config));

    // launch SeD
    const bp::child c = bp::create_child(exec, args, ctx);
    UMSSeD.reset(utils::copy_child(c));
    boost::this_thread::sleep(boost::posix_time::milliseconds(SLEEP_TIME*2));
    BOOST_TEST_MESSAGE( "== Test setup [END]: launching "
                        << name << " ==" );
  }


  ~UMSSEDFixtureTemplate()
  {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Stopping "
                        << name << " ==" );
    if( UMSSeD ) {
      try {
        UMSSeD->terminate();
        
        // FIXME: currently UMSSeD->wait() crashes, we need to set the signal handler of SIGCHLD to SID_DFL
        signal(SIGCHLD, SIG_DFL);
      } catch (...) {
        BOOST_TEST_MESSAGE( "== Problem while stopping "
                            << name << " ==" );
      }
    }
    boost::this_thread::sleep(boost::posix_time::milliseconds(SLEEP_TIME*2));
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Stopping "
                        << name << " ==" );
  }
};

typedef UMSSEDFixtureTemplate<UMSSeD, BinDirUMSSeD, ConfigUMSSeD>  UMSSeDFixture;
