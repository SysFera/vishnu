#include <boost/test/unit_test.hpp>
#include <string>
#include <boost/format.hpp>
#include <cstdlib>
#include "TMSVishnuException.hpp"
#include "ServerTMS.hpp"
#include "ListJobServer.hpp"



BOOST_AUTO_TEST_SUITE( ListJobServer_unit_tests )

std::string sqlListOfJobs =
"SELECT vsessionid, submitMachineId, submitMachineName, jobId, jobName, workId, jobPath,"
" outputPath, errorPath, jobPrio, nbCpus, jobWorkingDir, job.status, submitDate, endDate, owner, jobQueue,"
" wallClockLimit, groupName, jobDescription, memLimit, nbNodes, nbNodesAndCpuPerNode, batchJobId, userid "
"FROM job, vsession, users "
"WHERE vsession.numsessionid=job.vsession_numsessionid"
" AND vsession.users_numuserid=users.numuserid"
" AND job.status > 0 ";



BOOST_AUTO_TEST_CASE( test_processOptions_no_options )
{
  //TMS_Data::ListJobsOptions, TMS_Data::ListJobs, ListJobServer
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;

  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status < 5");
}

BOOST_AUTO_TEST_CASE( test_processOptions_nbCpu )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;

  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setNbCpu(3);
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and nbCpus='3' and job.status < 5");
}

BOOST_AUTO_TEST_CASE( test_processOptions_Bad_Nbcpu )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setNbCpu(-2);
  BOOST_CHECK_THROW(listJobServer.processOptions(options, test_sql) , UserException );

}

BOOST_AUTO_TEST_CASE( test_processOptions_FromSubmitDate )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  time_t test = 1121211221;
  options->setFromSubmitDate(test);
  listJobServer.processOptions(options, test_sql);
  std::string tmp = boost::posix_time::to_simple_string(boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(test)));
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and submitDate >= '"+tmp+"' and job.status < 5");

}

BOOST_AUTO_TEST_CASE( test_processOptions_Bad_FromSubmitDate )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setFromSubmitDate(-1);
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status < 5");

}

BOOST_AUTO_TEST_CASE( test_processOptions_ToSubmitDate )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  time_t test = 1121211221;
  options->setToSubmitDate(test);
  listJobServer.processOptions(options, test_sql);
  std::string tmp = boost::posix_time::to_simple_string(boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(test)));
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and submitDate <= '"+tmp+"' and job.status < 5");

}

BOOST_AUTO_TEST_CASE( test_processOptions_Bad_ToSubmitDate )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setFromSubmitDate(-1);
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status < 5");

}

BOOST_AUTO_TEST_CASE( test_processOptions_JobState )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setStatus(5);
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status='5'");

}

BOOST_AUTO_TEST_CASE( test_processOptions_bad_JobState )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setStatus(-5);
  BOOST_CHECK_THROW(listJobServer.processOptions(options, test_sql) , UserException );
}

BOOST_AUTO_TEST_CASE( test_processOptions_multiple_JobState )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setMultipleStatus("SQWRTCDF");
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and (job.status=1 or job.status=2 or job.status=3 or job.status=4 or job.status=5 or job.status=6 or job.status=7 or job.status=8)");
}

BOOST_AUTO_TEST_CASE( test_processOptions_bad_multiple_JobState )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setMultipleStatus("hty");
  BOOST_CHECK_THROW(listJobServer.processOptions(options, test_sql) , UserException );
}

BOOST_AUTO_TEST_CASE( test_processOptions_JobPriority )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setPriority(2);
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status < 5 and jobPrio='2'");

}

BOOST_AUTO_TEST_CASE( test_processOptions_bad_JobPriority )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setPriority(-5);
  BOOST_CHECK_THROW(listJobServer.processOptions(options, test_sql) , UserException );
}

BOOST_AUTO_TEST_CASE( test_processOptions_JobOwner )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setOwner("Unit");
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status < 5 and owner='Unit'");

}

BOOST_AUTO_TEST_CASE( test_processOptions_WorkId )
{
  SessionServer session;
  std::string machineId;
  ListJobServer listJobServer(session, machineId);
  std::string test_sql = sqlListOfJobs;
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setWorkId(2);
  listJobServer.processOptions(options, test_sql);
  BOOST_CHECK_EQUAL(test_sql, sqlListOfJobs+" and job.status < 5 and workId='2'");

}

/*BOOST_AUTO_TEST_CASE( test_list )
{
  SessionServer session ;
  std::string machineId = "machine_1";
  TMS_Data::ListJobsOptions_ptr options = new TMS_Data::ListJobsOptions();
  options->setWorkId(2);
  ListJobServer listJobServer(options, session, machineId);
  listJobServer.list();

}*/


BOOST_AUTO_TEST_SUITE_END()
