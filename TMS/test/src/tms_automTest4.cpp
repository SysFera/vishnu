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


// T1.2 : get Completed Jobs output
BOOST_FIXTURE_TEST_SUITE(get_Completed_Jobs_output, TMSSeDFixture)


//get Completed Jobs output : normal call


BOOST_AUTO_TEST_CASE(get_completed_jobs_output_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the get completed jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;


    try {

      //Setting submitjob parameters

      const std::string scriptFilePath = generateTmpScript(m_test_tms_machines.at(i).batch_name, "fast");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo,options)==0  );

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

      // wait a few seconds and check the success of cancelling job
      Job job;
      getJobInfo(sessionKey, jobInfo.getJobId(), job);

      // ensure that the job is terminated

      while (vishnu::STATE_COMPLETED != job.getStatus()){

        bpt::seconds sleepTime(5);
        boost::this_thread::sleep(sleepTime);

        getJobInfo(sessionKey, jobInfo.getJobId(), job);
      }

      // Check the success of get completed jobs output function

      ListJobResults listOfResults;
      JobOuputOptions outputOpts;
      outputOpts.setMachineId(machineId);
      outputOpts.setOutputDir(m_test_tms_working_dir);

      BOOST_CHECK_EQUAL(getCompletedJobsOutput(sessionKey, listOfResults, outputOpts), 0);

      for (size_t i=0; i< listOfResults.getNbJobs(); ++i){

        bool pathExist=false;

        pathExist=bfs::exists(bfs::path(listOfResults.getResults().get(i)->getOutputPath())) &&  bfs::exists(bfs::path(listOfResults.getResults().get(i)->getErrorPath()));

        BOOST_CHECK( pathExist );

        std::string outputPath = bfs::basename(listOfResults.getResults().get(i)->getOutputPath());
        std::string errorPath = bfs::basename(listOfResults.getResults().get(i)->getErrorPath());
        if (pathExist){
          bfs::remove (bfs::path(m_test_tms_working_dir + "/" + outputPath));
          bfs::remove (bfs::path(m_test_tms_working_dir + "/" + errorPath));
        }
      }

      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);

      BOOST_TEST_MESSAGE("*********************** get completed jobs output : normal call ok!!!!*****************************");
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}



//------------------------------------------------------------------------------------

//get Completed Jobs output : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(get_completed_job_output_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing bad sessionKey for the get jobs output function corresponding to use case T2.6" );


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

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );


      ListJobResults listOfResults;
      JobOuputOptions outputOpts;
      outputOpts.setMachineId(machineId);
      outputOpts.setOutputDir(m_test_tms_working_dir);

      BOOST_CHECK_THROW(getCompletedJobsOutput("bad sessionKey", listOfResults, outputOpts), VishnuException);

      BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad sessionKey ok!!!!*****************************");

      //  Clean up: delete the submitted job
      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getJobId());
      BOOST_REQUIRE(cancelJob(sessionKey, cancelOpts) == 0);
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//------------------------------------------------------------------------------------

//get Completed Jobs output : bad parameters: bad machineId

BOOST_AUTO_TEST_CASE(get_completed_job_output_bad_machineId)
{

  BOOST_TEST_MESSAGE("Testing normal bad machineId for the get completed jobs output function corresponding to use case T2.6" );


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

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo,options) == 0);

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );


      ListJobResults listOfResults;
      JobOuputOptions outputOpts;
      outputOpts.setOutputDir(m_test_tms_working_dir);
      outputOpts.setMachineId("bad machineId");

      BOOST_CHECK_THROW(getCompletedJobsOutput(sessionKey, listOfResults, outputOpts), VishnuException);

      BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad machineId ok!!!!*****************************");

      //  Clean up: delete the submitted job
      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getJobId());
      BOOST_REQUIRE(cancelJob(sessionKey, cancelOpts) == 0);
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}


BOOST_AUTO_TEST_SUITE_END()
