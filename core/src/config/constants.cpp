#include "constants.hpp"

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
    /* [9]  */ {MACHINEID, "vishnuMachineId", STRING_PARAMETER},
    /* [10] */ {REMOTEBINDIR, "vishnuRemoteBinDirectory", STRING_PARAMETER},
    /* [11] */ {INTERVALMONITOR, "intervalMonitor", INT_PARAMETER},
    /* [12] */ {DBPOOLSIZE, "databaseConnectionsNb", INT_PARAMETER},
    /* [13] */ {AUTHENTYPE, "authenticationType",STRING_PARAMETER},
    /* [14] */ {DEFAULTBATCHCONFIGFILE, "defaultBatchConfigFile",STRING_PARAMETER},
    /* [15] */ {ADDR, "address",STRING_PARAMETER},
    /* [16] */ {PORT, "port",INT_PARAMETER},
	/* [17] */ {URI, "uri", URI_PARAMETER}
  };
//}}RELAX<MISRA_0_1_3>
}
