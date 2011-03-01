/**
* \file ServerUMS.cpp
* \brief This file presents the implementation of the UMS server.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2001
*/

#include "ServerUMS.hpp"
#include "UMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "internalApi.hpp"

ServerUMS* ServerUMS::minstance = NULL;

/**
 * \brief To get the unique instance of the server
 */
ServerUMS*
ServerUMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerUMS();
  }
  return minstance;
}

/**
* \brief To get the path to the sendmail script
* \fn std::string getSendmailScriptPath()
* \return the path of the configuration file
*/
std::string
ServerUMS::getSendmailScriptPath() {
  return msendmailScriptPath;
}
/**
* \brief Constructor (private)
* \fn ServerUMS()
*/
ServerUMS::ServerUMS() : mprofile(NULL) {
}

/**
* \brief To initialize the UMS server with individual parameters
* \fn int init(std::string vishnuid)
* \param vishnuId The id of the vishnu configuration registered in the database
* \param dbType   The type of the database (POSTGREDB|ORACLEDB)
* \param dbHost   The host of the database server
* \param dbUsername The name of the database user on the server
* \param dbPassword The password of the database user on the server
* \param sendmailScriptPath The path to the script for sending emails
* \return an error code (0 if success and 1 if an error occurs)
*/
int
ServerUMS::init(int vishnuId,
                int dbType,
                std::string dbHost,
                std::string dbUsername,
                std::string dbPassword,
                std::string sendmailScriptPath) {

  DbFactory factory;

  //To get database instance
  Database *mdatabaseVishnu = factory.getDatabaseInstance(dbType,
                                                          dbHost,
                                                          dbUsername,
                                                          dbPassword,
                                                          DATABASENAME);
  Vishnuid::mvishnuid = convertToString(vishnuId);
  DatabaseResult *result;

  std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+Vishnuid::mvishnuid);
  std::cout <<"SQL COMMAND:"<<sqlCommand<<std::endl;

  try {

    /*connection to the database*/
    mdatabaseVishnu->connect();

    UMSMapper* mapper = new UMSMapper(MapperRegistry::getInstance(), utilServer::UMSMAPPERNAME);
    mapper->registerMapper();

    /* Checking of vishnuid on the database */
    result = mdatabaseVishnu->getResult(sqlCommand.c_str());
    std::cout.flush();
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;

    }

  } catch (VishnuException& e) {
        std::cout << "Error: " << e.getMsg()<<std::endl;
        std::cout << e.what() <<std::endl;
        exit(0);
  }

  // initialization of the service table
  diet_service_table_init(NB_SRV);

  /* solveSessionConnect */

  mprofile = diet_profile_desc_alloc(SRV[0], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSessionConnect)) {
    return 1;
  }

  /* solveSessionReconnect */

  mprofile = diet_profile_desc_alloc(SRV[1], 4, 4, 6);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSessionReconnect)) {
    return 1;
  }

  /* solveSessionClose */

  mprofile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSessionClose)) {
    return 1;
  }

  /* solveUserCreate */

  mprofile = diet_profile_desc_alloc(SRV[3], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserCreate)) {
    return 1;
  }

  /* solveUserUpdate */

  mprofile = diet_profile_desc_alloc(SRV[4], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserUpdate)) {
    return 1;
  }

  /* solveUserDelete */

  mprofile = diet_profile_desc_alloc(SRV[5], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserDelete)) {
    return 1;
  }

  /* solveUserPasswordChange */

  mprofile = diet_profile_desc_alloc(SRV[6], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserPasswordChange)) {
    return 1;
  }

  /* solveUserPasswordReset */

  mprofile = diet_profile_desc_alloc(SRV[7], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserPasswordReset)) {
    return 1;
  }

  /* solveMachineCreate */

  mprofile = diet_profile_desc_alloc(SRV[8], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveMachineCreate)) {
    return 1;
  }

  /* solveMachineUpdate */

  mprofile = diet_profile_desc_alloc(SRV[9], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveMachineUpdate)) {
    return 1;
  }

  /* solveMachineDelete */

  mprofile = diet_profile_desc_alloc(SRV[10], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveMachineDelete)) {
    return 1;
  }

  /* solveLocalAccountCreate */

  mprofile = diet_profile_desc_alloc(SRV[11], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLocalAccountCreate)) {
    return 1;
  }

  /* solveLocalAccountUpdate */

  mprofile = diet_profile_desc_alloc(SRV[12], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLocalAccountUpdate)) {
    return 1;
  }

  /* solveLocalAccountDelete */

  mprofile = diet_profile_desc_alloc(SRV[13], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLocalAccountDelete)) {
    return 1;
  }

  /* solveConfigurationSave */

  mprofile = diet_profile_desc_alloc(SRV[14], 0, 0, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveConfigurationSave)) {
    return 1;
  }

  /* solveConfigurationRestore */

  mprofile = diet_profile_desc_alloc(SRV[15], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveConfigurationRestore)) {
    return 1;
  }

  /* solveOptionValueSet */

  mprofile = diet_profile_desc_alloc(SRV[16], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveOptionValueSet)) {
    return 1;
  }

  /* solveOptionValueSetDefault */

  mprofile = diet_profile_desc_alloc(SRV[17], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveOptionValueSetDefault)) {
    return 1;
  }

  /* solveListSessions */

  mprofile = diet_profile_desc_alloc(SRV[18], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListSessions)) {
    return 1;
  }

  /* solveListLocalAccount */

  mprofile = diet_profile_desc_alloc(SRV[19], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListLocalAccount)) {
    return 1;
  }

  /* solveListMachines */

  mprofile = diet_profile_desc_alloc(SRV[20], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListMachines)) {
    return 1;
  }

  /* solveListHistoryCmd */

  mprofile = diet_profile_desc_alloc(SRV[21], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListHistoryCmd)) {
    return 1;
  }

  /* solveListOptions */

  mprofile = diet_profile_desc_alloc(SRV[22], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListOptions)) {
    return 1;
  }

  /* solveListUsres */

  mprofile = diet_profile_desc_alloc(SRV[23], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListUsers)) {
    return 1;
  }

  /* solveRestore */

  mprofile = diet_profile_desc_alloc(SRV[24], 0, 0, 0);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  if (diet_service_table_add(mprofile, NULL, solveRestore)) {
    return 1;
  }

  diet_profile_desc_free(mprofile);

  return 0;
}

/**
* \fn ~ServerUMS()
* \brief Destructor, raises an exception on error
*/
ServerUMS::~ServerUMS() {
}

