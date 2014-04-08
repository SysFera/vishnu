#include "constants.hpp"

#include <boost/assign/list_of.hpp>     // for generic_list, map_list_of, etc
#include <map>                          // for map
#include <utility>                      // for pair

namespace vishnu {

  //{{RELAX<MISRA_0_1_3> Because this table is used in other files
  // !!! None of these keywords can be a prefix for another !!!
  param_t params[] =
  {
    /* [0]  */ {VISHNUID, "vishnuId", INT_PARAMETER},
    /* [1]  */ {DBTYPE, "databaseType", STRING_PARAMETER},
    /* [2]  */ {DBHOST, "databaseHost", STRING_PARAMETER},
    /* [3]  */ {DBPORT, "databasePort", INT_PARAMETER},
    /* [4]  */ {DBNAME, "databaseName", STRING_PARAMETER},
    /* [5]  */ {DBUSERNAME, "databaseUserName", STRING_PARAMETER},
    /* [6]  */ {DBPASSWORD, "databaseUserPassword", STRING_PARAMETER},
    /* [7]  */ {SENDMAILSCRIPT, "sendmailScriptPath", STRING_PARAMETER},
    /* [8]  */ {BATCHTYPE, "batchSchedulerType", STRING_PARAMETER},
    /* [9]  */ {BATCHVERSION, "batchSchedulerVersion", STRING_PARAMETER},
    /* [10] */ {MACHINEID, "vishnuMachineId", STRING_PARAMETER},
    /* [11] */ {TIMEOUT, "timeout", INT_PARAMETER},
    /* [12] */ {REMOTEBINDIR, "vishnuRemoteBinDirectory", STRING_PARAMETER},
    /* [13] */ {INTERVALMONITOR, "intervalMonitor", INT_PARAMETER},
    /* [14] */ {DBPOOLSIZE, "databaseConnectionsNb", INT_PARAMETER},
    /* [15] */ {AUTHENTYPE, "authenticationType", STRING_PARAMETER},
    /* [16] */ {DEFAULTBATCHCONFIGFILE, "defaultBatchConfig",STRING_PARAMETER},
    /* [17] */ {URISUPERVISOR, "uriSupervisor", URI_PARAMETER},
    /* [18] */ {DISP_URIADDR, "disp_uriAddr", URI_PARAMETER},
    /* [19] */ {DISP_URISUBS, "disp_uriSubs", URI_PARAMETER},
    /* [20] */ {DISP_NBTHREAD, "disp_nbthread", INT_PARAMETER},
    /* [21] */ {HOST_URIADDR, "host_uriAddr", URI_PARAMETER},
    /* [22] */ {SED_URIADDR, "sed_uriAddr", URI_PARAMETER},
    /* [23] */ {CLOUDENDPOINT, "cloudEndpoint", STRING_PARAMETER},
    /* [24] */ {DEBUG_LEVEL, "debugLevel", INT_PARAMETER},
    /* [25] */ {DB_USE_SSL, "databaseUseSsl", BOOL_PARAMETER},
    /* [26] */ {DB_SSL_CA_FILE, "databaseSslCa", STRING_PARAMETER},
    /* [27] */ {USE_SSL, "useSsl", BOOL_PARAMETER},
    /* [28] */ {SERVER_PUBLIC_KEY, "serverPublicKey", STRING_PARAMETER},
    /* [29] */ {SERVER_PRIVATE_KEY, "serverPrivateKey", STRING_PARAMETER},
    /* [30] */ {SERVER_SSL_CERTICATE, "serverSslCertificate", STRING_PARAMETER},
    /* [31] */ {SSL_CA, "sslCa", STRING_PARAMETER},
    /* [32] */ {SUBSCRIBE, "subscribe", BOOL_PARAMETER},
    /* [33] */ {STANDALONE, "standalone", BOOL_PARAMETER},
    /* [34] */ {HAS_UMS, "enableUMS", BOOL_PARAMETER},
    /* [35] */ {HAS_TMS, "enableTMS", BOOL_PARAMETER}
  };

  std::map<cloud_env_vars_t, std::string> CLOUD_ENV_VARS =  boost::assign::map_list_of
                                                            (CLOUD_ENDPOINT, "VISHNU_CLOUD_ENDPOINT")
                                                            (CLOUD_USER, "VISHNU_CLOUD_USER")
                                                            (CLOUD_USER_PASSWORD, "VISHNU_CLOUD_USER_PASSWORD")
                                                            (CLOUD_TENANT, "VISHNU_CLOUD_TENANT")
                                                            (CLOUD_VM_IMAGE, "VISHNU_CLOUD_VM_IMAGE")
                                                            (CLOUD_VM_USER, "VISHNU_CLOUD_VM_USER")
                                                            (CLOUD_VM_USER_KEY, "VISHNU_CLOUD_VM_USER_KEY")
                                                            (CLOUD_CONTEXT_INIT, "VISHNU_CLOUD_CONTEXT_INIT")
                                                            (CLOUD_VIRTUAL_NET, "VISHNU_CLOUD_VIRTUAL_NET")
                                                            (CLOUD_VIRTUAL_NET_MASK, "VISHNU_CLOUD_VIRTUAL_NET_MASK")
                                                            (CLOUD_VIRTUAL_NET_GATEWAY, "VISHNU_CLOUD_VIRTUAL_NET_GATEWAY")
                                                            (CLOUD_VIRTUAL_NET_DNS, "VISHNU_CLOUD_VIRTUAL_NET_DNS")
                                                            (CLOUD_DEFAULT_FLAVOR, "VISHNU_CLOUD_DEFAULT_FLAVOR")
                                                            (CLOUD_NFS_SERVER, "VISHNU_CLOUD_NFS_SERVER")
                                                            (CLOUD_NFS_MOUNT_POINT, "VISHNU_CLOUD_NFS_MOUNT_POINT");


  //}}RELAX<MISRA_0_1_3>
}
