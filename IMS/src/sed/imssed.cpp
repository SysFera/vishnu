#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include "ServerIMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include "utilServer.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "controller/HM/HM.hpp"

#include <boost/thread.hpp>

#include <sys/types.h>
#include <signal.h>

using namespace vishnu;
using namespace boost;
/**
 * \mainpage IMS
 *
 * <center><b>IMS is the module that deals with the management of the information in the vishnu system</b></center>
 *
 * \image html logoSysFera.jpg
 *
 * \authors CAPOCHICHI Eugene
 * \authors CISSE      Ibrahima
 * \authors COULOMB    Kevin
 * \authors ISNARD     Benjamin
 * \authors TRAORE     Daouda
 *
 * \file imssed.cpp
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
  std::cout << "Usage: %s <vishnu_config.cfg> \n"+ std::string(cmd);
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
  string sendmailScriptPath;
  string dietConfigFile;
  string IMSTYPE = "IMS";
  struct sigaction action;
  string mid;
  string cfg;
  string address;
  int port;

  if (argc < 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::SENDMAILSCRIPT,
                                               sendmailScriptPath);
    config.getRequiredConfigValue<std::string>(vishnu::MACHINEID, mid);
    config.getRequiredConfigValue<std::string>(vishnu::ADDR, address);
    config.getRequiredConfigValue<int>(vishnu::PORT, port);
  } catch (UserException& e) {
    std::cerr << e.what() << "\n";
    exit(1);
  }catch (std::exception& e) {
    std::cerr << argv[0] << " : "<< e.what() << "\n";
    exit(1);
  }
  // Check DIET Configuration file
  if(!boost::filesystem::is_regular_file(sendmailScriptPath)) {
    std::cerr << "Error: cannot open DIET configuration file" << "\n";
    exit(1);
  }

  // Initialize the IMS Server (Opens a connection to the database)
//  ServerIMS* server = ServerIMS::getInstance();
  boost::shared_ptr<ServerIMS> server(ServerIMS::getInstance());

  res = server->init(vishnuId, dbConfig, sendmailScriptPath, mid);

  registerSeD(IMSTYPE, config, cfg);

  // History maker thread
  HM hm = HM(sendmailScriptPath, mid);
  thread thr2(bind(&HM::run, &hm));//%RELAX<MISRA_0_1_3> Because it used to launch a thread

  //Declaration of signal handler, to remove script children
   action.sa_handler = controlSignal;
   sigemptyset (&(action.sa_mask));
   action.sa_flags = 0;
   sigaction (SIGCHLD, &action, NULL);

  // Initialize the DIET SeD
  if (!res) {
    ZMQServerStart(server, address, port);
    unregisterSeD(IMSTYPE, mid);
    pid_t pid = getpid();
    kill(pid, SIGINT);
  } else {
    std::cerr << "There was a problem during services initialization\n";
    exit(1);
  }
  // To avoid quitting to fast in case of problems
  while(1){
    sleep(1000);
  }
  return res;
}
