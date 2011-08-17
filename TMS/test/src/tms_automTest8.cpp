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


// T2.2 : cancel a job

BOOST_FIXTURE_TEST_SUITE(cancel_a_job, TMSSeDFixture)


  // cancel a job: normal call

BOOST_AUTO_TEST_CASE(cancel_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal job cancelling corresponding to use case T2.2" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    // now let cancel the job

    // setting cancel job parameters
    BOOST_CHECK_EQUAL(cancelJob(sessionKey, machineId, jobInfo.getJobId()),0  );

    // wait a few seconds and check the success of cancelling job
    bpt::seconds sleepTime(4);
    boost::this_thread::sleep(sleepTime);

    Job job;
    BOOST_REQUIRE(getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job)==0);

    BOOST_TEST_MESSAGE("************ The job status is " << job.getStatus() );

    BOOST_CHECK_EQUAL(job.getStatus(), 6);

    BOOST_TEST_MESSAGE("***********************  cancel a job: normal call  ok!!!!*****************************");

  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }

}

//----------------------------------------------------------------------------------------

// cancel a job: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing bad session Key for job cancelling (use case T2.2)");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ==0 );

    // now let cancel the job

    // setting cancel job parameters


    BOOST_CHECK_THROW(cancelJob("bad sessionKey ", machineId, jobInfo.getJobId()),VishnuException );

    BOOST_TEST_MESSAGE("***********************  cancel a job: bad sessionKey   ok!!!!*****************************");

  // Clean up:  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


//---------------------------------------------------------------------------

// cancel a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE("Testing bad machine identifier for job cancellin (use case T2.2)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ==0 );

    // now let try to cancel the job


    BOOST_CHECK_THROW(cancelJob(sessionKey, "bad machineId", jobInfo.getJobId()),VishnuException );

    BOOST_TEST_MESSAGE("***********************  cancel a job: bad machine id  ok!!!!*****************************");

    //  Clean up: delete the submitted job

    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }

}


//---------------------------------------------------------------------------



// cancel a job: bad parameters: bad job id



BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_JobId)
{
  BOOST_TEST_MESSAGE("Testing bad job identifier for job cancelling (use case T2.2)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  // now let cancel the job


  BOOST_CHECK_THROW(cancelJob(sessionKey,machineId, "bad job id"),VishnuException );

  BOOST_TEST_MESSAGE("***********************  cancel a job: bad job id  ok!!!!*****************************");

  // wait a few seconds and check the success of cancelling job

//  bpt::seconds sleepTime(500);

 // boost::this_thread::sleep(sleepTime);

}





//---------------------------------------------------------------------------

// cancel a job: bad parameters: bad user id

// FIX ME: I need two different UMS Local accounts for root and user_1

BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_userId)
{
  BOOST_TEST_MESSAGE("Testing bad user identifier for job cancelling (use case T2.2)" );

  VishnuConnexion vc1("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc1.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_REQUIRE( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0 );

  // now let cancel the job

  // setting cancel job parameters

  // get another connexion for another user


    VishnuConnexion vc2("user_1","toto");

    // get his session key

    sessionKey=vc2.getConnexion();

    BOOST_CHECK_THROW(cancelJob(sessionKey, machineId, jobInfo.getJobId()),VishnuException  );

    BOOST_TEST_MESSAGE("***********************  cancel a job: bad user  id  ok!!!!*****************************");
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
//Test category 2



