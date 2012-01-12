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
#include <signal.h>
#include <boost/scoped_ptr.hpp>

#include "MachineServer.hpp"
#include "ServerTMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include "MonitorTMS.hpp"

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cerr << std::endl;
  std::cout << "Usage: " << cmd << " vishnu_config.cfg" << std::endl;
  std::cerr << std::endl;
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
  std::string dietConfigFile;
  BatchType batchType ;
  std::string batchTypeStr;
  std::string machineId;
  std::string remoteBinDirectory;
  string TMSTYPE = "TMS";
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
    if (interval < 0) {
      throw UserException(ERRCODE_INVALID_PARAM, "The Monitor interval value is incorrect");
    }
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
    if (batchTypeStr == "TORQUE") {
#ifndef HAVE_TORQUE
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize TORQUE batch type because this server has not compiled with TORQUE library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = TORQUE;
    } else if (batchTypeStr == "LOADLEVELER") {
#ifndef HAVE_LOADLEVELER
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize LOADLEVELER batch type because this server has not compiled with LOADLEVELER library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = LOADLEVELER;
    } else if (batchTypeStr == "SLURM") { 
#ifndef HAVE_SLURM
      std::cerr << std::endl;
      std::cerr << "Error: can't initialize SLURM batch type because this server has not compiled with SLURM library" << std::endl;
      std::cerr << std::endl;
      exit(1);
#endif
      batchType = SLURM;
    } else {
      std::cerr << std::endl;
      std::cerr << "Error: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVELER' or 'SLURM')" << std::endl;
      std::cerr << std::endl;
      exit(1);
    }
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, machineId);
    if (!config.getConfigValue<std::string>(vishnu::REMOTEBINDIR, remoteBinDirectory)) {
      remoteBinDirectory = ExecConfiguration::getCurrentBinaryDir();
    }
  } catch (UserException& e) {
    std::cerr << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << std::endl;
    exit(1);
  }catch (std::exception& e) {
    std::cerr << std::endl;
    std::cerr << argv[0] << " : "<< e.what() << std::endl;
    std::cerr << std::endl;
    exit(1);
  }

  // Fork a child for UMS monitoring
  pid_t pid;
  pid_t ppid;
  pid = fork();

  if (pid > 0) {

    try {
      //Check if machineId is authorized
      if(machineId.compare(AUTOMATIC_SUBMIT_JOB_KEYWORD)==0){ 
        std::cerr << std::endl;
        std::cerr << AUTOMATIC_SUBMIT_JOB_KEYWORD;
        std::cerr << " is not authorized as machine identifier. It is a TMS keyword." << std::endl;
        std::cerr << std::endl;
        exit(1);
      }
      if(machineId.compare(LIST_JOBS_ON_MACHINES_KEYWORD)==0) {
        std::cerr << std::endl;
        std::cerr << LIST_JOBS_ON_MACHINES_KEYWORD;
        std::cerr << " is not authorized as machine identifier. It is a TMS keyword." << std::endl;
        std::cerr << std::endl;
        exit(1);
      }
 
      //Initialize the TMS Server
      boost::scoped_ptr<ServerTMS> server (ServerTMS::getInstance());
      res = server->init(vishnuId, dbConfig, machineId, batchType, remoteBinDirectory);
      registerSeD(TMSTYPE, config, cfg);

      UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
      UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
      machine->setMachineId(machineId);

      MachineServer machineServer(machine);
      machineServer.checkMachine();
      delete machine;

      // Initialize the DIET SeD
      if (!res) {
        diet_print_service_table();
        res = diet_SeD(cfg.c_str(), argc, argv);
        unregisterSeD(TMSTYPE, machineId);
      } else {
        std::cerr << std::endl;
        std::cerr << "There was a problem during services initialization" << std::endl;
        std::cerr << std::endl;
        exit(1);
      }
    } catch (VishnuException& e) {
      std::cerr << e.what() << std::endl;
      exit(1);
    }

  }  else if (pid == 0) {
    // Initialize the TMS Monitor (Opens a connection to the database)
    MonitorTMS monitor(interval);
    dbConfig.setDbPoolSize(1);
    monitor.init(vishnuId, dbConfig, machineId, batchType);
    ppid = getppid();

    while(kill(ppid,0) == 0) {
      monitor.run();
    }
  } else {
    std::cerr << std::endl;
    std::cerr << "There was a problem to initialize the server" << std::endl;
    std::cerr << std::endl;
    exit(1);
  }
  return res;
}
