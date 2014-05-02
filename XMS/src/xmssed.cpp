#include <sys/wait.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include "MonitorXMS.hpp"
#include "ServerXMS.hpp"
#include "CommServer.hpp"
#include "tmsUtils.hpp"
#include "Logger.hpp"



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

  if (cfg.batchType == DELTACLOUD || cfg.batchType == OPENNEBULA) {
    exportCloudSpecificParam(&cfg.config);
  } else if (cfg.batchType == POSIX) {
  } else {
    std::string versError;
    if (! vishnu::checkIfSupportedBatchVersion(cfg.batchType, cfg.batchVersion, versError)) {
      std::string logMsg = boost::str(boost::format("[ERROR] specified batch version is not supported.\n"
                                                    "Supported versions for %1% are %2%") %batchTypeStr % versError);
      std::cerr << logMsg <<"\n";
      exit(1);
    }
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

    cfg.config.getConfigValue<bool>(vishnu::HAS_UMS, cfg.hasUMS);
    cfg.config.getConfigValue<bool>(vishnu::HAS_TMS, cfg.hasTMS);
    cfg.config.getConfigValue<bool>(vishnu::HAS_FMS, cfg.hasFMS);

    if (!cfg.config.getConfigValue<std::string>(vishnu::IPC_URI_BASE, cfg.ipcUriBase)) {
      cfg.ipcUriBase = "/tmp/vishnu-";
    }

    if (!cfg.hasUMS && !cfg.hasTMS && !cfg.hasFMS) {
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

  std::string vishnuRcFile = boost::str(boost::format("%1%/.vishnurc") % getenv("HOME"));
  if (boost::filesystem::exists(vishnuRcFile)) {
    vishnu::sourceFile(vishnuRcFile);
  }

  SedConfig cfg;
  readConfiguration(argv[1], cfg);

  // forking a child: sed monitoring
  pid_t pid;
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
    int interval;
    if (! cfg.config.getConfigValue(vishnu::INTERVALMONITOR, interval)) {
      interval = 60;
    }
    MonitorXMS monitor(interval);
    cfg.dbConfig.setDbPoolSize(1);
    monitor.init(cfg);
    monitor.run();
  } else {
    std::cerr << "There was a problem initializing the server\n";
    return 1;
  }
}
