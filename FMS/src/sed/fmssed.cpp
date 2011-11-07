#include <csignal>
#include <sys/wait.h>
#include <sys/types.h>
#include <fstream>
#include "ServerFMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include "MonitorFMS.hpp"
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
  int interval = 1;
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  std::string dietConfigFile;
  string FMSTYPE = "FMS";
  string mid;
  string cfg;

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<std::string>(vishnu::DIETCONFIGFILE, dietConfigFile);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    config.getRequiredConfigValue<int>(vishnu::INTERVALMONITOR, interval);
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, mid);
    if (interval < 0) {
      throw UserException(ERRCODE_INVALID_PARAM, "The Monitor interval value is incorrect");
    } 
    dbConfig.check();
  } catch (UserException& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }catch (std::exception& e) {
    std::cerr << argv[0] << " : "<< e.what() << std::endl;
    exit(1);
  }


  // Fork a child for FMS monitoring
  pid_t pid;
  pid_t ppid;
  pid = fork();

  if (pid > 0) {

    try {

      //Initialize the FMS Server (Opens a connection to the database)
      boost::scoped_ptr<ServerFMS> server(ServerFMS::getInstance());
      res = server->init(vishnuId, dbConfig);

      registerSeD(FMSTYPE, config, cfg);
      // Initialize the DIET SeD
      if (!res) {
        diet_print_service_table();
        res = diet_SeD(cfg.c_str(), argc, argv);
        unregisterSeD(FMSTYPE, mid);
      } else {
        std::cerr << "There was a problem during services initialization" << std::endl;
        unregisterSeD(FMSTYPE, mid);
        exit(1);
      }
    } catch (VishnuException& e) {
      std::cerr << e.what() << std::endl;
      exit(1);
    }
  }  else if (pid == 0) {
    // Initialize the TMS Monitor (Opens a connection to the database)
    MonitorFMS monitor(interval);
    dbConfig.setDbPoolSize(1);
    monitor.init(vishnuId, dbConfig);
    ppid = getppid();

    while(kill(ppid,0) == 0) {
      monitor.run();
    }
  } else {
    std::cerr << "There was a problem to initialize the server" << std::endl;
    exit(1);
  }

  return res;
}
