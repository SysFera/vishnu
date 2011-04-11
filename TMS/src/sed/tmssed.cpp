#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "TMSServer.hpp"

int
usage(char* cmd) {
  std::cout << "Usage: %s <diet_config.cfg> batchType [TORQUE|LOADLEVELER] <machineId>\n"+ std::string(cmd);
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
  BatchType bacthType ;
  std::string batchTypeStr;
  std::string machineId;

  if (argc < 4) {
    return usage(argv[0]);
  }

  // Check DIET Configuration file
  if(!boost::filesystem::is_regular_file(argv[1])) {
    std::cerr << "Error: cannot open DIET configuration file" << std::endl;
    exit(1);
  }

  // Other command-line parameters
  batchTypeStr = argv[2];
  if (batchTypeStr == "TORQUE") {
    bacthType = TORQUE;
  } else if (batchTypeStr == "LOADLEVELER") {
    bacthType = LOADLEVELER;
  } else {
    std::cerr << "Error: invalid value for batch type parameter (must be 'TORQUE' or 'LOADLEVLER')" << std::endl;
    exit(1);
  }
  machineId = argv[3];

  //Initialize the UMS Server (Opens a connection to the database)
  TMSServer* server = TMSServer::getInstance();
  res = server->init(bacthType, machineId);

  // Initialize the DIET SeD
  if (!res) {
    diet_print_service_table();
    res = diet_SeD(argv[1], argc, argv);
    if (server != NULL) {
      delete server;
    }
  } else {
    std::cerr << "There was a problem during services initialization" << std::endl;
    exit(1);
  }

return res;
}
