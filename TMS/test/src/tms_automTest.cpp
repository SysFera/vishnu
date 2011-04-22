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
using namespace std;
using namespace UMS_Data;
using namespace TMS_Data;
using namespace vishnu;
namespace bpt= boost::posix_time;

BOOST_GLOBAL_FIXTURE(TMSSeDFixture)

//Test category 1

// T1.1 : submit a job

BOOST_AUTO_TEST_SUITE(submit_a_job)

const string jobOutputPath = TMSWORKINGDIR "/output.txt";
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

// BOOST_AUTO_TEST_CASE( initialize_database )
// {
//     BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing database ==" );
//     string sqlPath = TMSSQLPATH;
//     if (restore(sqlPath + "/cleanall.sql") != 0) {
//       cout << "Clean database failed" << endl;
//       return;
//     }
//     if (restore(sqlPath + "/TMSinitTest.sql")!=0) {
//       cout << "Database initialization failed" << endl;
//       return;
//     }
//     BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing database ==" );
// }

  // submit a job: normal call

BOOST_AUTO_TEST_CASE( submit_a_Job_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal job submission corresponding to use case T1.1" );

  // sleep while waiting for  tms and ums SeD

  bpt::seconds sleepTime(6);

 boost::this_thread::sleep(sleepTime);

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
 // BOOST_REQUIRE_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );

  //BOOST_REQUIRE( (lsJobs.getJobs().size()==1) && ( lsJobs.getJobs().get(0)->getJobId()==jobInfo.getJobId() )  );


}


// submit a job: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey="bad session key";
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_CHECK_THROW(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,VishnuException );
}

// submit a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="badMachineId";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);


  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );
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

const string jobOutputPath = TMSWORKINGDIR "/output.txt";
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  // submit a job: normal call

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
  Job job;
  BOOST_CHECK_THROW(getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job),VishnuException  );

}


// cancel a job: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for job cancelling (use case T2.2)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string badSessionKey="bad session key";
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,0 );

  // now let cancel the job

  // setting cancel job parameters


  BOOST_CHECK_THROW(cancelJob(badSessionKey, machineId, jobInfo.getJobId()),VishnuException );

}

// cancel a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for job cancellin (use case T2.2)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";
  string badMachineId="badMachineId";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,0 );

  // now let cancel the job

  // setting cancel job parameters

  BOOST_CHECK_THROW(cancelJob(sessionKey, badMachineId, jobInfo.getJobId()),VishnuException );

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

}


/*


// cancel a job: bad parameters: bad user id

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_userId)
{
  BOOST_TEST_MESSAGE(" Testing bad user identifier for job cancelling (use case T2.2)" );

  VishnuConnexion vc1("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc1.getConnexion();

  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE_EQUAL( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0 );

  // now let cancel the job

  // setting cancel job parameters



  // get another connexion for another user

  VishnuConnexion vc2("U_1","vishnu_user");

    // get the session key and the machine identifier
    //
       sessionKey=vc2.getConnexion();

  BOOST_CHECK_THROW(cancelJob(sessionKey, machineId, jobInfo.getJobId()),VishnuException  );



}

*/

BOOST_AUTO_TEST_SUITE_END()


/*

//-----------------------------------------------------------------------------------------

// TA1 : set machine refresh period

BOOST_AUTO_TEST_SUITE(set_machine_refresh_period)

  //set machine refresh period: normal call

BOOST_AUTO_TEST_CASE( set_machine_refresh_period_normal_call)
{

  BOOST_TEST_MESSAGE(" Testing normal execution of the set machine refresh period function  corresponding to use case TA1" );


  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  int expectedValue =10;
  //Setting  parameters

  BOOST_CHECK_EQUAL(setMachineRefreshPeriod(sessionKey,machineId, expectedValue),0);



  // wait a few seconds and check the success of cancelling job
  //bpt::seconds sleepTime(5);
  // boost::this_thread::sleep(sleepTime);
  int realValue;
  BOOST_CHECK_EQUAL(getMachineRefreshPeriod(sessionKey,machineId, realValue),0);
  BOOST_CHECK_EQUAL (realValue,expectedValue);
}


// set machine refresh period: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( set_machine_refresh_period_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for the set machine refresh period function (use case TA1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string badSessionKey="bad session key";
  string machineId="machine_1";

  int value =10;



  BOOST_CHECK_THROW(setMachineRefreshPeriod(badSessionKey,machineId, value),VishnuException );

}

// set machine refresh period: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( set_machine_refresh_period_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for set machine refresh period (use case TA1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string badMachineId="badMachineId";

  int value =10;

  BOOST_CHECK_THROW(setMachineRefreshPeriod(sessionKey,badMachineId, value),VishnuException );

}


// set machine refresh period: bad parameters: negative value



BOOST_AUTO_TEST_CASE( set_machine_refresh_period_negative_value)
{
  BOOST_TEST_MESSAGE(" Testing negative value for set machine refresh period (use case TA1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";

  int negValue=-1;

  BOOST_CHECK_THROW(setMachineRefreshPeriod(sessionKey,machineId, negValue),VishnuException );


}


// set machine refresh period: bad parameters: bad user id

BOOST_AUTO_TEST_CASE( set_machine_refresh_period_bad_userId)
{
  BOOST_TEST_MESSAGE(" Testing bad user identifier for set machine refresh period (use case TA1)" );

  VishnuConnexion vc("U_1","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";
  int value=10;

  BOOST_CHECK_THROW(setMachineRefreshPeriod(sessionKey,machineId, value),VishnuException );
}


BOOST_AUTO_TEST_SUITE_END()


*/


//-----------------------------------------------------------------------------------------



//==============================================================================================



//Test category 2



// T2.1 : get job information

BOOST_AUTO_TEST_SUITE( get_job_information)

const string jobOutputPath = TMSWORKINGDIR "/output.txt";
const string jobErrorPath = TMSWORKINGDIR "/error.txt";

  //  get job information: normal call

BOOST_AUTO_TEST_CASE(  get_job_information_normal_call)
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

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0  );

  BOOST_TEST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  ListJobs lsJobs;
  ListJobsOptions lsOptions;
  lsOptions.setJobId(jobInfo.getJobId());
  BOOST_REQUIRE_EQUAL(listJobs(sessionKey, machineId,lsJobs,lsOptions),0  );


  Job job;
  BOOST_CHECK_EQUAL(getJobInfo(sessionKey, machineId, jobInfo.getJobId(), job),0  );

// Check the success of getJobInfo function
  //BOOST_REQUIRE(  lsJobs.getJobs().get(0)->getJobId()==pJob->getJobId() )  );
}


//get job information: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( get_job_information_bad_sessionKey)
{

  BOOST_TEST_MESSAGE(" Testing bad session Key for the get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string badSessionKey="bad session key";
  string machineId="machine_1";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobOutputPath);
  options.setErrorPath(jobErrorPath);

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,0 );

  // now let get the job information

  // set the get the job information function parameters

  Job job;

  BOOST_CHECK_THROW(getJobInfo(badSessionKey, machineId, jobInfo.getJobId(), job),VishnuException );

}

// get job information: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( get_job_information_bad_machineId)
{
  BOOST_TEST_MESSAGE(" Testing bad machine identifier for the get job information function (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="machine_1";
  string badMachineId="badMachineId";

  //Setting submitjob parameters

  const std::string scriptFilePath=TMSSCRIPTSPATH "/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath(jobErrorPath);
  options.setErrorPath(jobOutputPath);

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,0 );

  // setting get job information function parameters
  Job job;
  BOOST_CHECK_THROW(getJobInfo(sessionKey, badMachineId, jobInfo.getJobId(),job),VishnuException );

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

}


BOOST_AUTO_TEST_SUITE_END()
