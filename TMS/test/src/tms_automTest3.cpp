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


// T2.6 : get jobs output

BOOST_FIXTURE_TEST_SUITE(get_job_output, TMSSeDFixture)


//  get jobs output: normal call


BOOST_AUTO_TEST_CASE(get_job_output_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the get jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  try {
    //Setting submitjob parameters

    const std::string scriptFilePath=TMSFASTSCRIPTPATH;
    Job jobInfo;
    SubmitOptions options;

    BOOST_TEST_MESSAGE("************ The Script is " << scriptFilePath );

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

    // wait a few seconds and check the success of cancelling job
    Job job;

    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);

  // ensure that the job is terminated

    while (5!=job.getStatus()){

      bpt::seconds sleepTime(5);
      boost::this_thread::sleep(sleepTime);

      getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    }

    // Check the success of get jobs output function
    JobResult outputInfos;

    BOOST_CHECK_EQUAL(getJobOutput(sessionKey,machineId, jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),0  );

    bool pathExist=bfs::exists(bfs::path(outputInfos.getOutputPath())) &&  bfs::exists(bfs::path(outputInfos.getOutputPath())) ;

    BOOST_CHECK( pathExist );

    if (pathExist){
      bfs::remove (bfs::path(outputInfos.getOutputPath()));
      bfs::remove (bfs::path(outputInfos.getErrorPath()));
    }
    BOOST_TEST_MESSAGE("*********************** get jobs output : normal call ok!!!!*****************************");
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------

//  get jobs output: normal call using $VISHNU_OUTPUTDIR in the script

BOOST_AUTO_TEST_CASE(get_job_output_normal_call_with_outputdir)
{
  
  BOOST_TEST_MESSAGE("Testing normal execution of the get jobs output function corresponding to use case T2.6 with the output directory" );
  
  
  VishnuConnexion vc("root","vishnu_user");
  
  // get the session key and the machine identifier
  
  string sessionKey=vc.getConnexion();
  
  string machineId="machine_1";
  
  try {
    //Setting submitjob parameters
    
    const std::string scriptFilePath=TMSFASTSCRIPTPATHOUTDIR;
    Job jobInfo;
    SubmitOptions options;
    
    BOOST_TEST_MESSAGE("************ The Script is " << scriptFilePath );

    BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );
    
    BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
    
    // wait a few seconds and check the success of cancelling job
    Job job;
    
    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    
    // ensure that the job is terminated
    
    while (5!=job.getStatus()){
      
      bpt::seconds sleepTime(5);
      boost::this_thread::sleep(sleepTime);
      
      getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    }
    
    // Check the success of get jobs output function
    JobResult outputInfos;
    
    BOOST_CHECK_EQUAL(getJobOutput(sessionKey,machineId, jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),0  );
    
    bool pathExist=bfs::exists(bfs::path(outputInfos.getOutputPath())) &&  bfs::exists(bfs::path(outputInfos.getErrorPath())) && bfs::is_directory(bfs::path(outputInfos.getOutputDir()));
    
    BOOST_CHECK( pathExist );
    std::string filepath =  outputInfos.getOutputDir()+"/TMS_res";
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
    BOOST_TEST_MESSAGE("*********************** get jobs output with outputdir: normal call ok!!!!*****************************");
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------

// get job out : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE(get_job_output_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing normal bad sessionKey for the get jobs output function corresponding to use case T2.6" );


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

    JobResult outputInfos;

    BOOST_CHECK_THROW(getJobOutput("bad sessionKey",machineId, jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),VishnuException  );

    BOOST_TEST_MESSAGE("*********************** get jobs output : normal call ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------

// get job out : bad parameters: bad machineId


BOOST_AUTO_TEST_CASE(get_job_output_bad_machineId)
{

  BOOST_TEST_MESSAGE("Testing normal bad machineId for the get jobs output function corresponding to use case T2.6" );


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

    JobResult outputInfos;

    BOOST_CHECK_THROW(getJobOutput(sessionKey,"bad machineId", jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),VishnuException  );

    BOOST_TEST_MESSAGE("*********************** get jobs output : bad machine id ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------

// get job out : try to get the output of non terminated job

BOOST_AUTO_TEST_CASE(get_job_output_unterminated)
{

  BOOST_TEST_MESSAGE("Testing an unterminated  job output  corresponding to use case T2.6" );


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

    JobResult outputInfos;

    BOOST_CHECK_THROW(getJobOutput(sessionKey,machineId, jobInfo.getJobId(),outputInfos, TMSWORKINGDIR),VishnuException  );

    BOOST_TEST_MESSAGE("*********************** get jobs output : unterminated ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()
