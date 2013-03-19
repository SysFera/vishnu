#include <boost/test/unit_test.hpp>
#include <string>
#include "UserException.hpp"
#include "ExecConfiguration.hpp"
#include "FileParser.hpp"               // for FileParser
#include "constants.hpp"
#include "tmsUtils.hpp"


class ExecConfigurationMock : public  ExecConfiguration {
  
public:
  ExecConfigurationMock(){
    
  };
  
  ConfigMap getConfigMap(){
    return mconfig;
  };
};


BOOST_AUTO_TEST_SUITE( ExecConfiguration_unit_tests )


BOOST_AUTO_TEST_CASE( test_initFromFile )
{
  std::string config = "batchSchedulerType=SLURM\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  BOOST_CHECK_EQUAL(sedConfig.getConfigMap().find("batchSchedulerType")->second, "SLURM");
  BOOST_CHECK_EQUAL(sedConfig.getConfigMap().find("batchSchedulerVersion")->second, "2.5");
  
}

BOOST_AUTO_TEST_CASE( test_initFromFile_multiplevalues )
{
  std::string config = "batchSchedulerType=SLURM;SGE;TORQUE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  BOOST_CHECK_EQUAL(sedConfig.getConfigMap().find("batchSchedulerType")->second, "SLURM;SGE;TORQUE");
  BOOST_CHECK_EQUAL(sedConfig.getConfigMap().find("batchSchedulerVersion")->second, "2.5");
  
}
BOOST_AUTO_TEST_CASE( test_initFromFile_noFile )
{
  ExecConfigurationMock sedConfig;
  BOOST_CHECK_THROW(sedConfig.initFromFile("badfile"); , UserException );
  
}

BOOST_AUTO_TEST_CASE( test_initFromFile_badFile )
{
  std::string config = "batchSchedulerType SLURM\nbatchSchedulerVersion 2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
 
}

BOOST_AUTO_TEST_CASE( test_getConfigValue_exist )
{
  std::string config = "batchSchedulerType=SLURM\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::string value;
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  BOOST_CHECK_EQUAL(sedConfig.getConfigValue<std::string>(vishnu::BATCHTYPE, value), true);
  BOOST_CHECK_EQUAL(value, "SLURM"); 
}

BOOST_AUTO_TEST_CASE( test_getConfigValue_no )
{
  std::string config = "batchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::string value="";
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  BOOST_CHECK_EQUAL(sedConfig.getConfigValue<std::string>(vishnu::BATCHTYPE, value), false);
  BOOST_CHECK_EQUAL(value, "");
}
BOOST_AUTO_TEST_CASE( test_getConfigValue_badvalue )
{
  std::string config = "batchSchedulerType=\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::string value="";
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  BOOST_CHECK_EQUAL(sedConfig.getConfigValue<std::string>(vishnu::BATCHTYPE, value), true);
  BOOST_CHECK_EQUAL(value, "");
}

BOOST_AUTO_TEST_CASE( test_getConfigValues_exist )
{
  std::string config = "batchSchedulerType=SLURM;SGE;TORQUE;LSF\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::vector<std::string> values;
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  BOOST_CHECK_EQUAL(sedConfig.getConfigValues(vishnu::BATCHTYPE, values), true);
  BOOST_CHECK_EQUAL(values.at(0), "SLURM");
  BOOST_CHECK_EQUAL(values.at(1), "SGE");
  BOOST_CHECK_EQUAL(values.at(2), "TORQUE");
  BOOST_CHECK_EQUAL(values.at(3), "LSF");
}

BOOST_AUTO_TEST_CASE( test_getConfigValues_no )
{
  std::string config = "nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::vector<std::string> values;
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  BOOST_CHECK_EQUAL(sedConfig.getConfigValues(vishnu::BATCHTYPE, values), false);
}

BOOST_AUTO_TEST_CASE( test_getRequiredConfigValue_exist )
{
  std::string config = "batchSchedulerType=SLURM\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::string value;
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  sedConfig.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, value);
  BOOST_CHECK_EQUAL(value, "SLURM");

}

BOOST_AUTO_TEST_CASE( test_getRequiredConfigValue_no )
{
  std::string config = "batchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  ExecConfigurationMock sedConfig;
  sedConfig.initFromFile(configpath);
  std::string value="";
  vishnu::param_type_t param = vishnu::BATCHTYPE;
  BOOST_CHECK_THROW(sedConfig.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, value), UserException );
  BOOST_CHECK_EQUAL(value, "");
}







BOOST_AUTO_TEST_SUITE_END()
