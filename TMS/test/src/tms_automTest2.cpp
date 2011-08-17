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


// T2.4 : list jobs

BOOST_FIXTURE_TEST_SUITE(list_jobs, TMSSeDFixture)


  //  list job : normal call

BOOST_AUTO_TEST_CASE(list_job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSSCRIPTPATH;
    Job firstJob;
    SubmitOptions options;

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath,firstJob,options)==0  );

    BOOST_TEST_MESSAGE("************ The first job identifier is " << firstJob.getJobId() );

    // submit a second job

    Job secondJob;
    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath,secondJob,options)==0  );

    BOOST_TEST_MESSAGE("************ The second job identifier is " << secondJob.getJobId() );


    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    //lsOptions.setJobId(jobInfo.getJobId());
    // retrieve the two last submitted jobs
    bool found= false;
    bool foundFisrtJob= false;
    bool foundSecondJob= false;

    BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );


  // Check the success of listJobs function

    int i=0;

    while ( ( false==found ) && ( i < lsJobs.getNbJobs()) ){

      if(firstJob==*(lsJobs.getJobs().get(i))){
      foundFisrtJob= true;
      }


      if(secondJob==*(lsJobs.getJobs().get(i))){
     foundSecondJob=true;
      }

      found= (foundSecondJob && foundSecondJob);

      i++;
    }



    BOOST_CHECK( found ) ;

    BOOST_TEST_MESSAGE("*********************** list a job info: normal call ok!!!!*****************************");

  //  Clean up: delete the submitted jobs
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, firstJob.getJobId())==0  );
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, secondJob.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------


//  list job : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(list_job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  list job  function corresponding to use case T2.4" );


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
  // lsOptions.setJobId(jobInfo.getJobId());

    BOOST_CHECK_THROW(listJobs("bad sessionKey ", machineId,lsJobs,lsOptions),VishnuException  );


    BOOST_TEST_MESSAGE("*********************** list a job info: bad sessionkey ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------


//  list job : bad parameters: bad machineId

BOOST_AUTO_TEST_CASE(list_job_bad_machineId)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the  list job  function corresponding to use case T2.4" );


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

    BOOST_CHECK_THROW(listJobs(sessionKey, "bad machineId",lsJobs,lsOptions),VishnuException );



    BOOST_TEST_MESSAGE("*********************** list a job : bad machine id ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
