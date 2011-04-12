#include "constants.hpp"

namespace vishnu {

// !!! None of these keywords can be a prefix for another !!!
  param_t params[] =
  {
    {DIETCONFIGFILE, "dietConfigFile", STRING_PARAMETER},
    /* [1]  */ {VISHNUID, "vishnuId", INT_PARAMETER},
    /* [2]  */ {DBTYPE, "databaseType", STRING_PARAMETER},
    /* [3]  */ {DBHOST, "databaseHost", STRING_PARAMETER},
    /* [4]  */ {DBPORT, "databasePort", INT_PARAMETER},
    /* [5]  */ {DBNAME, "databaseName", STRING_PARAMETER},
    /* [6]  */ {DBUSERNAME, "databaseUserName", STRING_PARAMETER},
    /* [7]  */ {DBPASSWORD, "databaseUserPassword", STRING_PARAMETER},
    /* [8]  */ {SENDMAILSCRIPT, "sendmailScriptPath", STRING_PARAMETER},
    /* [9]  */ {BATCHTYPE, "batchSchedulerType", STRING_PARAMETER},
    /* [10] */ {MACHINEID, "vishnuMachineId", STRING_PARAMETER}
  };
}

