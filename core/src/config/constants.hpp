
#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include <string>

namespace vishnu {
  typedef enum {
    NONE_PARAMETER,
    BOOL_PARAMETER,
    INT_PARAMETER,
    ULONG_PARAMETER,
    STRING_PARAMETER,
    ADDRESS_PARAMETER
  } c_type_t ;

  typedef enum {
    DIETCONFIGFILE=0,
    VISHNUID,
    DBTYPE,
    DBNAME,
    DBUSERNAME,
    DBPASSWORD,
    SENDMAILSCRIPT
  } param_type_t;

  struct param_t
  {
    param_type_t key;
    const std::string value;
    // TODO: unused today but may help get rid of "simple_cast"
    c_type_t type;
  };

  extern param_t params[];
}

#endif /* _CONSTANTS_HPP_ */
