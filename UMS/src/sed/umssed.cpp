#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include "ServerUMS.hpp"
#include "MonitorUMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/shared_ptr.hpp>
#include "AuthenticatorConfiguration.hpp"

//For ZMQ
#include "zmq.hpp"
#include "DIET_client.h"
#include "Server.hpp"

using namespace vishnu;

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "Usage: " << cmd << " vishnu_config.cfg\n";
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
int
main(int argc, char* argv[], char* envp[]) {
  int res = 0;
  int vishnuId = 0;
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  AuthenticatorConfiguration authenticatorConfig(config);

  std::string sendmailScriptPath;
  struct sigaction action;
  string UMSTYPE = "UMS";
  string mid;
  string cfg;
  string uri;
  string uridispatcher;

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::SENDMAILSCRIPT, sendmailScriptPath);
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, mid);
    config.getRequiredConfigValue<std::string>(vishnu::URI, uri);
    if(!boost::filesystem::is_regular_file(sendmailScriptPath)) {
      std::cerr << "Error: cannot open the script file for sending email\n";
      exit(1);
    }
    authenticatorConfig.check();
    if (authenticatorConfig.getAuthenType() != AuthenticatorConfiguration::UMS) {
      #ifndef USE_LDAP
        std::cerr << "Error: this authentification type uses LDAP and "
                  << "this server has not been compiled with LDAP library\n";
        exit(1);
      #endif
    }
  } catch (UserException& e) {
    std::cerr << e.what() << "\n";
    exit(1);
  }catch (std::exception& e) {
    std::cerr << argv[0] << " : "<< e.what() << "\n";
    exit(1);
  }


  // Fork a child for UMS monitoring
  pid_t pid;
  pid_t ppid;
  pid = fork();

  if (pid > 0) {
    //Initialize the UMS Server (Opens a connection to the database)
    boost::shared_ptr<ServerUMS> server(ServerUMS::getInstance());
    res = server->init(vishnuId, dbConfig, sendmailScriptPath, authenticatorConfig);

    try {
      std::vector<std::string> ls = server.get()->getServices();
      registerSeD(UMSTYPE, config, cfg, ls);
    } catch (VishnuException& e) {
      std::cout << "failed to register with err" << e.what()  << std::endl;
    }

    //Declaration of signal handler
    action.sa_handler = controlSignal;
    sigemptyset (&(action.sa_mask));
    action.sa_flags = 0;
    sigaction (SIGCHLD, &action, NULL);

    // Initialize the DIET SeD
    if (!res) {
      ZMQServerStart(server, uri);
      unregisterSeD(UMSTYPE, config);
    } else {
      std::cerr << "There was a problem during services initialization\n";
      exit(1);
    }
  }
  else if (pid == 0) {
    // Initialize the UMS Monitor (Opens a connection to the database)
    MonitorUMS monitor;
    dbConfig.setDbPoolSize(1);
    monitor.init(vishnuId, dbConfig, authenticatorConfig);
    ppid = getppid();

    while(kill(ppid,0) == 0) {
      monitor.run();
    }
  } else {
    std::cerr << "There was a problem to initialize the server\n";
    exit(1);
  }
  return res;
}
