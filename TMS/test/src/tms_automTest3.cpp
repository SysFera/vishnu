/**
 * \file tms_automTest
 * \brief Contains TMS API test implementation
 * References: VISHNU_D4_1b_TMS-PlanTests
 */


//UMS forward Headers
#include "UMS_Data_forward.hpp"
#include "constants.hpp"

// TMS forward Headers
#include "TMS_Data_forward.hpp"
#include "TMS_fixtures.hpp"
#include "tmsTestUtils.hpp"
#include "utilVishnu.hpp"

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


// T2.6 : get jobs output

BOOST_FIXTURE_TEST_SUITE(get_job_output, TMSSeDFixture)


//  get jobs output: normal call


BOOST_AUTO_TEST_CASE(get_job_output_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the get jobs output function corresponding to use case T2.6" );


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

      BOOST_TEST_MESSAGE("************ The Script is " << scriptFilePath );

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo, options) == 0);

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getId() );

      // wait a few seconds and check the success of cancelling job
      Job job;

      getJobInfo(sessionKey, jobInfo.getId(), machineId, job);

      // ensure that the job is terminated

      while (vishnu::STATE_COMPLETED!=job.getStatus()){
        bpt::seconds sleepTime(5);
        boost::this_thread::sleep(sleepTime);
        getJobInfo(sessionKey, jobInfo.getId(), machineId, job);
      }

      // Check the success of get jobs output function
      JobResult outputInfos;
      std::string workingdir(m_test_tms_working_dir);
      JobOutputOptions outputOpts;
      outputOpts.setOutputDir(workingdir);

      BOOST_CHECK_EQUAL(getJobOutput(sessionKey, job.getId(), outputInfos, outputOpts), 0);

      bool pathExist=bfs::exists(bfs::path(outputInfos.getErrorPath())) &&  bfs::exists(bfs::path(outputInfos.getOutputPath())) ;

      BOOST_CHECK( pathExist );

      if (pathExist){
        bfs::remove (bfs::path(outputInfos.getOutputPath()));
        bfs::remove (bfs::path(outputInfos.getErrorPath()));
      }
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
      BOOST_TEST_MESSAGE("*********************** get jobs output : normal call ok!!!!*****************************");
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------

//  get jobs output: normal call using $VISHNU_OUTPUTDIR in the script

BOOST_AUTO_TEST_CASE(get_job_output_normal_call_with_outputdir)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the get jobs output function corresponding to use case T2.6 with the output directory" );


  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {
    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      //Setting submitjob parameters

      const std::string scriptFilePath=generateTmpScript(m_test_tms_machines.at(i).batch_name, "output");
      Job jobInfo;
      SubmitOptions options;

      BOOST_TEST_MESSAGE("************ The Script is " << scriptFilePath );

      options.setMachine(machineId);
      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo, options)==0  );

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getId() );

      // wait a few seconds and check the success of cancelling job
      Job job;

      getJobInfo(sessionKey, jobInfo.getId(), machineId, job);

      // ensure that the job is terminated

      while (5!=job.getStatus()){

        bpt::seconds sleepTime(5);
        boost::this_thread::sleep(sleepTime);

        getJobInfo(sessionKey, jobInfo.getId(), machineId, job);
      }

      // Check the success of get jobs output function
      JobResult outputInfos;
      std::string workingdir(m_test_tms_working_dir);

      JobOutputOptions outputOpts;
      outputOpts.setOutputDir(workingdir);
      BOOST_CHECK_EQUAL(getJobOutput(sessionKey, jobInfo.getId(), outputInfos, outputOpts), 0);

      bool pathExist=bfs::exists(bfs::path(outputInfos.getOutputPath())) &&  bfs::exists(bfs::path(outputInfos.getErrorPath())) && bfs::is_directory(bfs::path(outputInfos.getOutputDir()));

      BOOST_CHECK(pathExist);

      std::string filepath =  workingdir+"/"+bfs::basename(jobInfo.getOutputDir())+"/TMS_res";
      BOOST_MESSAGE(filepath);
      bool fileexist = bfs::exists(bfs::path(filepath));
      BOOST_CHECK( fileexist );

      bool correctpath = (bfs::path(outputInfos.getOutputPath()).parent_path().string().compare(bfs::path(outputInfos.getOutputPath()).parent_path().string()) == 0);
      correctpath = correctpath && (bfs::path(outputInfos.getOutputPath()).parent_path().string().compare(outputInfos.getOutputDir())== 0);

      BOOST_CHECK( correctpath );

      if (pathExist){
        bfs::remove (bfs::path(outputInfos.getOutputPath()));
        bfs::remove (bfs::path(outputInfos.getErrorPath()));
        bfs::remove (bfs::path(filepath));
      }
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
      BOOST_TEST_MESSAGE("*********************** get jobs output with outputdir: normal call ok!!!!*****************************");
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//------------------------------------------------------------------------------------------------------------------------

// get job out : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(get_job_output_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing normal bad sessionKey for the get jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;

    try {
      //Setting submitjob parameters

      const std::string scriptFilePath=generateTmpScript(m_test_tms_machines.at(i).batch_name, "fast");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo,options)==0  );

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getId() );

      JobResult outputInfos;
      JobOutputOptions outputOpts;
      outputOpts.setOutputDir(m_test_tms_working_dir);

      BOOST_CHECK_THROW(getJobOutput("bad sessionKey", jobInfo.getId(), outputInfos, outputOpts), VishnuException);

      BOOST_TEST_MESSAGE("*********************** get jobs output : normal call ok!!!!*****************************");

      //  Clean up: delete the submitted job
      CancelOptions cancelOpts;
      cancelOpts.setJobId(jobInfo.getId());
      BOOST_REQUIRE(cancelJob(sessionKey, cancelOpts) == 0);
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script) == 1);
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}

//------------------------------------------------------------------------------------

// get job out : try to get the output of non terminated job

BOOST_AUTO_TEST_CASE(get_job_output_unterminated)
{

  BOOST_TEST_MESSAGE("Testing an unterminated  job output  corresponding to use case T2.6" );


  VishnuConnexion vc(m_test_tms_user_vishnu_login, m_test_tms_user_vishnu_pwd);

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  for(int i = 0; i < m_test_tms_machines.size();++i)
  {

    std::string machineId= m_test_tms_machines.at(i).machine_id;


    try {
      //Setting submitjob parameters

      const std::string scriptFilePath=generateTmpScript(m_test_tms_machines.at(i).batch_name, "fast");
      Job jobInfo;
      SubmitOptions options;
      options.setMachine(machineId);

      BOOST_REQUIRE(submitJob(sessionKey, scriptFilePath, jobInfo, options)==0  );

      BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getId() );

      JobResult outputInfos;
      JobOutputOptions outputOpts;
      outputOpts.setOutputDir(m_test_tms_working_dir);

      BOOST_CHECK_THROW(getJobOutput(sessionKey, jobInfo.getId(), outputInfos, outputOpts),VishnuException  );

      BOOST_TEST_MESSAGE("*********************** get jobs output : unterminated ok!!!!*****************************");

      //  Clean up: delete the submitted job
      CancelOptions cancelOpts;
      cancelOpts.setJobId( jobInfo.getId());
      BOOST_REQUIRE(cancelJob(sessionKey, cancelOpts)==0  );
      bfs::path script(scriptFilePath.c_str());
      BOOST_CHECK(bfs::remove_all(script)==1);
    } catch (VishnuException& e) {
      BOOST_MESSAGE(e.what());
      BOOST_CHECK(false);
    }
  }
}


BOOST_AUTO_TEST_SUITE_END()
