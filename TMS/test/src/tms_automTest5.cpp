/**
 * \file tms_automTest
 * \brief Contains TMS API test implementation
 * References: VISHNU_D4_1b_TMS-PlanTests
 */


//UMS forward Headers
#include "UMS_Data_forward.hpp"

// TMS forward Headers
#include "TMS_Data_forward.hpp"
#include "TMS_fixtures.hpp"
#include "tmsTestUtils.hpp"
#include "TMS_testconfig.h"

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

// namespaces declaration and  aliases
using namespace std;
using namespace UMS_Data;
using namespace TMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;


// The database, UMS and TMS SeD are launched by TMSSedFixture.


// T2.5 : get Jobs progression
BOOST_FIXTURE_TEST_SUITE(get_Jobs_progression, TMSSeDFixture)


  //get Jobs progression : normal call


BOOST_AUTO_TEST_CASE(get_jobs_progression_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the get jobs progression function corresponding to use case T2.5" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSFASTSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );


    ListProgression listOfProgress;

    ProgressOptions pgOptions;

    pgOptions.setJobId(jobInfo.getJobId());

    BOOST_CHECK_EQUAL( getJobProgress(sessionKey,machineId,listOfProgress, pgOptions),0);

    // Check the success of the get jobs progression function

    BOOST_CHECK( listOfProgress.getProgress().get(0)->getJobId()==jobInfo.getJobId());

    BOOST_TEST_MESSAGE( "listOfProgress.getProgress().get(0)->getPercent():" <<listOfProgress.getProgress().get(0)->getPercent());

    Job job;


    // ensure that the job is terminated

    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);

    while (4!=job.getStatus()){

      bpt::seconds sleepTime(1);
      boost::this_thread::sleep(sleepTime);

      getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    }

    ListProgression jobRunningProgress;

    bpt::seconds sleepTime(2);
    boost::this_thread::sleep(sleepTime);
    BOOST_CHECK_EQUAL( getJobProgress(sessionKey,machineId,jobRunningProgress, pgOptions),0);

    // Check the success of the get jobs progression function
    BOOST_CHECK( (jobRunningProgress.getProgress().get(0))->getPercent() > 0);

    BOOST_TEST_MESSAGE( "jobRunningProgress.getProgress().get(0)->getPercent():" << jobRunningProgress.getProgress().get(0)->getPercent());

    BOOST_TEST_MESSAGE("*********************** get jobs progression: normal call ok!!!!*****************************");

    //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


//------------------------------------------------------------------------------------------------------------------------
//  get Jobs progression: bad parameters : bad sessionKey

BOOST_AUTO_TEST_CASE(get_jobs_progression_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing bad sessionKey for the get jobs progression function corresponding to use case T2.5" );

  string machineId="machine_1";


  ListProgression listOfProgress;

  ProgressOptions pgOptions;

  BOOST_CHECK_THROW(getJobProgress("bad sessionKey",machineId,listOfProgress, pgOptions),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get jobs progression : bad sessionKey ok!!!!*****************************");
}



//------------------------------------------------------------------------------------------------------------------------

//  get Jobs progression: bad parameters : bad machineId

BOOST_AUTO_TEST_CASE(get_jobs_progression_bad_machineId)
{

  BOOST_TEST_MESSAGE("Testing bad machineId for the get jobs progression function corresponding to use case T2.5" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();


  ListProgression listOfProgress;

  ProgressOptions pgOptions;

  BOOST_CHECK_THROW(getJobProgress(sessionKey,"bad machineId",listOfProgress, pgOptions),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get jobs progression : bad machineId ok!!!!*****************************");

}


BOOST_AUTO_TEST_SUITE_END()


//------------------------------------------------------------------------------------------------------------------------
