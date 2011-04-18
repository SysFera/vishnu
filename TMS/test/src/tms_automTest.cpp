//#include <ecore.hpp> // Ecore metamodel
//#include <ecorecpp.hpp> // EMF4CPP utils
// Generated code
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "TMS_Data.hpp"
#include "TMS_Data_forward.hpp"
#include <iostream>
#include "fixtures.hpp"
#include "tmsTestUtils.hpp"
using namespace std;
using namespace UMS_Data;
using namespace TMS_Data;
using namespace vishnu;

BOOST_GLOBAL_FIXTURE(VishnuConfigure)

//Test category 1

// T1.1 : submit a job

BOOST_AUTO_TEST_SUITE(submit_a_job)

  // submit a job: normal call

BOOST_AUTO_TEST_CASE( submit_a_Job_normal_call)
{

  BOOST_MESSAGE(" Testing normal job submission corresponding to use case T1.1" );
  
  
  VishnuConnexion vc("root","vishnu_user"); 

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";


  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/output.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/error.txt");

  BOOST_CHECK_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0  );

  BOOST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );
  // Check the success of submitJob function

}


// submit a job: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_sessionKey)
{

  BOOST_MESSAGE(" Testing bad session Key for job submission (use case T1.1)" );
  
  VishnuConnexion vc("root","vishnu_user"); 

  // get the session key and the machine identifier
  string sessionKey="bad session key";
  string machineId="MA_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/error.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/output.txt");

  BOOST_CHECK_THROW(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,VishnuException );
}

// submit a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_machineId)
{
  BOOST_MESSAGE(" Testing bad machine identifier for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="badMachineId";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/error.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/output.txt");



  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );
}


// submit a job: bad parameters: bad script content

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script_content)
{
  BOOST_MESSAGE(" Testing bad script content for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/bad_torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/output.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/error.txt");

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );

}
// submit a job: bad parameters: bad script path

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_script_path)
{
  BOOST_MESSAGE(" Testing bad script path for job submission (use case T1.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/output.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/error.txt");

  BOOST_CHECK_THROW( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),VishnuException );

}
BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------------------


// T2.1 : cancel a job

BOOST_AUTO_TEST_SUITE(cancel_a_job)

  // submit a job: normal call

BOOST_AUTO_TEST_CASE( cancel_a_Job_normal_call)
{

  BOOST_MESSAGE(" Testing normal job cancelling corresponding to use case T2.1" );
  
  
  VishnuConnexion vc("root","vishnu_user"); 

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";


  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/output.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/error.txt");

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0  );

  BOOST_MESSAGE("************ The job identifier is " << jobInfo.getJobId() );

  // now let cancel the job
 
// setting cancel job parameters


  BOOST_CHECK_EQUAL(cancelJob(sessionKey, machineId, jobInfo.getJobId()),0  );


}


// cancel a job: bad parameters: bad session key

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_sessionKey)
{

  BOOST_MESSAGE(" Testing bad session Key for job cancelling (use case T2.1)" );
  
  VishnuConnexion vc("root","vishnu_user"); 

  // get the session key and the machine identifier
  string sessionKey=vc.getConnexion();
  string badSessionKey="bad session key";
  string machineId="MA_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/error.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/output.txt");

  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,0 );

  // now let cancel the job

  // setting cancel job parameters


  BOOST_CHECK_THROW(cancelJob(badSessionKey, machineId, jobInfo.getJobId()),VishnuException );

}

// cancel a job: bad parameters: bad machine identifier

BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_machineId)
{
  BOOST_MESSAGE(" Testing bad machine identifier for job cancellin (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";
  string badMachineId="badMachineId";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/torque_script";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/error.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/output.txt");


  BOOST_REQUIRE_EQUAL(submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options) ,0 );

  // now let cancel the job

  // setting cancel job parameters

  BOOST_CHECK_THROW(cancelJob(sessionKey, badMachineId, jobInfo.getJobId()),VishnuException );

}


// submit a job: bad parameters: bad job id



BOOST_AUTO_TEST_CASE( cancel_a_Job_bad_JobId)
{
  BOOST_MESSAGE(" Testing bad job identifier for job cancelling (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";


  // now let cancel the job


  BOOST_CHECK_THROW(cancelJob(sessionKey,machineId, "bad job id"),VishnuException );

}


/*


// submit a job: bad parameters: bad user id

BOOST_AUTO_TEST_CASE( submit_a_Job_bad_userId)
{
  BOOST_MESSAGE(" Testing bad user identifier for job cancelling (use case T2.1)" );

  VishnuConnexion vc("root","vishnu_user");

  // get the session key and the machine identifier

  string sessionKey=vc.getConnexion();

  string machineId="MA_1";

  //Setting submitjob parameters

  const std::string scriptFilePath="/home/ibrahima/Brouillon/";
  Job jobInfo;
  SubmitOptions options;
  options.setOutputPath("/home/ibrahima/Brouillon/JobOutput/output.txt");
  options.setErrorPath("/home/ibrahima/Brouillon/JobOutput/error.txt");

  BOOST_REQUIRE_EQUAL( submitJob(sessionKey, machineId, scriptFilePath, jobInfo,options),0 );

  // now let cancel the job

  // setting cancel job parameters


  // get another connexion for another user 
  
  VishnuConnexion vc("U_1","vishnu_user");

    // get the session key and the machine identifier
    //
      string sessionKey=vc.getConnexion();

  BOOST_CHECK_THROW(cancelJob(sessionKey, machineId, jobInfo.getJobId()),VishnuException  );



}
*/


BOOST_AUTO_TEST_SUITE_END()




//-----------------------------------------------------------------------------------------



//==============================================================================================


/*


//Test category 2

// T1.1 : submit a job

BOOST_AUTO_TEST_SUITE(T1.1_submit_a_job)

// submit a job: normal call

BOOST_AUTO_TEST_CASE( submitJob_normal_call)
{
 BOOST_MESSAGE(" Testing normal job submission T1.1" );

   BOOST_CHECK( true );
}


// submit a job: bad parameters

BOOST_AUTO_TEST_CASE( submitJob_bad_parameters)
{
 BOOST_MESSAGE(" Testing normal job submission T1.1" );

   BOOST_CHECK( true );
}

BOOST_AUTO_TEST_SUITE_END()



//-----------------------------------------------------------------------------------------------

*/
