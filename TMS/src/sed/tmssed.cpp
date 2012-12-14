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

//For ZMQ
#include "zmq.hpp"
#include "DIET_client.h"
#include "Server.hpp"

#include <signal.h>


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
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  BatchType batchType ;
  std::string batchTypeStr;
  std::string batchVersion;
  std::string machineId;
  std::string remoteBinDirectory;
  std::string defaultBatchConfig;
  string TMSTYPE = "tmssed";
  string cfg;

  string uri;

  if (argc != 2) {
      return usage(argv[0]);
    }

  // First set the rc file if it don't exists
  system("touch -f $HOME/.vishnurc");

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    config.getRequiredConfigValue<int>(vishnu::INTERVALMONITOR, interval);
    config.getConfigValue<std::string>(vishnu::DEFAULTBATCHCONFIGFILE, defaultBatchConfig);
    config.getRequiredConfigValue<std::string>(vishnu::TMS_URIADDR, uri);

    if (interval < 0) {
        throw UserException(ERRCODE_INVALID_PARAM, "The Monitor interval value is incorrect");
      }
    dbConfig.check();

    config.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
    config.getConfigValue<std::string>(vishnu::BATCHVERSION, batchVersion);
    if (batchTypeStr == "TORQUE") {
      batchType = TORQUE;
    } else if (batchTypeStr == "PBS") {
      batchType = PBSPRO;
    } else if (batchTypeStr == "LOADLEVELER") {
      batchType = LOADLEVELER;
    } else if (batchTypeStr == "SLURM") {
      batchType = SLURM;
    } else if (batchTypeStr == "LSF") {
      batchType = LSF;
    } else if (batchTypeStr == "SGE") {
      batchType = SGE;
    } else if (batchTypeStr == "POSIX") {
      batchType = POSIX;
    } else {
      std::cerr << "\nError: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVELER' or 'SLURM' or 'LSF' or 'SGE' or 'PBS' or 'POSIX')\n\n";
      exit(1);
    }

    // Checks on the batchVersions
    if (batchVersion.empty() && batchType != POSIX) {
      std::cerr << "\nError: batch version has not been specified, this parameter is mandatory for batch schedulers other than POSIX\n\n";
      exit(1);
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
      if (batchVersion != "3.x" || batchVersion != "2.x") {
        versError = "2.x and 3.x";
      }
      break;
    }
    case SLURM: {
      if (batchVersion != "2.2"
          || batchVersion != "2.3"
          || batchVersion != "2.4") {
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
    default: {
      // nothing to do
    }
    }

    if (!versError.empty()) {
      std::cerr << "\nError: specified batch version is not supported.\n"
                << "Supported versions for " << batchTypeStr
                << " are: " << versError << "\n";
      exit(1);
    }


    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, machineId);
    if (!config.getConfigValue<std::string>(vishnu::REMOTEBINDIR, remoteBinDirectory)) {
      remoteBinDirectory = ExecConfiguration::getCurrentBinaryDir();
    }
  } catch (UserException& e) {
    std::cerr << "\n" << e.what() << "\n\n";
    exit(1);
  } catch (std::exception& e) {
    std::cerr << "\n" << argv[0] << " : "<< e.what() << "\n\n";
    exit(1);
  }

  // Fork a child for UMS monitoring
  pid_t pid = fork();
  pid_t ppid;
  if (pid > 0) {

      try {
        //Check if machineId is authorized
        if (0 == machineId.compare(AUTOMATIC_SUBMIT_JOB_KEYWORD)){
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
        res = server->init(vishnuId, dbConfig, machineId, batchType,
                           batchVersion, remoteBinDirectory,defaultBatchConfig);

        std::vector<std::string> ls = server.get()->getServices();
        registerSeD(TMSTYPE, config, cfg, ls);

        UMS_Data::UMS_DataFactory_ptr ecoreFactory =
            UMS_Data::UMS_DataFactory::_instance();
        UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
        machine->setMachineId(machineId);

        MachineServer machineServer(machine);
        machineServer.checkMachine();
        delete machine;

        // Initialize the DIET SeD
        if (!res) {
            ZMQServerStart(server, uri);
            unregisterSeD(TMSTYPE, config);
          } else {
            std::cerr << "\nThere was a problem during services initialization\n\n";
            exit(1);
          }
      } catch (VishnuException& e) {
        std::cerr << e.what() << "\n";
        exit(1);
      }
  }  else if (pid == 0) {
    ppid = getppid();

    if (batchType != POSIX){
      pid_t pidp;
      pid_t ppidp;
      pidp = fork();
      if (pidp == -1){
        std::cerr << "\nThere was a problem to fork the posix monitor\n\n";
        exit(1);
      } else if (pidp > 0) {
        // Initialize the TMS Monitor (Opens a connection to the database)
        MonitorTMS monitor(interval);
        dbConfig.setDbPoolSize(1);
        monitor.init(vishnuId, dbConfig, machineId, batchType, batchVersion);
        monitor.run();
      } else {
        // Initialize the TMS Monitor (Opens a connection to the database)
        MonitorTMS monitor2(interval);
        dbConfig.setDbPoolSize(1);
        monitor2.init(vishnuId, dbConfig, machineId, POSIX, batchVersion);

        ppidp = getppid();
        monitor2.run();
      }
    } else {
      // Initialize the TMS Monitor (Opens a connection to the database)
      MonitorTMS monitor(interval);
      dbConfig.setDbPoolSize(1);
      monitor.init(vishnuId, dbConfig, machineId, batchType, batchVersion);
      monitor.run();
    }
  } else {
    std::cerr << "\nThere was a problem to initialize the server\n\n";
    exit(1);
  }
  return res;
}
