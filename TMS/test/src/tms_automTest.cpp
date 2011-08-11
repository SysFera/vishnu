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


BOOST_GLOBAL_FIXTURE(TMSSeDFixture)



//Test category 1

// T1.1 : submit a job


BOOST_AUTO_TEST_SUITE(submit_a_job)


  // submit a job: normal call

BOOST_AUTO_TEST_CASE( submit_a_Job_normal_call)
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

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_sessionKey)
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

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_machineId)
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

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script_content)
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

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script_path)
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


// T2.2 : cancel a job

BOOST_AUTO_TEST_SUITE(cancel_a_job)

  // cancel a job: normal call

BOOST_AUTO_TEST_CASE( cancel_a_Job_normal_call)
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

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_sessionKey)
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

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_machineId)
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



BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_JobId)
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

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_userId)
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



//-----------------------------------------------------------------------------------------




//==============================================================================================



//Test category 2



// T2.1 : get job information

BOOST_AUTO_TEST_SUITE( get_job_information)


  //  get job information: normal call

BOOST_AUTO_TEST_CASE( get_job_information_normal_call)
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

BOOST_AUTO_TEST_CASE( get_job_information_bad_sessionKey)
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

BOOST_AUTO_TEST_CASE( get_job_information_bad_machineId)
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


BOOST_AUTO_TEST_CASE( get_job_information_bad_JobId)
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

//------------------------------------------------------------------------------------------------------------------------

// T2.4 : list jobs


BOOST_AUTO_TEST_SUITE( list_jobs)


  //  list job : normal call

BOOST_AUTO_TEST_CASE( list_job_normal_call)
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

BOOST_AUTO_TEST_CASE( list_job_bad_sessionKey)
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

BOOST_AUTO_TEST_CASE( list_job_bad_machineId)
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



//------------------------------------------------------------------------------------------------------------------------

// T2.6 : get jobs output

BOOST_AUTO_TEST_SUITE( get_job_output)


//  get jobs output: normal call


BOOST_AUTO_TEST_CASE( get_job_output_normal_call)
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

    bool pathExist=bfs::exists(bfs::path(outputInfos.getOutputPath())) &&  bfs::exists(bfs::path(outputInfos.getOutputPath()));

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

// get job out : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE( get_job_output_bad_sessionKey)
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


BOOST_AUTO_TEST_CASE( get_job_output_bad_machineId)
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

BOOST_AUTO_TEST_CASE( get_job_output_unterminated)
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





//------------------------------------------------------------------------------------------------------------------------
// T1.2 : get Completed Jobs output

BOOST_AUTO_TEST_SUITE( get_Completed_Jobs_output )

  //get Completed Jobs output : normal call


BOOST_AUTO_TEST_CASE( get_completed_jobs_output_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the get completed jobs output function corresponding to use case T2.6" );


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

    // wait a few seconds and check the success of cancelling job
    Job job;

    getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);

  // ensure that the job is terminated

    while (5!=job.getStatus()){

      bpt::seconds sleepTime(5);
      boost::this_thread::sleep(sleepTime);

      getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job);
    }

    // Check the success of get completed jobs output function

    ListJobResults listOfResults;

    BOOST_CHECK_EQUAL(getCompletedJobsOutput(sessionKey,machineId,listOfResults, TMSWORKINGDIR),0 );

    for (size_t i=0; i< listOfResults.getNbJobs(); ++i){
    
      bool pathExist=false;

      pathExist=bfs::exists(bfs::path(listOfResults.getResults().get(i)->getOutputPath())) &&  bfs::exists(bfs::path(listOfResults.getResults().get(i)->getErrorPath()));

      BOOST_CHECK( pathExist );

      if (pathExist){
        bfs::remove (bfs::path(listOfResults.getResults().get(i)->getOutputPath()));
        bfs::remove (bfs::path(listOfResults.getResults().get(i)->getErrorPath() ));
      }
    }

    BOOST_TEST_MESSAGE("*********************** get completed jobs output : normal call ok!!!!*****************************");
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}



//------------------------------------------------------------------------------------

//get Completed Jobs output : bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE( get_completed_job_output_bad_sessionKey)
{

  BOOST_TEST_MESSAGE("Testing bad sessionKey for the get jobs output function corresponding to use case T2.6" );


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


    ListJobResults listOfResults;

    BOOST_CHECK_THROW(getCompletedJobsOutput("bad sessionKey",machineId, listOfResults, TMSWORKINGDIR),VishnuException  );

    BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad sessionKey ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------

//get Completed Jobs output : bad parameters: bad machineId

BOOST_AUTO_TEST_CASE( get_completed_job_output_bad_machineId)
{

  BOOST_TEST_MESSAGE("Testing normal bad machineId for the get completed jobs output function corresponding to use case T2.6" );


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


    ListJobResults listOfResults;

    BOOST_CHECK_THROW(getCompletedJobsOutput(sessionKey,"bad machineId",listOfResults, TMSWORKINGDIR),VishnuException  );

    BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad machineId ok!!!!*****************************");

  //  Clean up: delete the submitted job
    BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_SUITE_END()






//------------------------------------------------------------------------------------------------------------------------
// T2.5 : get Jobs progression

BOOST_AUTO_TEST_SUITE( get_Jobs_progression )


  //get Jobs progression : normal call


BOOST_AUTO_TEST_CASE( get_jobs_progression_normal_call)
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

BOOST_AUTO_TEST_CASE( get_jobs_progression_bad_sessionKey)
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

BOOST_AUTO_TEST_CASE( get_jobs_progression_bad_machineId)
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
// T2.5 :  list job queues

BOOST_AUTO_TEST_SUITE( list_job_queues )


  //list job queues : normal call

BOOST_AUTO_TEST_CASE( list_job_queues_normal_call)
{

  BOOST_TEST_MESSAGE("Testing normal execution of the list job queues function corresponding to use case T2.3" );

  try {

    VishnuConnexion vc("root","vishnu_user");

    // get the session key and the machine identifier

    string sessionKey=vc.getConnexion();

    string machineId="machine_1";
    // create queues
    std::ostringstream createCommand;
    if(BATCHTYPE=="TORQUE") {
      createCommand << "qmgr";
      createCommand << " " << TMSBATCHHOSTNAME << " < " << TMSCONFIGDIR << "/config_queues_test.cfg >/dev/null";
      if(system(createCommand.str().c_str())) {
       throw UserException(ERRCODE_INVALID_PARAM, "TORQUE: error during creation of queues");
      }
    } else if(BATCHTYPE=="SLURM") {
      createCommand << "scontrol create partition=" << "test_queue1";
      if(system(createCommand.str().c_str())) {
        throw UserException(ERRCODE_INVALID_PARAM, "SLURM: error during creation of partition test_queue1");
      }
      createCommand.str("");
      createCommand << "scontrol create partition=" << "test_queue2";
      if(system(createCommand.str().c_str())) {
        throw UserException(ERRCODE_INVALID_PARAM, "SLURM: error during creation of partition test_queue2");
      }
    } else {
      throw UserException(ERRCODE_INVALID_PARAM, "Unknown Batch Type");
    }

    ListQueues listofQueues;

    BOOST_CHECK_EQUAL( listQueues(sessionKey, machineId, listofQueues),0);
    bool success = false;
    string name;

    // check the successfull of list queues function

    //  bpt::seconds sleepTime(120);

    //boost::this_thread::sleep(sleepTime);


    for (int i=0; i< listofQueues.getQueues().size(); ++i){
      name=listofQueues.getQueues().get(i)->getName();
      if (name=="test_queue1" || name == "test_queue2"){
        success =true ;
      }
    }
    BOOST_CHECK(success);
    //Delete the precedent created queues
    // create queues
    ostringstream delCommand;
    if(BATCHTYPE=="TORQUE") {
      delCommand << "qmgr";
      delCommand << " " << TMSBATCHHOSTNAME <<  " < " << TMSCONFIGDIR << "/config_clean_queues.cfg >/dev/null";
      if(system(delCommand.str().c_str())){
        throw UserException(ERRCODE_INVALID_PARAM, "TORQUE: error during deletion of queues");
      }
    } else if(BATCHTYPE=="SLURM") {
      delCommand << "scontrol delete partition=" << "test_queue1";
      if(system(delCommand.str().c_str())){
        throw UserException(ERRCODE_INVALID_PARAM, "SLURM: error during deletion of partition test_queue1");
      }
      delCommand.str("");
      delCommand << "scontrol delete partition=" << "test_queue2";
      if(system(delCommand.str().c_str())){
        throw UserException(ERRCODE_INVALID_PARAM, "SLURM: error during deletion of partition test_queue2");
      }
    }

    BOOST_TEST_MESSAGE("*********************** list job queues: normal call ok!!!!*****************************");
  } catch (VishnuException& e) {
    BOOST_MESSAGE(e.what());
    BOOST_CHECK(false);
  }
}

//------------------------------------------------------------------------------------------------------------------------
//  get Jobs progression: bad parameters : bad sessionKey

BOOST_AUTO_TEST_CASE( list_job_queues_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad sessionKey for the list job queues function corresponding to use case T2.3" );

  string machineId="machine_1";

  ListQueues listofQueues;

  BOOST_CHECK_THROW(listQueues( "bad sessionKey", machineId, listofQueues),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** list job queues : bad sessionKey ok!!!!*****************************");
}


//------------------------------------------------------------------------------------------------------------------------

//  get Jobs progression: bad parameters : bad machineId

BOOST_AUTO_TEST_CASE( list_job_queues_bad_machineId)
{

  BOOST_TEST_MESSAGE(" Testing bad machineId for the list job queues function corresponding to use case T2.3" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  ListQueues listofQueues;

  BOOST_CHECK_THROW(listQueues(sessionKey, "bad machineId", listofQueues),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** list job queues : bad machineId ok!!!!*****************************");
}


BOOST_AUTO_TEST_SUITE_END()


// THE END

