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


//Test category 1

// T1.1 : submit a job


BOOST_FIXTURE_TEST_SUITE(submit_a_job, TMSSeDFixture)


  // submit a job: normal call

BOOST_AUTO_TEST_CASE(submit_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal job submission corresponding to use case T1.1");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath= TMSSCRIPTPATH;
    Job jobInfo;
    SubmitOptions subOptions;

    BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    // To check the success of submitJob function

    ListJobs lsJobs;
    ListJobsOptions lsOptions;
    lsOptions.setJobId(jobInfo.getJobId());
    BOOST_REQUIRE(listJobs(sessionKey, machineId,lsJobs,lsOptions)==0  );

    BOOST_REQUIRE( (lsJobs.getJobs().size()==1) && ( *(lsJobs.getJobs().get(0)) == jobInfo )  );

    BOOST_TEST_MESSAGE("***********************  submit a job: normal call   ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------------------

// submit a job: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing bad session Key for job submission (use case T1.1)");


  // set the machine ID
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTPATH;
  Job jobInfo;
  SubmitOptions subOptions;
  BOOST_CHECK_THROW(submitJob("bad sessionKey", machineId, scriptFilePath, jobInfo,subOptions) ,VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad sessionKey    ok!!!!*****************************");


}

//---------------------------------------------------------------------------

// submit a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE("Testing bad machine identifier for job submission (use case T1.1)");

  VishnuConnexion vc("root","vishnu_user");

  // get the session key

  string sessionKey=vc.getConnexion();


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTPATH;
  Job jobInfo;
  SubmitOptions subOptions;
  BOOST_CHECK_THROW( submitJob(sessionKey, "bad machineId", scriptFilePath, jobInfo,subOptions),VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad machine ID ok!!!!*****************************");


}


// submit a job: bad parameters: bad script content

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_script_content)
{
  BOOST_TEST_MESSAGE("Testing bad script content for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters
  std::string scriptFilePath;
  if(BATCHTYPE=="TORQUE") {
    scriptFilePath=TMSCONFIGDIR "/bad_torque_script";
  } else if(BATCHTYPE=="SLURM") {
    scriptFilePath=TMSCONFIGDIR "/bad_slurm_script";
  } else {
    scriptFilePath=TMSCONFIGDIR "/bad_unknow_batch_type_script";
  }

  Job jobInfo;
  SubmitOptions options;

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad script content  ok!!!!*****************************");


}
// submit a job: bad parameters: bad script path

BOOST_AUTO_TEST_CASE(submit_a_Job_bad_script_path)
{
  BOOST_TEST_MESSAGE("Testing bad script path for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/";
  Job jobInfo;
  SubmitOptions subOptions;

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),VishnuException );

}
BOOST_AUTO_TEST_SUITE_END()




//-----------------------------------------------------------------------------------------

