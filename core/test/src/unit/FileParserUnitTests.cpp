
#include <boost/test/unit_test.hpp>
#include <string>
#include "FileParser.hpp"
#include "CommonParser.hpp"            
#include "UserException.hpp"
#include "tmsUtils.hpp"


BOOST_AUTO_TEST_SUITE( FileParser_unit_tests )


BOOST_AUTO_TEST_CASE( test_parseFile )
{
  std::string config = "SED_URIADDR=tcp://192.168.0.1 machine_1\nbatchSchedulerType=SLURM\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  FileParser fileParser;
  fileParser.parseFile(configpath);
  std::map<std::string, std::string> configmap = fileParser.getConfiguration();
  BOOST_CHECK_EQUAL(configmap.at("SED_URIADDR"), "tcp://192.168.0.1 machine_1");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerType"), "SLURM");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerVersion"), "2.5");
  
}


BOOST_AUTO_TEST_CASE( test_parseFile_no_file )
{
  
  FileParser fileParser;
  BOOST_CHECK_THROW(fileParser.parseFile("configpath") , UserException );
  
}

BOOST_AUTO_TEST_CASE( test_parseFile_multiple_value )
{
  std::string config = "SED_URIADDR=tcp://192.168.0.1 machine_1\nbatchSchedulerType=SLURM=SGE=TORQUE\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  FileParser fileParser;
  fileParser.parseFile(configpath);
  std::map<std::string, std::string> configmap = fileParser.getConfiguration();
  BOOST_CHECK_EQUAL(configmap.at("SED_URIADDR"), "tcp://192.168.0.1 machine_1");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerType"), "SLURM=SGE=TORQUE");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerVersion"), "2.5");

}

BOOST_AUTO_TEST_CASE( test_parseFile_no_value )
{
  std::string config = "SED_URIADDR=tcp://192.168.0.1 machine_1\nbatchSchedulerType=\nbatchSchedulerVersion=2.5\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  FileParser fileParser;
  fileParser.parseFile(configpath);
  std::map<std::string, std::string> configmap = fileParser.getConfiguration();
  BOOST_CHECK_EQUAL(configmap.at("SED_URIADDR"), "tcp://192.168.0.1 machine_1");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerType"), "");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerVersion"), "2.5");
  
}

BOOST_AUTO_TEST_CASE( test_parseFile_double_key )
{
  std::string config = "SED_URIADDR=tcp://192.168.0.1 machine_1\nbatchSchedulerType=SLURM\nbatchSchedulerVersion=2.5\nbatchSchedulerType=SGE\n";
  std::string configpath = boost::filesystem::unique_path(boost::filesystem::temp_directory_path().string() +"/vishnu-config%%%%%%%").string();
  vishnu::saveInFile(configpath, config);
  FileParser fileParser;
  fileParser.parseFile(configpath);
  std::map<std::string, std::string> configmap = fileParser.getConfiguration();
  BOOST_CHECK_EQUAL(configmap.at("SED_URIADDR"), "tcp://192.168.0.1 machine_1");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerType"), "SLURM");
  BOOST_CHECK_EQUAL(configmap.at("batchSchedulerVersion"), "2.5");
  
}




BOOST_AUTO_TEST_SUITE_END()