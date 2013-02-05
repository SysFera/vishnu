#include "constants.hpp"
#include "boost/assign.hpp"

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
    /* [8]  */ {BATCHVERSION, "batchSchedulerVersion", STRING_PARAMETER},
    /* [9]  */ {MACHINEID, "vishnuMachineId", STRING_PARAMETER},
    /* [10] */ {TIMEOUT, "timeout", INT_PARAMETER},
    /* [11] */ {REMOTEBINDIR, "vishnuRemoteBinDirectory", STRING_PARAMETER},
    /* [12] */ {INTERVALMONITOR, "intervalMonitor", INT_PARAMETER},
    /* [13] */ {DBPOOLSIZE, "databaseConnectionsNb", INT_PARAMETER},
    /* [14] */ {AUTHENTYPE, "authenticationType", STRING_PARAMETER},
    /* [15] */ {DEFAULTBATCHCONFIGFILE, "defaultBatchConfig",STRING_PARAMETER},
    /* [16] */ {URLSUPERVISOR, "urlSupervisor", URI_PARAMETER},
    /* [17] */ {DISP_URIADDR, "disp_uriAddr", URI_PARAMETER},
    /* [18] */ {DISP_URISUBS, "disp_uriSubs", URI_PARAMETER},
    /* [19] */ {DISP_NBTHREAD, "disp_nbthread", INT_PARAMETER},
    /* [20] */ {FMS_URIADDR, "fms_uriAddr", URI_PARAMETER},
    /* [21] */ {IMS_URIADDR, "ims_uriAddr", URI_PARAMETER},
    /* [22] */ {TMS_URIADDR, "tms_uriAddr", URI_PARAMETER},
    /* [23] */ {UMS_URIADDR, "ums_uriAddr", URI_PARAMETER},
    /* [24] */ {CLOUDENDPOINT, "cloudEndpoint", STRING_PARAMETER},
    /* [25] */ {DEBUG_LEVEL, "debugLevel", INT_PARAMETER}
  };


  std::map<cloud_env_vars_t, std::string> CLOUD_ENV_VARS =  boost::assign::map_list_of
                                                            (CLOUD_ENDPOINT, "VISHNU_CLOUD_ENDPOINT")
                                                            (CLOUD_USER, "VISHNU_CLOUD_USER")
                                                            (CLOUD_USER_PASSWORD, "VISHNU_CLOUD_USER_PASSWORD")
                                                            (CLOUD_TENANT, "VISHNU_CLOUD_TENANT")
                                                            (CLOUD_VM_IMAGE, "VISHNU_CLOUD_VM_IMAGE")
                                                            (CLOUD_VM_USER, "VISHNU_CLOUD_VM_USER")
                                                            (CLOUD_VM_USER_KEY, "VISHNU_CLOUD_VM_USER_KEY")
                                                            (CLOUD_DEFAULT_FLAVOR, "VISHNU_CLOUD_DEFAULT_FLAVOR")
                                                            (CLOUD_NFS_SERVER, "VISHNU_CLOUD_NFS_SERVER")
                                                            (CLOUD_NFS_MOUNT_POINT, "VISHNU_CLOUD_NFS_MOUNT_POINT");
  //}}RELAX<MISRA_0_1_3>
}
