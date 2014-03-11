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


// T2.2 : cancel a job

BOOST_FIXTURE_TEST_SUITE(cancel_a_job, TMSSeDFixture)


// cancel a job: normal call

BOOST_AUTO_TEST_CASE(cancel_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal job cancelling corresponding to use case T2.2" );


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

      BOOST_REQUIRE_EQUAL(submitJob(sessionKey, scriptFilePath, jobInfo, options), 0);

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

      // now let cancel the job

      // setting cancel job parameters
      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getJobId());
      BOOST_CHECK_EQUAL(cancelJob(sessionKey, cancelOpts), 0);

      // wait a few seconds and check the success of cancelling job
      bpt::seconds sleepTime(4);
      boost::this_thread::sleep(sleepTime);

      Job job;
      BOOST_REQUIRE(getJobInfo(sessionKey, jobInfo.getJobId(), job) == 0);

      BOOST_TEST_MESSAGE("************ The job status is " << job.getStatus() );

      BOOST_CHECK_EQUAL(job.getStatus(), 6);

      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
      BOOST_TEST_MESSAGE("***********************  cancel a job: normal call  ok!!!!*****************************");

    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//----------------------------------------------------------------------------------------

// cancel a job: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_sessionKey)
{

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
      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo, options) == 0);

      // now let cancel the job

      // setting cancel job parameters


      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getJobId());
      BOOST_CHECK_THROW(cancelJob("bad sessionKey ", cancelOpts), VishnuException);

      BOOST_TEST_MESSAGE("***********************  cancel a job: bad sessionKey   ok!!!!*****************************");

      // Clean up:  Clean up: delete the submitted job
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

// cancel a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE("Testing bad machine identifier for job cancellin (use case T2.2)" );

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

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo, options) == 0);

      // now let try to cancel the job

      CancelOptions cancelopts;
      cancelopts.setMachineId("bad machineId");
      BOOST_CHECK_THROW(cancelJob(sessionKey, cancelopts), VishnuException);

      BOOST_TEST_MESSAGE("***********************  cancel a job: bad machine id  ok!!!!*****************************");

      //  Clean up: delete the submitted job

      cancelopts.setMachineId(machineId);
      BOOST_REQUIRE(cancelJob(sessionKey, cancelopts) == 0);
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }

}


//---------------------------------------------------------------------------



// cancel a job: bad parameters: bad job id



BOOST_AUTO_TEST_CASE(cancel_a_Job_bad_JobId)
{
  BOOST_TEST_MESSAGE("Testing bad job identifier for job cancelling (use case T2.2)" );

  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    CancelOptions cancelopts;
    cancelopts.setMachineId(machineId);
    cancelopts.setJobId("bad job id");
    BOOST_CHECK_THROW(cancelJob(sessionKey, cancelopts), VishnuException);

    BOOST_TEST_MESSAGE("***********************  cancel a job: bad job id  ok!!!!*****************************");
  }
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



  // get the session key and the machine identifier

  VishnuConnexion vc(m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd);
  string adminsessionKey=vc.getConnexion();

  // get another connexion for another user
  VishnuConnexion vc2(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);
  string userSessionKey=vc2.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      //Setting submitjob parameters

      const std::string scriptFilePath = generateTmpScript(m_test_tms_machines.at(i).batch_name, "wait");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      BOOST_REQUIRE(submitJob(adminsessionKey, scriptFilePath, jobInfo, options) == 0);

      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getJobId());
      BOOST_CHECK_THROW(cancelJob(userSessionKey, cancelOpts), VishnuException);

      bfs::path script(scriptFilePath);
      BOOST_CHECK(bfs::remove_all(script)==1);

      BOOST_TEST_MESSAGE("***********************  cancel a job: bad user  id  ok!!!!*****************************");

    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}


//---------------------------------------------------------------------------

// cancel all user jobs as admin

BOOST_AUTO_TEST_CASE(cancel_all_jobs_as_admin)
{
  BOOST_TEST_MESSAGE("Testing if admin can cancel all user jobs" );

  // get a session key for the root user
  VishnuConnexion vc(m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd);
  string adminSessionKey=vc.getConnexion();

  //Cancel all jobs      // Now test the cancellation
  CancelOptions cancelOpts;
  cancelOpts.setUser("all");
  BOOST_CHECK(cancelJob(adminSessionKey, cancelOpts) == 0);

  // get another connexion for a simple user
  VishnuConnexion vc2(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);
  std::string userSessionKey=vc2.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      // Set submitjob parameters
      const std::string scriptFilePath = generateTmpScript(m_test_tms_machines.at(i).batch_name, "wait");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      // Submit a job as admin
      BOOST_REQUIRE(submitJob(adminSessionKey, scriptFilePath, jobInfo, options) == 0);

      // Submit 2 jobs as non admin
      BOOST_REQUIRE(submitJob(userSessionKey, scriptFilePath, jobInfo, options) == 0);
      BOOST_REQUIRE(submitJob(userSessionKey, scriptFilePath, jobInfo, options) == 0);

      TMS_Data::ListJobs myJobsList;
      TMS_Data::ListJobsOptions listJobsOpts;

      listJobs(adminSessionKey, myJobsList, listJobsOpts);

      // Get that all the jobs are running
      BOOST_CHECK(myJobsList.getJobs().size() == 3);

      //Cancel all jobs      // Now test the cancellation
      CancelOptions cancelOpts;
      cancelOpts.setUser("all");
      BOOST_CHECK(cancelJob(adminSessionKey, cancelOpts) == 0);

      //Check that the jobs have been cancelled
      myJobsList.getJobs().clear();
      listJobs(adminSessionKey, myJobsList, listJobsOpts);
      BOOST_CHECK(myJobsList.getJobs().size() == 0);

      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);

      BOOST_TEST_MESSAGE("***********************  cancel all jobs as admin:  ok!!!!*****************************");

    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}



//---------------------------------------------------------------------------

// cancel a given user jobs by admin

BOOST_AUTO_TEST_CASE(cancel_given_user_jobs_by_admin)
{
  BOOST_TEST_MESSAGE("Testing if admin can cancel only a given user jobs" );

  // get a session key for the root user
  VishnuConnexion vc(m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd);
  string adminSessionKey=vc.getConnexion();

  // get another connexion for a simple user
  VishnuConnexion vc2(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);
  std::string userSessionKey=vc2.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      // First clean all jobs on the platform
      CancelOptions cancelOpts;
      cancelOpts.setUser("all");
      BOOST_CHECK(cancelJob(adminSessionKey, cancelOpts) == 0);

      // Set submitjob parameters
      const std::string scriptFilePath = generateTmpScript(m_test_tms_machines.at(i).batch_name, "wait");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      // Submit a job as admin
      BOOST_REQUIRE(submitJob(adminSessionKey, scriptFilePath, jobInfo, options) == 0);

      // Submit 2 jobs as non admin
      BOOST_REQUIRE(submitJob(userSessionKey, scriptFilePath, jobInfo, options) == 0);
      BOOST_REQUIRE(submitJob(userSessionKey, scriptFilePath, jobInfo, options) == 0);


      // Get that all the jobs are running
      TMS_Data::ListJobs myJobsList;
      TMS_Data::ListJobsOptions listJobsOpts;
      listJobs(adminSessionKey, myJobsList, listJobsOpts);

      BOOST_CHECK(myJobsList.getJobs().size() == 3);

      //Cancel the user jobs
      cancelOpts.setUser(m_test_tms_user_vishnu_login);
      BOOST_CHECK(cancelJob(adminSessionKey, cancelOpts) == 0);

      //Check that the jobs have been cancelled
      myJobsList.getJobs().clear();
      listJobs(adminSessionKey, myJobsList, listJobsOpts);
      BOOST_CHECK(myJobsList.getJobs().size() == 1);

      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);

      BOOST_TEST_MESSAGE("***********************  cancel_given_user_jobs_by_admin:  ok!!!!*****************************");

    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//---------------------------------------------------------------------------

// cancel a non-admin can't cancel other user jobs

BOOST_AUTO_TEST_CASE(cancel_non_admin_cant_cancel_other_user_jobs)
{
  BOOST_TEST_MESSAGE("Testing if admin can cancel only a given user jobs" );

  // get a session key for the root user
  VishnuConnexion vc(m_test_ums_root_vishnu_login, m_test_ums_root_vishnu_pwd);
  string adminSessionKey=vc.getConnexion();

  // get another connexion for a simple user
  VishnuConnexion vc2(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);
  std::string userSessionKey=vc2.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    CancelOptions cancelOpts;
    cancelOpts.setUser("all");
    BOOST_CHECK(cancelJob(adminSessionKey, cancelOpts) == 0);

    try {
      // Set submitjob parameters
      const std::string scriptFilePath = generateTmpScript(m_test_tms_machines.at(i).batch_name, "wait");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      // Submit a job as admin
      BOOST_REQUIRE(submitJob(adminSessionKey, scriptFilePath, jobInfo, options) == 0);

      // Submit 2 jobs as non admin
      BOOST_REQUIRE(submitJob(userSessionKey, scriptFilePath, jobInfo, options) == 0);
      BOOST_REQUIRE(submitJob(userSessionKey, scriptFilePath, jobInfo, options) == 0);


      // Get that all the jobs are running
      TMS_Data::ListJobs myJobsList;
      TMS_Data::ListJobsOptions listJobsOpts;
      listJobs(adminSessionKey, myJobsList, listJobsOpts);

      BOOST_CHECK(myJobsList.getJobs().size() == 3);

      //Try to cancel admin jobs
      cancelOpts.setUser(m_test_ums_root_vishnu_login);

      BOOST_CHECK_THROW(cancelJob(userSessionKey, cancelOpts), VishnuException);

      //Check that the jobs have been cancelled
      myJobsList.getJobs().clear();
      listJobs(adminSessionKey, myJobsList, listJobsOpts);

      BOOST_CHECK(myJobsList.getJobs().size() == 3);

      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);

      BOOST_TEST_MESSAGE("*********************** cancel_non_admin_cant_cancel_other_user_jobs:  ok!!!!*****************************");

    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}
BOOST_AUTO_TEST_SUITE_END()
//Test category 2
