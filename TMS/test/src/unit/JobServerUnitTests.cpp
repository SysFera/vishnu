#include <boost/test/unit_test.hpp>
#include <string>
#include <boost/format.hpp>
#include <cstdlib>
#include "TMSVishnuException.hpp"
#include "JobServer.hpp"
#include "TMSVishnuException.hpp"




class JobServerMock :  public JobServer
{
public:
  
  JobServerMock(const SessionServer& sessionServer,
                const std::string& machineId,
                const TMS_Data::Job& job,
                const ExecConfiguration_Ptr sedConfig):JobServer(sessionServer, machineId, job, sedConfig){

  }
  /**
   * \brief Destructor
   */
  ~JobServerMock(){
  }
  void scanErrorMessageMock(const std::string& errorInfo, int& code, std::string& message){
    this->scanErrorMessage(errorInfo, code, message);
  }

  void
  processDefaultOptionsMock(const std::vector<std::string>& defaultBatchOption, std::string& content, std::string& key){
    this->processDefaultOptions(defaultBatchOption, content, key);
  }

  void
  insertOptionLineMock(std::string& optionLineToInsert, std::string& content, std::string& key){
    this->insertOptionLine(optionLineToInsert, content, key);
  }
  
  void
  computeOutputDirMock(const std::string& parentDir,
                       const std::string & dirSuffix,
                       std::string & content){
    this->computeOutputDir(parentDir, dirSuffix, content);
  }
  

  std::string
  getBatchDirectiveMock(std::string& seperator) const{
    return this->getBatchDirective(seperator);
  }
  
  void
  treatSpecificParamsMock(const std::string& specificParams, std::string& scriptContent){
    this->treatSpecificParams(specificParams, scriptContent);
  }
  
  
  std::string
  processScriptMock(std::string& scriptContent,
                    TMS_Data::SubmitOptions& options,
                    const std::vector<std::string>& defaultBatchOption,
                    const std::string& machineName){

    return this->processScript(scriptContent, options, defaultBatchOption, machineName);
  }

  
  void deserializeJobMock(std::string jobSerialized){
    this->deserializeJob(jobSerialized);
  }

};



std::string Script_Content = "#!/bin/sh\n"
                             "#$ -N first_job\n"
                             "#$ -o my_first_job_gen.out\n"
                             "#$ -e my_first_job_gen.err\n"
                             "#$ -m b\n"
                             "#$ -l walltime=01:00:00\n"
                             "echo toto\n";


std::string Script_Content_variant = "#!/bin/sh\n"
                                     "#$ -N first_job\n"
                                     "#$ -o my_first_job_gen.out\n"
                                     "hjqgdjqgdqjgdqj\n"
                                     "#$ -e my_first_job_gen.err\n"
                                     "#$ -m b\n"
                                     "#$ -l walltime=01:00:00\\\n"
                                     "qgdhgqjkghjgdqjhd\n"
                                     "echo toto\n";
std::string Script_Content_result = "#!/bin/sh\n"
                                    "#$ -N first_job\n"
                                    "#$ -o my_first_job_gen.out\n"
                                    "#$ -e my_first_job_gen.err\n"
                                    "#$ -m b\n"
                                    "#$ -l walltime=01:00:00\n"
                                    "#$ I am Toto\n"
                                    "echo toto\n";

std::string Script_Content_output = "#!/bin/sh\n"
                                    "#$ -N first_job\n"
                                    "#$ -o my_first_job_gen.out\n"
                                    "#$ -e my_first_job_gen.err\n"
                                    "#$ -m b\n"
                                    "#$ -l walltime=01:00:00\n"
                                    "echo toto\n"
                                    "echo \"Output directory test\" > $VISHNU_OUTPUT_DIR/TMS_res\n";

std::string Script_Content_output_result = "#!/bin/sh\n"
                                           "#$ -N first_job\n"
                                           "#$ -o my_first_job_gen.out\n"
                                           "#$ -e my_first_job_gen.err\n"
                                           "#$ -m b\n"
                                           "#$ -l walltime=01:00:00\n"
                                           "echo toto\n"
                                           "echo \"Output directory test\" > /tmp/OUTPUT_UnitTest/TMS_res\n";

std::string Script_Content_result_variant = "#!/bin/sh\n"
                                            "#$ -N first_job\n"
                                            "#$ -o my_first_job_gen.out\n"
                                            "hjqgdjqgdqjgdqj\n"
                                            "#$ -e my_first_job_gen.err\n"
                                            "#$ -m b\n"
                                            "#$ -l walltime=01:00:00\\\n"
                                            "qgdhgqjkghjgdqjhd\n"
                                            "#$ I am Toto\n"
                                            "echo toto\n";

std::string ll_Script_content = "#!/bin/sh\n"
                                "# @ job_name=first_job\n"
                                "# @ output=my_first_job_gen.out\n"
                                "# @ error=my_first_job_gen.err\n"
                                "# @ notification=start\n"
                                "# @ wall_clock_limit=01:00:00\n"
                                "# @ queue\n"
                                "echo toto\n";
std::string ll_Script_content_result = "#!/bin/sh\n"
                                       "# @ job_name=first_job\n"
                                       "# @ output=my_first_job_gen.out\n"
                                       "# @ error=my_first_job_gen.err\n"
                                       "# @ notification=start\n"
                                       "# @ wall_clock_limit=01:00:00\n"
                                       "# @ I am Toto\n"
                                       "# @ queue\n"
                                       "echo toto\n";
std::string Script_Content_spec = "#!/bin/sh\n"
                                  "#$ -N first_job\n"
                                  "#$ -o my_first_job_gen.out\n"
                                  "#$ -e my_first_job_gen.err\n"
                                  "#$ -m b\n"
                                  "#$ -l walltime=01:00:00\n"
                                  "echo toto\n";
std::string Script_Content_spec_result = "#!/bin/sh\n"
                                         "#$ -N first_job\n"
                                         "#$ -o my_first_job_gen.out\n"
                                         "#$ -e my_first_job_gen.err\n"
                                         "#$ -m b\n"
                                         "#$ -l walltime=01:00:00\n"
                                         "#$ Toto gqjgdq\n"
                                         "echo toto\n";

std::string Script_Content_spec_result2 = "#!/bin/sh\n"
                                          "#$ -N first_job\n"
                                          "#$ -o my_first_job_gen.out\n"
                                          "#$ -e my_first_job_gen.err\n"
                                          "#$ -m b\n"
                                          "#$ -l walltime=01:00:00\n"
                                          "#$ Toto gqjgdq\n"
                                          "#$ Titi jqhdkqhdkq\n"
                                          "echo toto\n";

static const std::string generic_Script = "#!/bin/sh\n"
                                          "#% vishnu_job_name=first_job\n"
                                          "#% vishnu_output=my_first_job_gen.out\n"
                                          "#% vishnu_error=my_first_job_gen.err\n"
                                          "#% vishnu_mailNotification= BEGIN\n"
                                          "#% vishnu_wallclocklimit=01:00:00\n"
                                          "echo $VISHNU_SUBMIT_MACHINE_NAME\n";

static const std::string generic_Script_Torque = "#!/bin/sh\n"
                                                 "#PBS -N first_job\n"
                                                 "#PBS -o my_first_job_gen.out\n"
                                                 "#PBS -e my_first_job_gen.err\n"
                                                 "#PBS -m b\n"
                                                 "#PBS -l walltime=01:00:00\n"
                                                 "#PBS I am Toto\n"
                                                 "echo machine_1\n";



BOOST_AUTO_TEST_SUITE( JobServer_unit_tests )



BOOST_AUTO_TEST_CASE( test_scanErrorMessage )
{
  const std::string error_info = "1# Memory access ERROR ";
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string message = "";
  int code;
  jobserver.scanErrorMessageMock(error_info, code, message);
  BOOST_CHECK_EQUAL(message, " Memory access ERROR ");
  BOOST_CHECK_EQUAL(code, 1);
}

BOOST_AUTO_TEST_CASE( test_scanErrorMessage_bad_error_info )
{
  const std::string bad_error_info = "1 Memory access";
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string message = "";
  int code;
  jobserver.scanErrorMessageMock(bad_error_info, code, message);
  BOOST_CHECK_EQUAL(message, "");
  BOOST_CHECK_EQUAL(code, ERRCODE_INVEXCEP);
}

BOOST_AUTO_TEST_CASE( test_scanErrorMessage_bad_error_code )
{
  const std::string bad_error_code = "Memory# access";
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string message = "";
  int code;
  jobserver.scanErrorMessageMock(bad_error_code, code, message);
  BOOST_CHECK_EQUAL(message, " access");
  BOOST_CHECK_EQUAL(code, ERRCODE_INVEXCEP);
}

BOOST_AUTO_TEST_CASE(test_insertOptionLine)
{
  std::string option = "#$ I am Toto\n";
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string key = "#$";
  jobserver.insertOptionLineMock(option, Script_Content, key);
  BOOST_CHECK_EQUAL(Script_Content, Script_Content_result);
  
}
BOOST_AUTO_TEST_CASE(test_insertOptionLine_variant)
{
  std::string option = "#$ I am Toto\n";
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string key = "#$";
  jobserver.insertOptionLineMock(option, Script_Content_variant, key);
  BOOST_CHECK_EQUAL(Script_Content_variant, Script_Content_result_variant);
  
}

BOOST_AUTO_TEST_CASE(test_insertOptionLine_LL)
{
  std::string config = "batchSchedulerType=LOADLEVELER\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  std::string option = "# @ I am Toto\n";
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string key = "# @";
  jobserver.insertOptionLineMock(option, ll_Script_content, key);
  BOOST_CHECK_EQUAL(ll_Script_content,ll_Script_content_result);
  
}

BOOST_AUTO_TEST_CASE(test_processDefaultOptions)
{
  std::vector<std::string> defaultBatchOption;
  defaultBatchOption.push_back("I am");
  defaultBatchOption.push_back("Toto");
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string key = "#$";
  jobserver.processDefaultOptionsMock(defaultBatchOption,Script_Content, key);
  BOOST_CHECK_EQUAL(Script_Content,Script_Content_result);
  
}

BOOST_AUTO_TEST_CASE(test_processDefaultOptions_exist)
{
  std::vector<std::string> defaultBatchOption;
  defaultBatchOption.push_back("I am");
  defaultBatchOption.push_back("Toto");
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string key = "#$";
  std::string test = Script_Content_result;
  jobserver.processDefaultOptionsMock(defaultBatchOption,Script_Content_result, key);
  BOOST_CHECK_EQUAL(Script_Content_result, test);
  
}
BOOST_AUTO_TEST_CASE(test_processDefaultOptions_variant)
{
  std::vector<std::string> defaultBatchOption;
  defaultBatchOption.push_back("I am");
  defaultBatchOption.push_back("Toto");
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string key = "#$";
  jobserver.processDefaultOptionsMock(defaultBatchOption,Script_Content_variant, key);
  BOOST_CHECK_EQUAL(Script_Content_variant, Script_Content_result_variant);
  
}

BOOST_AUTO_TEST_CASE(test_computeOutputDir)
{

  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string parentDir = "/tmp";
  std::string dirSuffix = "UnitTest";
  jobserver.computeOutputDirMock(parentDir, dirSuffix, Script_Content_output);
  BOOST_CHECK_EQUAL(Script_Content_output, Script_Content_output_result);
  
}

BOOST_AUTO_TEST_CASE(test_computeOutputDir_no_output)
{
  
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig = NULL;
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string parentDir = "/tmp";
  std::string dirSuffix = "UnitTest";
  std::string test = Script_Content;
  jobserver.computeOutputDirMock(parentDir, dirSuffix, Script_Content);
  BOOST_CHECK_EQUAL(Script_Content, test);
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_Slurm)
{
  
  std::string config = "batchSchedulerType=SLURM\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "#SBATCH");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_Torque)
{
  
  std::string config = "batchSchedulerType=TORQUE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "#PBS");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_LL)
{
  
  std::string config = "batchSchedulerType=LOADLEVELER\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "# @");
  BOOST_CHECK_EQUAL(separator, " = ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_LSF)
{
  
  std::string config = "batchSchedulerType=LSF\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "#BSUB");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_SGE)
{
  
  std::string config = "batchSchedulerType=SGE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "#$");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_PBSPRO)
{
  
  std::string config = "batchSchedulerType=PBS\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "#PBS");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_POSIX)
{
  
  std::string config = "batchSchedulerType=POSIX\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "#%");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_DELTACLOUD)
{
  
  std::string config = "batchSchedulerType=DELTACLOUD\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_getBatchDirective_UNDEFINED)
{
  
  std::string config = "batchSchedulerType=no\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::string separator = "";
  std::string directive = jobserver.getBatchDirectiveMock(separator);
  BOOST_CHECK_EQUAL(directive, "");
  BOOST_CHECK_EQUAL(separator, " ");
  
}

BOOST_AUTO_TEST_CASE(test_treatSpecificParams_case1)
{
  
  std::string config = "batchSchedulerType=SGE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  const std::string specparams = "Toto=gqjgdq";
  std::string test = Script_Content_spec;
  jobserver.treatSpecificParamsMock(specparams, test);
  BOOST_CHECK_EQUAL(test, Script_Content_spec_result);
  
  
}

BOOST_AUTO_TEST_CASE(test_treatSpecificParams_case2)
{
  
  std::string config = "batchSchedulerType=SGE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  const std::string specparams = "Toto=gqjgdq Titi=jqhdkqhdkq";
  std::string test = Script_Content_spec;
  jobserver.treatSpecificParamsMock(specparams, test);
  BOOST_CHECK_EQUAL(test, Script_Content_spec_result2);

  
}

BOOST_AUTO_TEST_CASE(test_treatSpecificParams_case3)
{
  
  std::string config = "batchSchedulerType=SGE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  const std::string specparams = "Toto gqjgdq Titijqhdkqhdkq";
  std::string test = Script_Content_spec;
  jobserver.treatSpecificParamsMock(specparams, test);
  BOOST_CHECK_EQUAL(test, Script_Content_spec);
  
  
}

BOOST_AUTO_TEST_CASE(test_processScript_generic_Torque)
{
  
  std::string config = "batchSchedulerType=TORQUE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = bfs::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  SessionServer session;
  std::string machineId;
  TMS_Data::Job job;
  ExecConfiguration_Ptr sedConfig  = new ExecConfiguration();
  sedConfig->initFromFile(configpath);
  JobServerMock jobserver(session, machineId, job, sedConfig);
  std::vector<std::string> defaultBatchOption;
  defaultBatchOption.push_back("I am");
  defaultBatchOption.push_back("Toto");
  const std::string machine = "machine_1";
  TMS_Data::SubmitOptions options;
  std::string test = generic_Script;
  BOOST_CHECK_EQUAL(jobserver.processScriptMock(test,options, defaultBatchOption, machine), generic_Script_Torque);
  
  
}




BOOST_AUTO_TEST_SUITE_END()
