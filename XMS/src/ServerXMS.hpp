#ifndef _XMS_SERVER_H_
#define _XMS_SERVER_H_

#include <string>
#include "AuthenticatorConfiguration.hpp"
#include "SeD.hpp"
#include "tmsUtils.hpp"
#include "utilServer.hpp"
#include "UMSMapper.hpp"
#include "TMSMapper.hpp"
#include "FMSMapper.hpp"


class Authenticator;
class Database;

struct SedConfig {
  SedConfig() : dbConfig(config), authenticatorConfig(config), vishnuId(0), sub(false), hasUMS(false), hasTMS(false) {}

  ExecConfiguration config;
  DbConfiguration dbConfig;
  AuthenticatorConfiguration authenticatorConfig;
  std::string mid;
  std::string uri;
  BatchType batchType;
  std::string batchVersion;
  std::string defaultBatchConfig;
  std::string sendmailScriptPath;
  int vishnuId;
  bool sub;
  bool hasUMS;
  bool hasTMS;
};

class ServerXMS : public SeD {
public:
  static ServerXMS*
  getInstance();

  int
  getVishnuId() const;

  std::string
  getSendmailScriptPath() const;

  int
  init(SedConfig& cfg);

  ~ServerXMS();

  ExecConfiguration_Ptr
  getSedConfig() const;

  std::vector<std::string>
  getDefaultBatchOption() const;

  BatchType
  getBatchType() const;

  std::string
  getBatchVersion() const;

  int
  getDebugLevel() const { return mdebugLevel; }

  Database*
  getDatabaseVishnu();

private:
  void
  initMap(const std::string& mid);

  ServerXMS();

  ServerXMS&
  operator=(const ServerXMS&);

  void
  getConfigOptions(const char* configPath,
                   std::vector<std::string>& defaultOptions,
                   const char* batchKey);

  static ServerXMS *minstance;
  std::string msendmailScriptPath;
  int mvishnuId;
  static Database *mdatabaseVishnu;
  static UMSMapper *mmapperUMS;
  static TMSMapper *mmapperTMS;
  static FMSMapper *mmapperFMS;
  static Authenticator *mauthenticator;
  static std::string muriNamerCfg;
  BatchType mbatchType;
  std::string mbatchVersion;
  std::string mmachineId;
  std::string mslaveBinDir;
  ExecConfiguration_Ptr msedConfig;
  std::vector<std::string> mdefaultBatchOption;
  int mdebugLevel;
  bool hasUMS;
  bool hasTMS;
};

#endif
