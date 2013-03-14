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
#include "tmsUtils.hpp"

#include <signal.h>


using namespace std;



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
  std::string batchVersion = "";
  std::string machineId;
  std::string defaultBatchConfig;
  string TMSTYPE = "tmssed";
  string uri;

  if (argc != 2) {
    return usage(argv[0]);
  }

  if (std::string(argv[1]) == "-v" ||
      std::string(argv[1]) == "--v"){
    return vishnu::showVersion("tmssed");
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

      //Initialize the TMS Server    ----- MUST BE DONE BEFORE CREATING INSTANCE
      boost::shared_ptr<ServerTMS> server (ServerTMS::getInstance());
      res = server->init(vishnuId, dbConfig, machineId, batchType, config);
      if (!res) {
        initSeD(TMSTYPE, *config, uri, server);
      } else {
        std::cerr << "There was a problem during services initialization\n";
        exit(1);
      }

      UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();

      // check the machine
      UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
      machine->setMachineId(machineId);
      MachineServer machineServer(machine);
      machineServer.checkMachine();
      if (machine) {
        delete machine;
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
