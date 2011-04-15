#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "MachineServer.hpp"
#include "ServerTMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "Usage: " << cmd << " vishnu_config.cfg" << std::endl;
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
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  std::string dietConfigFile;
  BatchType batchType ;
  std::string batchTypeStr;
  std::string machineId;

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<std::string>(vishnu::DIETCONFIGFILE, dietConfigFile);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
    if (batchTypeStr == "TORQUE") {
#ifndef HAVE_TORQUE
      std::cerr << "Error: can't initialize TORQUE batch type because this server has not compiled with torque library" << std::endl;
      exit(1);
#endif
      batchType = TORQUE;
    } else if (batchTypeStr == "LOADLEVELER") {
#ifndef HAVE_LOADLEVELER
      std::cerr << "Error: can't initialize LOADLEVELER batch type because this server has not compiled with LOADLEVELER library" << std::endl;
      exit(1);
#endif
      batchType = LOADLEVELER;
    } else {
      std::cerr << "Error: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVELER')" << std::endl;
      exit(1);
    }
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, machineId);
  } catch (UserException& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }

  //Initialize the TMS Server 
  ServerTMS* server = ServerTMS::getInstance();
  res = server->init(vishnuId, dbConfig, machineId, batchType);

  //A remettre dans le fichier util server
  {
    UMS_Data::UMS_DataFactory_ptr ecoreFactory = UMS_Data::UMS_DataFactory::_instance();
    UMS_Data::Machine_ptr machine = ecoreFactory->createMachine();
    machine->setMachineId(machineId);
    MachineServer machineServer(machine);
    if(machineServer.getAttribut("where machineid='"+machineId+"'").size()==0){
      delete machine;
      std::cerr << "Error: The machine of id " << machineId << " does not exist among the defined machines by VISHNU System" << std::endl;
      exit(1);
    }
    if(machineServer.getAttribut("where status=1 and  machineid='"+machineId+"'").size() == 0) {
      delete machine;
      std::cerr << "Error: The machine of id " << machineId << " is locked" << std::endl;
      exit(1);
    }
  }

  // Initialize the DIET SeD
  if (!res) {
    diet_print_service_table();
    res = diet_SeD(dietConfigFile.c_str(), argc, argv);
    if (server != NULL) {
      delete server;
    }
  } else {
    std::cerr << "There was a problem during services initialization" << std::endl;
    exit(1);
  }

return res;
}
