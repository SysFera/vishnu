//#include <ecore.hpp> // Ecore metamodel
//#include <ecorecpp.hpp> // EMF4CPP utils
// Generated code
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "TMS_Data.hpp"
#include "TMS_Data_forward.hpp"
#include <iostream>
#include "TMS_fixtures.hpp"
#include "tmsTestUtils.hpp"
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
using namespace std;
using namespace UMS_Data;
using namespace TMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;

BOOST_GLOBAL_FIXTURE(TMSSeDFixture)

//Test category 1g

// T1.1 : submit a job

BOOST_AUTO_TEST_SUITE(submit_a_job)

const string jobOutputPath = TMSWORKINGDIR "/output.txt" ;
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  // submit a job: normal call

BOOST_AUTO_TEST_CASE( submit_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal job submission corresponding to use case T1.1" << "\n");

 VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath= TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions subOptions;
  subOptions.setOutputPath(jobOutputPath);
  subOptions.setErrorPath(jobErrorPath);

  BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,subOptions),0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  // Check the success of submitJob function
  ListJobs lsJobs;
  ListJobsOptions lsOptions;
  lsOptions.setJobId(jobInfo.getJobId());
  BOOST_REQUIRE(listJobs(sessionKey, machineId,lsJobs,lsOptions)==0  );

  BOOST_REQUIRE( (lsJobs.getJobs().size()==1) && ( *(lsJobs.getJobs().get(0)) == jobInfo )  );

BOOST_TEST_MESSAGE("***********************  submit a job: normal call   ok!!!!*****************************" << "\n ");

// wait a few seconds and check the success of cancelling job

 // bpt::seconds sleepTime(5);

  //boost::this_thread::sleep(sleepTime);


// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}


// submit a job: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for job submission (use case T1.1)" << "\n" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_CHECK_THROW(submitJob("bad sessionKey", machineId, scriptFilePath, jobInfo,options) ,VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad sessionKey    ok!!!!*****************************" << " \n");




}

// submit a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for job submission (use case T1.1)" << "\n" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);


  BOOST_CHECK_THROW( submitJob(sessionKey, "bad machineId", scriptFilePath, jobInfo,options),VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad machine ID ok!!!!*****************************" << " \n");



}


// submit a job: bad parameters: bad script content

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script_content)
{
  BOOST_TEST_MESSAGE(" Testing bad script content for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/bad_torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );

  BOOST_TEST_MESSAGE("***********************  submit a job: bad script content  ok!!!!*****************************" << " \n");


}
// submit a job: bad parameters: bad script path

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script_path)
{
  BOOST_TEST_MESSAGE(" Testing bad script path for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );

}
BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------------------


// T2.2 : cancel a job

BOOST_AUTO_TEST_SUITE(cancel_a_job)

const string jobOutputPath = TMSWORKINGDIR "/output.txt" ;
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  // cancel a job: normal call

BOOST_AUTO_TEST_CASE( cancel_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal job cancelling corresponding to use case T2.2" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  // now let cancel the job

// setting cancel job parameters


  BOOST_CHECK_EQUAL(cancelJob(sessionKey, machineId, jobInfo.getJobId()),0  );

  // wait a few seconds and check the success of cancelling job
  //bpt::seconds sleepTime(5);
  // boost::this_thread::sleep(sleepTime);
  // FIXME replace me by calling a list job function and checking the status
  Job job;
  BOOST_CHECK_THROW(getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job),VishnuException  );

  BOOST_TEST_MESSAGE("***********************  cancel a job: normal call  ok!!!!*****************************" << " \n");


}


// cancel a job: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for job cancelling (use case T2.2)"  );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ==0 );

  // now let cancel the job

  // setting cancel job parameters


  BOOST_CHECK_THROW(cancelJob("bad sessionKey ", machineId, jobInfo.getJobId()),VishnuException );

  BOOST_TEST_MESSAGE("***********************  cancel a job: bad sessionKey   ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

}

// cancel a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for job cancellin (use case T2.2)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ==0 );

  // now let cancel the job

  // setting cancel job parameters

  BOOST_CHECK_THROW(cancelJob(sessionKey, "bad machineId", jobInfo.getJobId()),VishnuException );

  BOOST_TEST_MESSAGE("***********************  cancel a job: bad machine id  ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );


}


// cancel a job: bad parameters: bad job id



BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_JobId)
{
  BOOST_TEST_MESSAGE(" Testing bad job identifier for job cancelling (use case T2.2)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  // now let cancel the job


  BOOST_CHECK_THROW(cancelJob(sessionKey,machineId, "bad job id"),VishnuException );

  BOOST_TEST_MESSAGE("***********************  cancel a job: bad job id  ok!!!!*****************************" << " \n");

  // wait a few seconds and check the success of cancelling job

//  bpt::seconds sleepTime(500);

 // boost::this_thread::sleep(sleepTime);

}





// cancel a job: bad parameters: bad user id

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_userId)
{
  BOOST_TEST_MESSAGE(" Testing bad user identifier for job cancelling (use case T2.2)" );

  VishnuConnexion vc1("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc1.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0 );

  // now let cancel the job

  // setting cancel job parameters

  // get another connexion for another user


  VishnuConnexion vc2("user_1","toto");

  // get the session key

  sessionKey=vc2.getConnexion();

  BOOST_CHECK_THROW(cancelJob(sessionKey, machineId, jobInfo.getJobId()),VishnuException  );

  BOOST_TEST_MESSAGE("***********************  cancel a job: bad user  id  ok!!!!*****************************" << " \n");

}


BOOST_AUTO_TEST_SUITE_END()



//-----------------------------------------------------------------------------------------




//==============================================================================================



//Test category 2



// T2.1 : get job information

BOOST_AUTO_TEST_SUITE( get_job_information)

const string jobOutputPath = TMSWORKINGDIR "/output.txt";
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  //  get job information: normal call

BOOST_AUTO_TEST_CASE( get_job_information_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  get job information function corresponding to use case T2.1" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

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

  BOOST_TEST_MESSAGE("***********************  Get a job info: normal call ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}


//get job information: bad parameters: bad sessionKey

BOOST_AUTO_TEST_CASE( get_job_information_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for the get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)== 0 );

  // now let get the job information

  // set the get the job information function parameters

  Job job;

  BOOST_CHECK_THROW(getJobInfo("bad sessionKey ", machineId, jobInfo.getJobId(), job),VishnuException );

  BOOST_TEST_MESSAGE("***********************  Get a job info: bad sessionKey  ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}

// get job information: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( get_job_information_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for the get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobErrorPath);
  options.setErrorPath(jobOutputPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0 );

  // setting get job information function parameters
  Job job;
  BOOST_CHECK_THROW(getJobInfo(sessionKey, "bad machineId", jobInfo.getJobId(),job),VishnuException );

  BOOST_TEST_MESSAGE("***********************  Get a job info: bad machine ID ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}


//  get job information: bad parameters: bad job id


BOOST_AUTO_TEST_CASE( get_job_information_bad_JobId)
{
  BOOST_TEST_MESSAGE(" Testing bad job identifier for the  get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  // setting get job information function parameters
  Job job;
  BOOST_CHECK_THROW(getJobInfo(sessionKey,machineId, "bad job id",job),VishnuException );

  BOOST_TEST_MESSAGE("***********************  Get a job info: bad job ID ok!!!!*****************************" << " \n");

}


BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------------------------------------------------

// T2.4 : list jobs

BOOST_AUTO_TEST_SUITE( list_jobs)

const string jobOutputPath = TMSWORKINGDIR "/output.txt" ;
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  //  list job : normal call

BOOST_AUTO_TEST_CASE( list_job_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  ListJobs lsJobs;
  ListJobsOptions lsOptions;
  lsOptions.setJobId(jobInfo.getJobId());
  BOOST_CHECK_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );


// Check the success of listJobs function

  BOOST_CHECK(  *(lsJobs.getJobs().get(0))== jobInfo ) ;

  BOOST_TEST_MESSAGE("*********************** list a job info: normal call ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}

BOOST_AUTO_TEST_CASE( list_job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  ListJobs lsJobs;
  ListJobsOptions lsOptions;
 // lsOptions.setJobId(jobInfo.getJobId());

  BOOST_CHECK_THROW(listJobs("bad sessionKey ", machineId,lsJobs,lsOptions),VishnuException  );



  BOOST_TEST_MESSAGE("*********************** list a job info: bad sessionkey ok!!!!*****************************" << " \n");


// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}

BOOST_AUTO_TEST_CASE( list_job_bad_machineId)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the  list job  function corresponding to use case T2.4" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  ListJobs lsJobs;
  ListJobsOptions lsOptions;
  lsOptions.setJobId(jobInfo.getJobId());

  BOOST_CHECK_THROW(listJobs(sessionKey, "bad machineId",lsJobs,lsOptions),VishnuException );



  BOOST_TEST_MESSAGE("*********************** list a job : bad machine id ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}

BOOST_AUTO_TEST_SUITE_END()





//------------------------------------------------------------------------------------------------------------------------
// T2.6 : get jobs output

BOOST_AUTO_TEST_SUITE( get_job_output)

const string jobOutputPath = TMSWORKINGDIR "/output.txt" ;
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  //  list job : normal call

// T2.6 : get jobs output



BOOST_AUTO_TEST_CASE( get_job_output_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the get jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/fast_torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

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
  BOOST_TEST_MESSAGE("*********************** get jobs output : normal call ok!!!!*****************************" << " \n");
}





BOOST_AUTO_TEST_CASE( get_job_output_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing normal bad sessionKey for the get jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  JobResult outputInfos;

  BOOST_CHECK_THROW(getJobOutput("bad sessionKey",machineId, jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get jobs output : normal call ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}




BOOST_AUTO_TEST_CASE( get_job_output_bad_machineId)
{

  BOOST_TEST_MESSAGE(" Testing normal bad machineId for the get jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  JobResult outputInfos;

  BOOST_CHECK_THROW(getJobOutput(sessionKey,"bad machineId", jobInfo.getJobId(), outputInfos, TMSWORKINGDIR),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get jobs output : bad machine id ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}



BOOST_AUTO_TEST_CASE( get_job_output_unterminated)
{

  BOOST_TEST_MESSAGE(" Testing an unterminated  job output  corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  JobResult outputInfos;

  BOOST_CHECK_THROW(getJobOutput(sessionKey,machineId, jobInfo.getJobId(),outputInfos, TMSWORKINGDIR),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get jobs output : unterminated ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );
}


BOOST_AUTO_TEST_SUITE_END()





//------------------------------------------------------------------------------------------------------------------------
// T1.2 : get Completed Jobs output

BOOST_AUTO_TEST_SUITE( get_Completed_Jobs_output )

const string jobOutputPath = TMSWORKINGDIR "/output.txt" ;
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  //get Completed Jobs output : normal call




BOOST_AUTO_TEST_CASE( get_completed_jobs_output_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the get completed jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/fast_torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

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

  ListJobResults listOfResults;

  BOOST_CHECK_EQUAL(getCompletedJobsOutput(sessionKey,machineId,listOfResults, TMSWORKINGDIR),0 );

 bool pathExist=bfs::exists(bfs::path(listOfResults.getResults().get(0)->getOutputPath())) &&  bfs::exists(bfs::path(listOfResults.getResults().get(0)->getErrorPath()));

  BOOST_CHECK( pathExist );

  if (pathExist){
    bfs::remove (bfs::path(listOfResults.getResults().get(0)->getOutputPath()));
    bfs::remove (bfs::path(listOfResults.getResults().get(0)->getErrorPath() ));
  }


  BOOST_TEST_MESSAGE("*********************** get completed jobs output : normal call ok!!!!*****************************" << " \n");
}





BOOST_AUTO_TEST_CASE( get_completed_job_output_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing normal bad sessionKey for the get jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );


  ListJobResults listOfResults;

  BOOST_CHECK_THROW(getCompletedJobsOutput("bad sessionKey",machineId, listOfResults, TMSWORKINGDIR),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad sessionKey ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

}

BOOST_AUTO_TEST_CASE( get_completed_job_output_bad_machineId)
{

  BOOST_TEST_MESSAGE(" Testing normal bad machineId for the get completed jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );


  ListJobResults listOfResults;

  BOOST_CHECK_THROW(getCompletedJobsOutput(sessionKey,"bad machineId",listOfResults, TMSWORKINGDIR),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad machineId ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

}



BOOST_AUTO_TEST_CASE( get_completed_job_output_uncompleted_job)
{

  BOOST_TEST_MESSAGE(" Testing uncompleted job for the get completed jobs output function corresponding to use case T2.6" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";


  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options)==0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );


  ListJobResults listOfResults;

  BOOST_CHECK_THROW(getCompletedJobsOutput(sessionKey,machineId,listOfResults, TMSWORKINGDIR),VishnuException  );

  BOOST_TEST_MESSAGE("*********************** get completed jobs output : bad sessionKey ok!!!!*****************************" << " \n");

// delete the submitted job
  BOOST_REQUIRE(cancelJob(sessionKey, machineId, jobInfo.getJobId())==0  );

}


BOOST_AUTO_TEST_SUITE_END()
