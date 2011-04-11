#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include "ServerUMS.hpp"
#include "MonitorUMS.hpp"
#include "configuration.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace vishnu;
/**
 * \mainpage UMS
 *
 * <center><b>UMS is the module that deals with the authentication of the users and the machines in the VISHNU infrastructure</b></center>
 *
 * \image html logoSysFera.jpg
 *
 * \authors CAPOCHICHI Eugene
 * \authors CISSE      Ibrahima
 * \authors COULOMB    Kevin
 * \authors ISNARD     Benjamin
 * \authors TRAORE     Daouda
 *
 * \file umssed.cpp
 * \brief The main function offering the set of services
 */

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "Usage: %s vishnu_config.cfg\n"+ std::string(cmd);
  return 1;
}


/**
 * \brief To catch a signal
 * \param signum is the signal to catch
 */
void
controlSignal (int signum) {
  int res;
  switch (signum) {
    case SIGCHLD:
      res = waitpid (-1, NULL, WNOHANG);
      while (res > 0) {
        res = waitpid (-1, NULL, WNOHANG);
      }
      break;
    default:
     break;
  }
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
  int dbType = 0;
  int vishnuid = 0;
  std::string vishnuConfigFile;
  std::string dietConfigFile;
  std::string dbTypeStr;
  std::string dbHost;
  std::string dbUsername;
  std::string dbPassword;
  std::string sendmailScriptPath;
  struct sigaction action;

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Check VISHNU Configuration file
  if(!boost::filesystem::is_regular_file(argv[1])) {
    std::cerr << "Error: cannot open VISHNU configuration file" << std::endl;
    exit(1);
  }
  vishnuConfigFile = argv[1];

  // Load Configuration file
  FileParser fileParser;
  try {
    fileParser.parseFile(vishnuConfigFile);
  } catch (...) {
    throw SystemException(0, "while parsing " + vishnuConfigFile);
  }
  CONFIGMAP = fileParser.getConfiguration();

  // Check DIET Configuration file
  std::string tmpString;
  if (!CONFIG_STRING(vishnu::DIETCONFIGFILE, tmpString)) {
    throw UserException(ERRCODE_CONFIGNOTFOUND, "dietConfigFile");
  }
  if(!boost::filesystem::is_regular_file(tmpString)) {
    std::cerr << "Error: cannot open DIET configuration file" << std::endl;
    exit(1);
  }
  dietConfigFile = tmpString;

  // Check the script path for sending mail
  if(!boost::filesystem::is_regular_file(argv[7])) {
    std::cerr << "Error: cannot open the script file for sending mail" << std::endl;
    exit(1);
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
  sendmailScriptPath = argv[7];

  pid_t pid;
  pid_t ppid;
  pid = fork();

  if (pid > 0) {
    //Initialize the UMS Server (Opens a connection to the database)
    ServerUMS* server = ServerUMS::getInstance();
    res = server->init(vishnuid, dbType, dbHost, dbUsername, dbPassword, sendmailScriptPath);

    //Declaration of signal handler
    action.sa_handler = controlSignal;
    sigemptyset (&(action.sa_mask));
    action.sa_flags = 0;
    sigaction (SIGCHLD, &action, NULL);

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
  }
  else if (pid == 0) {
    // Initialize the UMS Monitor (Opens a connection to the database)
    MonitorUMS monitor;
    monitor.init(vishnuid, dbType, dbHost, dbUsername, dbPassword);
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
