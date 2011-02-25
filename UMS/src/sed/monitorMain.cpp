#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "MonitorUMS.hpp"

using namespace utilServer;

int
usage(char* cmd) {
  std::cout << "Usage: %s interval vishnuid [ora|pg] db_host db_username db_password\n"+ std::string(cmd);
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


  // Initialize the UMS monitor (Opens a connection to the database)
  MonitorUMS monitor = convertToInt(argv[1]);
  monitor.init(vishnuid, dbType, dbHost, dbUsername, dbPassword);
  monitor.run();

  return res;
}