#include <fstream>
#include "ServerIMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include "utilServer.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "controller/logTool/Watcher.hpp"
#include "controller/logTool/ToolFactory.hpp"
#include <boost/thread.hpp>

using namespace vishnu;
using namespace boost;
/**
 * \mainpage IMS
 *
 * <center><b>IMS is the module that deals with the management of the information in the vishnu system</b></center>
 *
 * \image html logoSysFera.jpg
 *
 * \authors CAPOCHICHI Eugene
 * \authors CISSE      Ibrahima
 * \authors COULOMB    Kevin
 * \authors ISNARD     Benjamin
 * \authors TRAORE     Daouda
 *
 * \file imssed.cpp
 * \brief The main function offering the set of services
 */

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "Usage: %s <vishnu_config.cfg> \n"+ std::string(cmd);
  return 1;
}

/**
 * \brief The main function
 * \fn int main(int argc, char* argv[], char* envp[])
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int main(int argc, char* argv[], char* envp[]) {

  int res = 0;
  int dbType = 0;
  int vishnuId = 0;
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  string sendmailScriptPath;
  string dietConfigFile;
  string IMSTYPE = "IMS";

  if (argc < 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<std::string>(vishnu::DIETCONFIGFILE, dietConfigFile);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::SENDMAILSCRIPT, sendmailScriptPath);
  } catch (UserException& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  // Check DIET Configuration file
  if(!boost::filesystem::is_regular_file(sendmailScriptPath)) {
    std::cerr << "Error: cannot open DIET configuration file" << std::endl;
    exit(1);
  }

  // Initialize the IMS Server (Opens a connection to the database)
  ServerIMS* server = ServerIMS::getInstance();
  res = server->init(vishnuId, dbConfig, sendmailScriptPath);

  registerSeD(IMSTYPE, config);

  // Watcher thread
  Watcher w(IMSVishnuTool_v1, argc, argv);
  thread thr1(bind(&Watcher::run, &w));
  thr1.join();

  // Initialize the DIET SeD
  if (!res) {
    cout << "*****************************Printing services ! " << endl;
    diet_print_service_table();
    res = diet_SeD(dietConfigFile.c_str(), argc, argv);
  } else {
    std::cerr << "There was a problem during services initialization" << std::endl;
    exit(1);
  }
  while(1){
    sleep(1000);
  }

  return res;
}
