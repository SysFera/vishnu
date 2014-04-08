#include <sys/wait.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include "MonitorXMS.hpp"
#include "ServerXMS.hpp"
#include "CommServer.hpp"



/**
 * @brief display CLI help for the generic sed
 * @param cmd CLI binary name
 * @return 1
 */
int
usage(const char* cmd) {
  using boost::format;
  std::cout << format("\nUsage: %1% vishnu_config.cfg\n\n") % cmd;
  return 1;
}

/* @brief Export cloud specific configuration parameters as environments variables
 */
void
exportCloudSpecificParam(ExecConfiguration_Ptr config) {
  std::string paramValue;
  if (config->getConfigValue<std::string>(vishnu::CLOUDENDPOINT, paramValue)) {
  setenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT].c_str(),
         paramValue.c_str(), 1);
  }
}


void
getBatchConfiguration(SedConfig& cfg) {
  cfg.config.getConfigValue<std::string>(vishnu::DEFAULTBATCHCONFIGFILE, cfg.defaultBatchConfig);
  std::string batchTypeStr;
  cfg.config.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
  cfg.batchType = vishnu::convertToBatchType(batchTypeStr);
  if (cfg.batchType == UNDEFINED) {
    std::cerr << boost::format("Invalid batch backend: %1%\n") 
        % batchTypeStr;
    exit(1);
  }

  cfg.config.getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, cfg.batchVersion);
  if (cfg.batchType != DELTACLOUD && cfg.batchType != OPENNEBULA) {
    cfg.config.getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, cfg.batchVersion);
  }

  std::string versError;
  switch (cfg.batchType) {
  case TORQUE: {
    if (cfg.batchVersion != "2.3") {
      versError = "2.3";
    }
    break;
  }
  case PBSPRO: {
    if (cfg.batchVersion != "10.4") {
      versError = "10.4";
    }
    break;
  }
  case LOADLEVELER: {
    if (cfg.batchVersion != "3.x" && cfg.batchVersion != "2.x") {
      versError = "2.x and 3.x";
    }
    break;
  }
  case SLURM: {
    if (cfg.batchVersion != "2.2" &&
      cfg.batchVersion != "2.3" &&
      cfg.batchVersion != "2.4") {
        versError = "2.2, 2.3 and 2.4";
    }
    break;
    }
  case LSF: {
    if (cfg.batchVersion != "7.0") {
      versError = "7.0";
    }
    break;
  }
  case SGE: {
    if (cfg.batchVersion != "11") {
      versError = "11";
    }
    break;
  }
  case DELTACLOUD:
  case OPENNEBULA:
    exportCloudSpecificParam(&cfg.config);
    break;
  default:
    break;
  }

  if (!versError.empty()) {
    std::cerr << "\nError: specified batch version is not supported.\n"
              << "Supported versions for " << batchTypeStr
              << " are: " << versError << "\n";
    exit(1);
  }
}

void
readConfiguration(const std::string& initFile, SedConfig& cfg) {
  try {
    cfg.config.initFromFile(initFile);
    cfg.config.getRequiredConfigValue<int>(vishnu::VISHNUID, cfg.vishnuId);
    cfg.dbConfig.check();
    cfg.config.getRequiredConfigValue<std::string>(vishnu::HOST_URIADDR, cfg.uri);
    cfg.config.getRequiredConfigValue<bool>(vishnu::SUBSCRIBE, cfg.sub);
    cfg.config.getConfigValue<std::string>(vishnu::MACHINEID, cfg.mid);

    bool res1 = cfg.config.getConfigValue<bool>(vishnu::HAS_UMS, cfg.hasUMS);
    bool res2 = cfg.config.getConfigValue<bool>(vishnu::HAS_TMS, cfg.hasTMS);
    if (!cfg.hasUMS && !cfg.hasTMS) {
      std::cerr << "Error: XMS is not configured to run any services\n";
      exit(1);
    }

    if (cfg.hasTMS) {
      getBatchConfiguration(cfg);
    }
    if (cfg.hasUMS) {
      cfg.config.getRequiredConfigValue<std::string>(vishnu::SENDMAILSCRIPT,
                                                     cfg.sendmailScriptPath);
      if (!boost::filesystem::is_regular_file(cfg.sendmailScriptPath)) {
        std::cerr << "Error: cannot open the script file for sending email on";
        exit(1);
      }

      cfg.authenticatorConfig.check();
    }

  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
  }
}


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

int
main(int argc, char* argv[], char* envp[]) {
  // initialisation
  struct sigaction action;
  std::string XMSTYPE = "xmssed";

  // command-line
  if (argc != 2) {
    return usage(argv[0]);
  }

  if ((0 == strcmp(argv[1], "-v")) ||
      (0 == strcmp(argv[1], "--v"))) {
    return vishnu::showVersion();
  }

  system("touch -f $HOME/.vishnurc"); // Create empty file it don't exist
  vishnu::sourceFile(std::string(getenv("HOME"))+"/.vishnurc"); // Source the rc file

  SedConfig cfg;
  readConfiguration(argv[1], cfg);

  // forking a child: sed monitoring
  pid_t pid;
  pid_t ppid;
  pid = fork();
  
  if (pid > 0) {
    //Initialize the UMS Server (Opens a connection to the database)
    boost::shared_ptr<ServerXMS> serverXMS(ServerXMS::getInstance());
    int res = serverXMS->init(cfg);

    if (cfg.sub) {
      boost::thread thr(boost::bind(&keepRegistered, XMSTYPE,
                                    cfg.config, cfg.uri, serverXMS));
    }

    //Declaration of signal handler
    action.sa_handler = controlSignal;
    sigemptyset (&(action.sa_mask));
    action.sa_flags = 0;
    sigaction (SIGCHLD, &action, NULL);

    // Initialize the Vishnu SeD
    if (!res) {
      initSeD(XMSTYPE, cfg.config, cfg.uri, serverXMS);
      exit(0);
    } else {
      std::cerr << "There was a problem during services initialization\n";
      exit(1);
    }
  } else if (pid == 0) {
    MonitorXMS monitor;
    cfg.dbConfig.setDbPoolSize(1);
    monitor.init(cfg.vishnuId, cfg.dbConfig, cfg.authenticatorConfig,
                 cfg.mid, cfg.batchType, cfg.batchVersion);
    ppid = getppid();
    while (kill(ppid, 0) == 0) {
      monitor.run();
    }
  } else {
    std::cerr << "There was a problem initializing the server\n";
    return 1;
  }
}
