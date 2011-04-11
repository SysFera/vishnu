#include "constants.hpp"

namespace vishnu {

// !!! None of these keywords can be a prefix for another !!!
  param_t params[] =
  {
    {DIETCONFIGFILE, "dietConfigFile", STRING_PARAMETER},
    /* [0]  */ {VISHNUID, "vishnuId", INT_PARAMETER},
    /* [1]  */ {DBTYPE, "databaseType", STRING_PARAMETER},
    /* [2]  */ {DBNAME, "databaseName", STRING_PARAMETER},
    /* [3]  */ {DBUSERNAME, "databaseUserName", STRING_PARAMETER},
    /* [4]  */ {DBPASSWORD, "databaseUserPassword", STRING_PARAMETER},
    /* [5]  */ {SENDMAILSCRIPT, "sendmailScriptPath", STRING_PARAMETER}
  };
}

