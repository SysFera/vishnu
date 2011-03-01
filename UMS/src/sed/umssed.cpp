#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "ServerUMS.hpp"
#include "MonitorUMS.hpp"

using namespace utilServer;

int
usage(char* cmd) {
  std::cout << "Usage: %s <diet_config.cfg> vishnuid [ora|pg] db_host db_username db_password\n"+ std::string(cmd);
  return 1;
}

int main(int argc, char* argv[], char* envp[]) {

  int res = 0;
  int dbType = 0;
  int vishnuid = 0;
  std::string dbTypeStr;
  std::string dbHost;
  std::string dbUsername;
  std::string dbPassword;

  if (argc < 7) {
    return usage(argv[0]);
  }

  // Check DIET Configuration file
  {
    std::ifstream f(argv[1], std::ifstream::in);
    if (!f.good()) {
      std::cerr << "Error: cannot open DIET configuration file" << std::endl;
      exit(1);
    }
    f.close();
  }

  // Other command-line parameters
  vishnuid = convertToInt(argv[2]);
  dbTypeStr = argv[3];
  if (dbTypeStr == "ora") {
    dbType = ORACLEDB;
  } else if (dbTypeStr == "pg") {
    dbType = POSTGREDB;
  } else {
    std::cerr << "Error: invalid value for database type parameter (must be 'ora' or 'pg')" << std::endl;
    exit(1);
  }
  dbHost = argv[4];
  dbUsername = argv[5];
  dbPassword = argv[6];

  pid_t pid;
  pid = fork();

  if (pid > 0) {
    // Initialize the UMS Server (Opens a connection to the database)
    ServerUMS server;
    res = server.init(vishnuid, dbType, dbHost, dbUsername, dbPassword);
    // Initialize the DIET SeD
    if (!res) {
      diet_print_service_table();
      res = diet_SeD(argv[1], argc, argv);
    } else {
      std::cerr << "There was a problem during services initialization" << std::endl;
      exit(1);
    }
  }
  else if (pid == 0) {
    // Initialize the UMS Monitor (Opens a connection to the database)
    MonitorUMS monitor;
    monitor.init(vishnuid, dbType, dbHost, dbUsername, dbPassword);
    monitor.run();
  } else {
    std::cerr << "There was a problem to initialize the server" << std::endl;
    exit(1);
  }
  return res;
}
