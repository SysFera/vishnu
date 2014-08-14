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


  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      //Setting submitjob parameters

      const std::string scriptFilePath=generateTmpScript(m_test_tms_machines.at(i).batch_name, "wait");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo, options) == 0);

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getId() );

      ListJobs lsJobs;
      ListJobsOptions lsOptions;
      lsOptions.setJobId(jobInfo.getId());
      lsOptions.setMachineId(machineId);
      BOOST_REQUIRE(listJobs(sessionKey, lsJobs, lsOptions)==0  );

      Job job;

      BOOST_CHECK_EQUAL(getJobInfo(sessionKey, jobInfo.getId(), machineId, job), 0  );

    // Check the success of getJobInfo function

      BOOST_CHECK( *(lsJobs.getJobs().get(0))== job) ;

      BOOST_TEST_MESSAGE("***********************  Get a job info: normal call ok!!!!*****************************");

    //  Clean up: delete the submitted job
      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getId());
      BOOST_REQUIRE(cancelJob(sessionKey, cancelOpts)==0  );
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//---------------------------------------------------------------------------

//get job information: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(get_job_information_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for the get job information function (use case T2.1)" );

  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      //Setting submitjob parameters

      const std::string scriptFilePath = generateTmpScript(m_test_tms_machines.at(i).batch_name, "wait");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo,options) == 0 );

      // now let get the job information

      // set the get the job information function parameters

      Job job;

      BOOST_CHECK_THROW(getJobInfo("bad sessionKey ", jobInfo.getId(), machineId, job), VishnuException);

      BOOST_TEST_MESSAGE("***********************  Get a job info: bad sessionKey  ok!!!!*****************************");

    //  Clean up: delete the submitted job

      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getId());
      BOOST_REQUIRE(cancelJob(sessionKey, cancelOpts) == 0);
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);

    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//---------------------------------------------------------------------------

//  get job information: bad parameters: bad job id


BOOST_AUTO_TEST_CASE(get_job_information_bad_JobId)
{
  BOOST_TEST_MESSAGE("Testing bad job identifier for the  get job information function (use case T2.1)" );

  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

  // setting get job information function parameters
    Job job;
    BOOST_CHECK_THROW(getJobInfo(sessionKey, "bad job id", machineId, job),VishnuException );

    BOOST_TEST_MESSAGE("***********************  Get a job info: bad job ID ok!!!!*****************************");
  }

}


BOOST_AUTO_TEST_SUITE_END()
