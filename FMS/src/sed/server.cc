#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include "Options.hh"
#include "UserTable.hh"
#include "GrpTable.hh"
#include "services.hh"

#include "DIET_server.h"

#include "server.hh"

using namespace std;

bool callHelp = false;
char* transferFile=NULL;

/* Global variables for the services implementations. */
UserTable* users = NULL;    // The users table
GroupTable* groups = NULL;  // The groups table

string tmpDirectory;        // The temporary dir. for "cp"
string serverHostname;      // The server hostname
string omniorbConfig;       // The omniORB config. file

/* Main function of DIET file management daemon.            */
/* Use utils::Options for parameters management.            */
/* Use server::*Srv class for the services implementations. */
int main(int argc, char* argv[], char* envp[]) {
  ConfigFile cfgfile;

  /* Needed for unique file name generation. */
  srand(time(NULL));
  
  /* Get the machine hostname. */
  char host[128];
  host[127]='\0';
  gethostname(host, 127);
  serverHostname = host;
  tmpDirectory = "/tmp";
  
  ServerConfig config(argv[0]);
  /* Read the configuration from config. file. */
  try {
    string attr;
    
    cfgfile.parseFile(FMD_CONFIG_FILE);
    
    if ((attr=cfgfile.getAttr("dietconfig"))!="")
      config_file(attr, &config);
    if ((attr=cfgfile.getAttr("usertable"))!="")
      user_table(attr, &config);
    if ((attr=cfgfile.getAttr("grouptable"))!="")
      grp_table(attr, &config);
    if ((attr=cfgfile.getAttr("tmpdirectory"))!="")
      tmp_dir(attr, &config);
    if ((attr=cfgfile.getAttr("hostname"))!="")
      overridehost(attr, &config);
  } catch (runtime_error& err) {
    cerr << "Warning: the configuration file " << FMD_CONFIG_FILE
         << " does not exist" << endl;
  }
  
	// getting info from the environment variables
  Options opt(&config, argc, argv, envp);
  opt.setEnvCallback("DFMS_DIET_CONFIG", config_file);
  opt.setEnvCallback("DFMS_USER_TABLE", user_table);
  opt.setEnvCallback("DFMS_GRP_TABLE", grp_table);
  opt.setEnvCallback("DFMS_TMP_DIR", tmp_dir);
  opt.setEnvCallback("DFMS_HOST_NAME", overridehost);
  opt.setEnvCallback("OMNIORB_CONFIG", omniorb_config);
  
	// getting the info from the command line
  opt.setOptCallback("--help", help);
  opt.setOptCallback("-h", help);
  opt.setOptCallback("--config", config_file);
  opt.setOptCallback("--user-table", user_table);
  opt.setOptCallback("--grp-table", grp_table);
  opt.setOptCallback("--tmp-dir", tmp_dir);
  opt.setOptCallback("--host", overridehost);
  opt.processEnv();
  opt.processOptions();

  if (callHelp) return EXIT_SUCCESS;
  if (users==NULL) users = new UserTable();
  if (groups==NULL) groups = new GroupTable();
  
  diet_service_table_init(14);
 
  diet_profile_desc_t* infosProf = getInfosProfile();
  diet_service_table_add(infosProf, NULL, get_infos);
//  diet_profile_desc_t* getFileProf = getCpProfile();
 // diet_service_table_add(getFileProf, NULL, cp_getFile);
 // diet_profile_desc_t* getIDProf = getIDCpProfile();
 // diet_service_table_add(getIDProf, NULL, cp_getIDFile);
 // diet_profile_desc_t* chgrpProf = getChgrpProfile();
 // diet_service_table_add(chgrpProf, NULL, chgrpFile);
 // diet_profile_desc_t* chmodProf = getChmodProfile();
 // diet_service_table_add(chmodProf, NULL, chmodFile);
  diet_profile_desc_t* headProf = getHeadProfile();
  diet_service_table_add(headProf, NULL, headFile);
  diet_profile_desc_t* tailProf = getTailProfile();
  diet_service_table_add(tailProf, NULL, tailFile);
  /*diet_profile_desc_t* mkdirProf = getMkdirProfile();
  diet_service_table_add(mkdirProf, NULL, mkdirFile);
  diet_profile_desc_t* rmProf = getRmProfile();
  diet_service_table_add(rmProf, NULL, rmFile);
  diet_profile_desc_t* rmdirProf = getRmdirProfile();
  diet_service_table_add(rmdirProf, NULL, rmdirFile);
  diet_profile_desc_t* lsRecProf = getLsRecProfile();
  diet_service_table_add(lsRecProf, NULL, lsRecFile);
  diet_profile_desc_t* lsProf = getLsProfile();
  diet_service_table_add(lsProf, NULL, lsFile);
  diet_profile_desc_t* lsSimpleProf = getLsSimpleProfile();
  diet_service_table_add(lsSimpleProf, NULL, lsSimpleFile);
  diet_profile_desc_t* statusProf = getStatusProfile();
  diet_service_table_add(statusProf, NULL, statusTransfer);
  */
  if (config.getConfigFile()=="") {
    cerr << "Error: missing a DIET configuration file." << endl;
    cerr << "Please set it:" << endl;
    cerr << "\t- Using the " << FMD_CONFIG_FILE << " file by defining the dietconfig value" << endl;
    cerr << "\t- Using the DFMS_DIET_CONFIG environment variable" << endl;
    cerr << "or" << endl;
    cerr << "\t- Using the command line argument --config <DIET config. file>" << endl;
    return EXIT_FAILURE;
  }
  if (diet_SeD(const_cast<char*>(config.getConfigFile().c_str()),
               argc, argv))
  {
    cerr << "Error: Can't launch the DIET File Management Daemon" << endl;
    cerr << "Possible errors:" << endl;
    if (omniorbConfig=="") {
      cerr << " * The environment variable OMNIORB_CONFIG is not set." << endl;
      cerr << "   It could explain why the SeD cannot contact the Master Agent." << endl;
    } else {
      cerr << " * Check the ORB configuration in file " << omniorbConfig << endl;
    }
    cerr << " * Check the " << config.getConfigFile() << " configuration file." << endl;
    cerr << " * Check if the Master Agent is reachable." << endl;
  }
  
  return EXIT_FAILURE;
}

ServerConfig::ServerConfig() : Configuration() {
}

ServerConfig::ServerConfig(const string& pgName) : Configuration(pgName) {
}

const string& ServerConfig::getDefaultPath() const {
  return defaultPath;
}

const string& ServerConfig::getUserTable() const {
  return userTable;
}

const string& ServerConfig::getGrpTable() const {
  return groupTable;
}

void ServerConfig::setDefaultPath(const std::string& defaultPath) {
  this->defaultPath = defaultPath;
}

void ServerConfig::setUserTable(const std::string& userTable) {
  this->userTable = userTable;
}

void ServerConfig::setGrpTable(const std::string& grpTable) {
  this->groupTable = grpTable;
}

void help(const string& specific, Configuration* config) {
  callHelp = true;
  if (specific=="") {
    cout << "Usage: " << config->getPgName() << "[options]" << endl << endl;
    cout << "\t--help [option]\t\tDisplay this help or the help for an option" << endl;
    help("config", config);
    help("user-table", config);
    help("grp-table", config);
    help("host", config);
    help("tmp-dir", config);
    cout << endl << "N.B: These parameters override the values of the configuration file (";
    cout << FMD_CONFIG_FILE << ") and the DFMS_* environment variables." << endl;
    return;
  }
  if (specific=="config") {
    cout << "\t--config <cfg. file>\tUse this DIET configuration file" << endl;
    return;
  }
  if (specific=="user-table") {
    cout << "\t--user-table <file>\tUse this users table" << endl;
    return;
  }
  if (specific=="grp-table") {
    cout << "\t--grp-table <file>\tUse this groups table" << endl;
    return;
  }
  if (specific=="host") {
    cout << "\t--host <name>\t\tUse this hostname (override the system hostname)" << endl;
    return;
  }
  if (specific=="tmp-dir") {
    cout << "\t--tmp-dir <dir>\t\tUse this directory to store temporary files" << endl;
    return;
  }
  cerr << "\tUnknown option: " << specific << endl;
}

void config_file(const string& configFile, Configuration* config) {
  config->setConfigFile(configFile);
}

void user_table(const string& file, Configuration* config) {
  ((ServerConfig*) config)->setUserTable(file);
  users = new UserTable(file);
}

void grp_table(const string& file, Configuration* config) {
  ((ServerConfig*) config)->setGrpTable(file);
  groups = new GroupTable(file);
}

void overridehost(const string& hostname, Configuration* config) {
  (void) config; // To avoid useless warning
  serverHostname = hostname;
}

void tmp_dir(const std::string& dir, Configuration* config) {
  (void) config; // To avoid useless warning
  tmpDirectory = dir;
}

void omniorb_config(const std::string& path, Configuration* config) {
  (void) config;
  omniorbConfig = path;
}
