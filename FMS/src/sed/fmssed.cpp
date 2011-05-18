#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include "ServerFMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/scoped_ptr.hpp>

using namespace vishnu;

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "Usage: " << std::string(cmd) << " vishnu_config.cfg\n";
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
  int vishnuId = 0;
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  std::string dietConfigFile;
  string FMSTYPE = "FMS";

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<std::string>(vishnu::DIETCONFIGFILE, dietConfigFile);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    dbConfig.check();
  } catch (UserException& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

    //Initialize the FMS Server (Opens a connection to the database)
    boost::scoped_ptr<ServerFMS> server(ServerFMS::getInstance());
    res = server->init(vishnuId, dbConfig);

    // Initialize the DIET SeD
    if (!res) {
      diet_print_service_table();
      res = diet_SeD(dietConfigFile.c_str(), argc, argv);
    } else {
      std::cerr << "There was a problem during services initialization" << std::endl;
      exit(1);
    }
  
  return res;
}
