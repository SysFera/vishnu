#include "internalApiUMS.hpp"
#include "internalApiTMS.hpp"
#include "tmsUtils.hpp"
#include "AuthenticatorConfiguration.hpp"


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
  init(int vishnuId, DbConfiguration dbConfig,
       AuthenticatorConfiguration authenticatorConfig, const std::string& machineId,
       const BatchType& batchType, const std::string& batchVersion);

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
  int minterval;
  std::string mmachineId;
  BatchType mbatchType;
  std::string mbatchVersion;
  Database *mdatabaseVishnu;
  Authenticator *mauthenticator;  
};
