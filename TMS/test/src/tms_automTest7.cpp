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
// T2.1 : get job information

BOOST_FIXTURE_TEST_SUITE(get_job_information, TMSSeDFixture)



  //  get job information: normal call

BOOST_AUTO_TEST_CASE(get_job_information_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  get job information function corresponding to use case T2.1" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    lsOptions.setJobId(jobInfo.getJobId());
    BOOST_REQUIRE(listJobs(sessionKey, machineId,lsJobs,lsOptions)==0  );

    Job job;

    BOOST_CHECK_EQUAL(getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job),0  );

  // Check the success of getJobInfo function

    BOOST_CHECK( *(lsJobs.getJobs().get(0))== job) ;

    BOOST_TEST_MESSAGE("***********************  Get a job info: normal call ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//---------------------------------------------------------------------------

//get job information: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(get_job_information_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for the get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)== 0 );

    // now let get the job information

    // set the get the job information function parameters

    Job job;

    BOOST_CHECK_THROW(getJobInfo("bad sessionKey ", machineId, jobInfo.getJobId(), job),VishnuException );

    BOOST_TEST_MESSAGE("***********************  Get a job info: bad sessionKey  ok!!!!*****************************");

  //  Clean up: delete the submitted job

    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//---------------------------------------------------------------------------


// get job information: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE(get_job_information_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for the get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0 );

    // setting get job information function parameters
    Job job;
    BOOST_CHECK_THROW(getJobInfo(sessionKey, "bad machineId", jobInfo.getJobId(),job),VishnuException );

    BOOST_TEST_MESSAGE("***********************  Get a job info: bad machine ID ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


//---------------------------------------------------------------------------

//  get job information: bad parameters: bad job id


BOOST_AUTO_TEST_CASE(get_job_information_bad_JobId)
{
  BOOST_TEST_MESSAGE("Testing bad job identifier for the  get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  // setting get job information function parameters
  Job job;
  BOOST_CHECK_THROW(getJobInfo(sessionKey,machineId, "bad job id",job),VishnuException );

  BOOST_TEST_MESSAGE("***********************  Get a job info: bad job ID ok!!!!*****************************");

}


BOOST_AUTO_TEST_SUITE_END()

