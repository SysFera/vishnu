/**
 * \file utilServer.cpp
 * \brief This file implements the utils functions of the vishnu system
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 15/02/2011
 */

#include "utilServer.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "DatabaseResult.hpp"
#include "utilVishnu.hpp"
#include "DbFactory.hpp"
#include "SystemException.hpp"
#include "DbFactory.hpp"
#include "Server.hpp"
#include "vishnu_version.hpp"
#include "TMSVishnuException.hpp"

bool
vishnu::isNew(const std::string& urlsup, const std::string& mid, const std::string& type) {

  DbFactory factory;
  Database *mdatabase;
  mdatabase = factory.getDatabaseInstance();
  std::string req = (boost::format("SELECT machineid"
                                   " FROM process"
                                   " WHERE machineid='%1%'"
                                   " AND vishnuname='%2%'"
                                   " AND dietname='%3%'"
                                   " AND pstatus<>%4%")%mdatabase->escapeData(mid) %mdatabase->escapeData(type) %mdatabase->escapeData(urlsup) %vishnu::STATUS_DELETED).str();
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req));
    if (result->getNbTuples() != 0) {
      return false;
    }
  } catch (SystemException& e) {
    e.appendMsgComp(" Failed to determine if the process "+type + " already exist");
    throw;
  }
  return true;
}


/**
 * @brief Validate session key and return details on the user and the session
 * @param authKey The authentication key
 * @param machineId The machine Id
 * @param databasePtr A pointer to a database instance
 * @param info The resulting information
 */
void
vishnu::validateAuthKey(const std::string& authKey,
                        const std::string& machineId,
                        Database* database,
                        UserSessionInfo& info)
{
  std::string sqlQuery = boost::str(boost::format("SELECT vsession.numsessionid, "
                                                  "  machine.address, machine.nummachineid,"
                                                  "  users.numuserid, users.userid, users.privilege, "
                                                  "  account.aclogin, account.home"
                                                  " FROM vsession, users, account, machine"
                                                  " WHERE vsession.sessionkey='%1%'"
                                                  "  AND vsession.state=%2%"
                                                  "  AND users.numuserid=vsession.users_numuserid"
                                                  "  AND users.numuserid=account.users_numuserid"
                                                  "  AND account.status=%3%"
                                                  "  AND account.machine_nummachineid=machine.nummachineid"
                                                  "  AND machine.machineid='%4%';"
                                                  )
                                    % database->escapeData(authKey)
                                    % vishnu::SESSION_ACTIVE
                                    % vishnu::STATUS_ACTIVE
                                    % database->escapeData(machineId));

  boost::scoped_ptr<DatabaseResult> sqlResult(database->getResult(sqlQuery));
  if (sqlResult->getNbTuples() < 1) {
    throw TMSVishnuException(ERRCODE_PERMISSION_DENIED,
                             "Can't get user information from the session token provided");
  }

  std::vector<std::string> rowResult = sqlResult->get(0);
  std::vector<std::string>::iterator rowResultIter = rowResult.begin();

  info.num_session = vishnu::convertToInt(*rowResultIter++);
  info.machine_address = *rowResultIter++;
  info.num_machine = *rowResultIter++;
  info.num_user = vishnu::convertToInt(*rowResultIter++);
  info.userid = *rowResultIter++;
  info.user_privilege = vishnu::convertToInt(*rowResultIter++);
  info.user_aclogin = *rowResultIter++;
  info.user_achome = *rowResultIter++;
}


/**
 * @brief Validate session key and return details on the user and the session
 * @param authKey The authentication key
 * @param databasePtr A pointer to a database instance
 * @param info The resulting information
 */
void
vishnu::validateAuthKey(const std::string& authKey,
                        Database* database,
                        UserSessionInfo& info)
{
  std::string sqlQuery = boost::str(
                           boost::format("SELECT vsession.numsessionid, "
                                         "  users.numuserid, users.userid, users.privilege, "
                                         "  account.aclogin, account.home"
                                         " FROM vsession, users, account"
                                         " WHERE vsession.sessionkey='%1%'"
                                         "  AND vsession.state=%2%"
                                         "  AND account.status=%3%"
                                         "  AND vsession.users_numuserid=users.numuserid"
                                         "  AND vsession.users_numuserid=account.users_numuserid"
                                         )
                           % database->escapeData(authKey)
                           % vishnu::SESSION_ACTIVE
                           % vishnu::STATUS_ACTIVE
                           );
  boost::scoped_ptr<DatabaseResult> sqlResult(database->getResult(sqlQuery));
  if (sqlResult->getNbTuples() < 1) {
    throw TMSVishnuException(ERRCODE_INVALID_PARAM,
                             "Can't get user local account. Check that:\n"
                             "  * your session is still active\n"
                             "  * you have a local account on this server");
  }

  std::vector<std::string> rowResult = sqlResult->get(0);
  std::vector<std::string>::iterator rowResultIter = rowResult.begin();

  info.num_session = vishnu::convertToInt(*rowResultIter++);
  info.num_user = vishnu::convertToInt(*rowResultIter++);
  info.userid = *rowResultIter++;
  info.user_privilege = vishnu::convertToInt(*rowResultIter++);
  info.user_aclogin = *rowResultIter++;
  info.user_achome = *rowResultIter++;
}


/**
 * @brief Get the template to build object identifier
 * @param objectType The object type
 * @return The template string
 */
std::string
vishnu::getIdFormatTemplate(IdType objectType)
{
  std::string format = "$CPT";
  switch(objectType) {
    case MACHINE:
      format = "$NAME";
      break;
    case USER:
      format = "$NAME";
      break;
    case JOB:
      format = "J_$CPT";
      break;
    case FILETRANSFERT:
      format = "TR_$CPT";
      break;
    case AUTH:
      format = "AUTH_$CPT";
      break;
    case WORK:
      format = "$CPT";
      break;
    default:
      break;
  }

  return format;
}
