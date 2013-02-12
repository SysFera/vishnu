/**
 * \file tmssed.cpp
 * \brief This file contains the VISHNU TMS sed main function.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April 2011
 */

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sys/types.h>
#include <boost/shared_ptr.hpp>

#include "MachineServer.hpp"
#include "ServerTMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include "MonitorTMS.hpp"
#include "Logger.hpp"

//For ZMQ
#include "zmq.hpp"
#include "DIET_client.h"
#include "Server.hpp"
#include "DbFactory.hpp"
#include "zhelpers.hpp"
#include "CommServer.hpp"

#include <signal.h>

static const unsigned int MAXPATHLEN = 255;   // make this larger if you need to.



/**
 * \brief To get the full path to the current executable
 */
// TODO: this method is not portable
// TODO: this method does not use boost
std::string
getCurrentBinaryDir() {
  int length;
  char fullpath[MAXPATHLEN];

  /* /proc/self is a symbolic link to the process-ID subdir
  * of /proc, e.g. /proc/4323 when the pid of the process
  * of this program is 4323.
  *
  * Inside /proc/<pid> there is a symbolic link to the
  * executable that is running as this <pid>.  This symbolic
  * link is called "exe".
  *
  * So if we read the path where the symlink /proc/self/exe
  * points to we have the full path of the executable.
  */

  length = readlink("/proc/self/exe", fullpath, sizeof(fullpath));

  // Catch some errors
  if (length < 0) {
    cerr << "Error resolving symlink /proc/self/exe." << endl;
    exit(1);
  }
  if (length >= MAXPATHLEN) {
    cerr << "Path too long. Truncated." << endl;
    exit(1);
  }

  /* I don't know why, but the string this readlink() function
  * returns is appended with a '@'.
  */
  fullpath[length] = '\0';       // Strip '@' off the end.

  // remove the executable name to get only the directory
  std::string execFullPath = fullpath;
  std::string execDir = execFullPath.substr(0,execFullPath.find_last_of("/"));

  return execDir;
}



/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "\nUsage: " << cmd << " vishnu_config.cfg\n\n";
  return 1;
}

/**
 * \brief The main function
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int main(int argc, char* argv[], char* envp[]) {

  int res = 0;
  int vishnuId = 0;
  int interval = 1;
  BatchType batchType ;
  std::string batchTypeStr;
  std::string batchVersion = "n/a";
  std::string machineId;
  std::string remoteBinDirectory;
  std::string defaultBatchConfig;
  string TMSTYPE = "tmssed";
  string cfg;
  string uri;

  if (argc != 2) {
    return usage(argv[0]);
  }

  system("touch -f $HOME/.vishnurc"); // Create empty file it don't exist
  vishnu::sourceFile(std::string(getenv("HOME"))+"/.vishnurc"); // Source the rc file

  // Read the configuration
  ExecConfiguration_Ptr config(new ExecConfiguration);
  DbConfiguration dbConfig(*config);
  try {
    config->initFromFile(argv[1]);
    dbConfig.check();
    config->getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    config->getConfigValue<std::string>(vishnu::DEFAULTBATCHCONFIGFILE, defaultBatchConfig);
    config->getRequiredConfigValue<std::string>(vishnu::TMS_URIADDR, uri);
    config->getRequiredConfigValue<std::string>(vishnu::MACHINEID, machineId);

    if (!config->getConfigValue(vishnu::INTERVALMONITOR, interval) || interval < 0) {
      interval = 60;
      std::clog << boost::format("The monitoring interval has been set to %1%")%internal;
    }

    config->getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
    batchType = vishnu::convertToBatchType(batchTypeStr);
    if (batchType == UNDEFINED) {
      std::cerr << "\nError: Invalid batch. Batch type must be TORQUE, LOADLEVELER, SLURM, LSF, SGE, PBSPRO, POSIX or DELTACLOUD)\n";
      exit(1);
    }

    if (batchType != DELTACLOUD) {
      config->getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, batchVersion);
    }

    std::string versError;
    switch (batchType) {
    case TORQUE: {
      if (batchVersion != "2.3") {
        versError = "2.3";
      }
      break;
    }
    case PBSPRO: {
      if (batchVersion != "10.4") {
        versError = "10.4";
      }
      break;
    }
    case LOADLEVELER: {
      if (batchVersion != "3.x" && batchVersion != "2.x") {
        versError = "2.x and 3.x";
      }
      break;
    }
    case SLURM: {
      if (batchVersion != "2.2" &&
          batchVersion != "2.3" &&
          batchVersion != "2.4") {
        versError = "2.2, 2.3 and 2.4";
      }

      if (batchVersion == "2.4") {
        // SLURM 2.4 uses the same API as 2.3
        batchVersion = "2.3";
      }
      break;
    }
    case LSF: {
      if (batchVersion != "7.0") {
        versError = "7.0";
      }
      break;
    }
    case SGE: {
      if (batchVersion != "11") {
        versError = "11";
      }
      break;
    }
    default:
      break;
    }

    if (!versError.empty()) {
      std::cerr << "\nError: specified batch version is not supported.\n"
                << "Supported versions for " << batchTypeStr
                << " are: " << versError << "\n";
      exit(1);
    }

    remoteBinDirectory = getCurrentBinaryDir();
  } catch (UserException& e) {
    std::cerr << "\n" << e.what() << "\n\n";
    exit(1);
  } catch (std::exception& e) {
    std::cerr << "\n" << argv[0] << " : "<< e.what() << "\n\n";
    exit(1);
  }

  // Fork a child for UMS monitoring
  pid_t pid = fork();
  if (pid > 0) {

    try {
      //Check if machineId is authorized
      if (machineId == AUTOMATIC_SUBMIT_JOB_KEYWORD) {
        std::cerr << "\n" << AUTOMATIC_SUBMIT_JOB_KEYWORD
                  << " is not authorized as machine identifier. "
                  << "It is a TMS keyword.\n\n";
        exit(1);
      }
      if (0 == machineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)) {
        std::cerr << "\n" << LIST_JOBS_ON_MACHINES_KEYWORD
                  << " is not authorized as machine identifier. "
                  << "It is a TMS keyword.\n\n";
        exit(1);
      }

      //Initialize the TMS Server
      boost::shared_ptr<ServerTMS> server (ServerTMS::getInstance());
      res = server->init(vishnuId, dbConfig, machineId, batchType, remoteBinDirectory, config);

      std::vector<std::string> ls = server.get()->getServices();
      registerSeD(TMSTYPE, *config, cfg, ls);

      UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();

      // check the machine
      UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
      machine->setMachineId(machineId);
      MachineServer machineServer(machine);
      machineServer.checkMachine();
      if (machine) delete machine;

      // Initialize the DIET SeD
      if (!res) {
        ZMQServerStart(server, uri);
        unregisterSeD(TMSTYPE, *config);
      } else {
        std::cerr << "\nAn error occurred when initializing the services\n\n";
        exit(1);
      }
    } catch (VishnuException& e) {
      std::cerr << e.what() << "\n";
      exit(1);
    }
  }  else if (pid == 0) {
    if (batchType != POSIX){
      pid_t pidp;
      pidp = fork();
      if (pidp == -1){
        std::cerr << "\nAn error occurred when creating the posix monitor\n\n";
        exit(1);
      } else if (pidp > 0) {
        MonitorTMS monitor(interval);
        dbConfig.setDbPoolSize(1);
        monitor.init(vishnuId, dbConfig, machineId, batchType, batchVersion);
        monitor.run();
      } else {
        MonitorTMS monitor2(interval);
        dbConfig.setDbPoolSize(1);
        monitor2.init(vishnuId, dbConfig, machineId, POSIX, batchVersion);
        monitor2.run();
      }
    } else {
      MonitorTMS monitor(interval);
      dbConfig.setDbPoolSize(1);
      monitor.init(vishnuId, dbConfig, machineId, batchType, batchVersion);
      monitor.run();
    }
  } else {
    std::cerr << "\nThere was a problem to initialize the server\n\n";
    exit(1);
  }
  delete config;
  return res;
}
