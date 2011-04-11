#include "constants.hpp"

namespace vishnu {

// !!! None of these keywords can be a prefix for another !!!
  param_t params[] =
  {
    {DIETCONFIGFILE, "dietConfigFile", STRING_PARAMETER},
    /* [1]  */ {VISHNUID, "vishnuId", INT_PARAMETER},
    /* [2]  */ {DBTYPE, "databaseType", STRING_PARAMETER},
    /* [3]  */ {DBNAME, "databaseName", STRING_PARAMETER},
    /* [4]  */ {DBUSERNAME, "databaseUserName", STRING_PARAMETER},
    /* [5]  */ {DBPASSWORD, "databaseUserPassword", STRING_PARAMETER},
    /* [6]  */ {SENDMAILSCRIPT, "sendmailScriptPath", STRING_PARAMETER}
  };
}

