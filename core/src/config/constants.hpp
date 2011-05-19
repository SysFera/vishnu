
#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include <string>

namespace vishnu {
  /**
   * \brief Define a constant type
   */
  typedef enum {
    NONE_PARAMETER,
    BOOL_PARAMETER,
    INT_PARAMETER,
    ULONG_PARAMETER,
    STRING_PARAMETER,
    ADDRESS_PARAMETER
  } c_type_t ;

/**
 * \brief Define a parameter type
 */

  typedef enum {
    DIETCONFIGFILE=0,
    VISHNUID,
    DBTYPE,
    DBHOST,
    DBPORT,
    DBNAME,
    DBUSERNAME,
    DBPASSWORD,
    SENDMAILSCRIPT,
    BATCHTYPE,
    MACHINEID,
    REMOTEBINDIR,
    INTERVALMONITOR,
    DBPOOLSIZE
  } param_type_t;

  /**
   * \brief A convenient data type to store a parameter
   */
  struct param_t
  {
    /**
     * \brief key to retrieve the parameter
     */
    param_type_t key;


    /**
     * \brief the value of the parameter
     */
    const std::string value;

   /**
    * \brief the type of the parameter
    */

    c_type_t type;

  };

  /**
   * \brief An arry to store all paraeters
   */
  extern param_t params[];
}

#endif /* _CONSTANTS_HPP_ */
