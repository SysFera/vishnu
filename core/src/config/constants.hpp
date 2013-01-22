
#ifndef _CONSTANTS_HPP_
#define _CONSTANTS_HPP_

#include <string>
#include <map>

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
    URI_PARAMETER
  } c_type_t ;

  /**
 * \brief Define a parameter type
 */

  typedef enum {
    VISHNUID=0,
    DBTYPE,
    DBHOST,
    DBPORT,
    DBNAME,
    DBUSERNAME,
    DBPASSWORD,
    SENDMAILSCRIPT,
    BATCHTYPE,
    BATCHVERSION,
    MACHINEID,
    TIMEOUT,
    REMOTEBINDIR,
    INTERVALMONITOR,
    DBPOOLSIZE,
    AUTHENTYPE,
    DEFAULTBATCHCONFIGFILE,
    URLSUPERVISOR,
    DISP_URIADDR,
    DISP_URISUBS,
    DISP_NBTHREAD,
    FMS_URIADDR,
    IMS_URIADDR,
    TMS_URIADDR,
    UMS_URIADDR,
    CLOUDENDPOINT
  } param_type_t;

  typedef enum {
    CLOUD_ENDPOINT,
    CLOUD_USER,
    CLOUD_USER_PASSWORD,
    CLOUD_TENANT,
    CLOUD_VM_IMAGE,
    CLOUD_VM_USER,
    CLOUD_VM_USER_KEY,
    CLOUD_DEFAULT_FLAVOR,
    CLOUD_NFS_SERVER,
    CLOUD_NFS_MOUNT_POINT
  } cloud_env_vars_t;

  typedef enum {
    JOB_UNDEFINED=0,
    JOB_SUBMITTED=1,
    JOB_QUEUED=2,
    JOB_WAITING=3,
    JOB_RUNNING=4,
    JOB_COMPLETED=5,
    JOB_CANCELLED=6,
    JOB_ALREADY_DOWNLOADED=7
  } job_status_t;



  /**
   * \brief A convenient data type to store a parameter
   */
  struct param_t {
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
  extern param_t params[]; //%RELAX<MISRA_3_1_3> Because this table is defined in constants.cpp
  extern std::map<cloud_env_vars_t, std::string> CLOUD_ENV_VARS;
}

#endif /* _CONSTANTS_HPP_ */
