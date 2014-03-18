#include <csignal>
#include <cstring>
#include <iostream>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include "AuthenticatorConfiguration.hpp"
#include "utilServer.hpp"
#include "MonitorXMS.hpp"



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

struct SedConfig {
  SedConfig() : dbConfig(config), authenticatorConfig(config), vishnuId(0) {}
  ExecConfiguration config;
  DbConfiguration dbConfig;
  AuthenticatorConfiguration authenticatorConfig;
  std::string mid;
  BatchType batchType;
  std::string batchVersion;
  int vishnuId;
};


void
readConfiguration(const std::string& initFile, SedConfig& cfg) {
  std::string sendmailScriptPath;
  std::string uri;
  bool sub(false);

  try {
    cfg.config.initFromFile(initFile);
    cfg.config.getRequiredConfigValue<int>(vishnu::VISHNUID, cfg.vishnuId);
    cfg.dbConfig.check();
    cfg.config.getRequiredConfigValue<std::string>(vishnu::SENDMAILSCRIPT, sendmailScriptPath);
    cfg.config.getRequiredConfigValue<std::string>(vishnu::SED_URIADDR, uri);
    cfg.config.getRequiredConfigValue<bool>(vishnu::SUBSCRIBE, sub);
    cfg.config.getConfigValue<std::string>(vishnu::MACHINEID, cfg.mid);
    if(!boost::filesystem::is_regular_file(sendmailScriptPath)) {
      std::cerr << "Error: cannot open the script file for sending email on";
      exit(1);
    }

    std::string batchTypeStr;
    cfg.config.getRequiredConfigValue<std::string>(vishnu::BATCHTYPE, batchTypeStr);
    cfg.batchType = vishnu::convertToBatchType(batchTypeStr);
    if (cfg.batchType == UNDEFINED) {
      std::cerr << "\nError: Invalid batch. Batch type must be TORQUE, LOADLEVELER, SLURM, LSF, SGE, PBSPRO, POSIX or DELTACLOUD)\n";
      exit(1);
    }

    if (cfg.batchType != DELTACLOUD) {
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
    default:
      break;
    }

    if (!versError.empty()) {
      std::cerr << "\nError: specified batch version is not supported.\n"
                << "Supported versions for " << batchTypeStr
                << " are: " << versError << "\n";
      exit(1);
    }

    cfg.authenticatorConfig.check();

  } catch (const std::exception& e) {
  }
}



int
main(int argc, char* argv[], char* envp[]) {
  // initialisation


  // command-line
  if (argc !=  2) {
    return usage(argv[0]);
  }

  if (strcmp(argv[1], "-v") || strcmp(argv[1], "--v")) {
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
