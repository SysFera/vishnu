#ifndef SERVER_HH
#define SERVER_HH

#include <string>

#include "DIET_data.h"

#include "Options.hh"

#ifndef FMD_CONFIG_FILE
#define FMD_CONFIG_FILE "/home/ibrahima/fileManagement/diet-fmd.cfg"
#endif

/* Configuration class for the server. */
class ServerConfig : public Configuration {
private:
  std::string defaultPath;
  std::string userTable;
  std::string groupTable;
public:
  ServerConfig();
  ServerConfig(const std::string& pgName);
  
  const std::string& getDefaultPath() const;
  const std::string& getUserTable() const;
  const std::string& getGrpTable() const;
  void setDefaultPath(const std::string& defaultPath);
  void setUserTable(const std::string& userTable);
  void setGrpTable(const std::string& grpTable);
};

/* Callback functions for the server program. */
void help(const std::string& specific, Configuration* config);
void config_file(const std::string& configFile, Configuration* config);
void user_table(const std::string& file, Configuration* config);
void grp_table(const std::string& file, Configuration* config);
void tmp_dir(const std::string& dir, Configuration* config);
void overridehost(const std::string& hostname, Configuration* config);
void omniorb_config(const std::string& path, Configuration* config);
#endif
