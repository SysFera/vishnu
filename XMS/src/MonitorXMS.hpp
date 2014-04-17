#include "internalApiUMS.hpp"
#include "internalApiTMS.hpp"
#include "tmsUtils.hpp"
#include "AuthenticatorConfiguration.hpp"
#include "ServerXMS.hpp"

class Authenticator;

class MonitorXMS {
public:
  /**
   * @brief default constructor
   */
  explicit MonitorXMS(int interval = 1);

  /**
   * @brief initialize stuff
   * @param vishnuId password to connect with database // FIXME!
   * @param dbConfig database configuration
   * @param authenticatorConfig authenticator configuration
   * @param machineId machine identifier
   * @param batchType batch scheduler used
   * @param batchVersion batch scheduler version
   * @throw
   */
  void
  init(const SedConfig& cfg);

  /**
   * @brief destructor
   */
  ~MonitorXMS();

  /**
   * @brief Launch generic sed monitor
   * @throw
   */
  int
  run();




private:
  void
  checkSession();
  void
  checkJobs(int batchtype);
  void
  checkFile();
  int minterval;
  std::string mmachineId;
  BatchType mbatchType;
  std::string mbatchVersion;
  Database *mdatabaseVishnu;
  Authenticator *mauthenticator;
  bool mhasUMS;
  bool mhasTMS;
  bool mhasFMS;
};
