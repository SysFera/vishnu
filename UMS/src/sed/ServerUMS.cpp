/**
* \file ServerUMS.cpp
* \brief This file presents the implementation of the UMS server.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2001
*/

#include "ServerUMS.hpp"
#include "internalApi.hpp"

Database *ServerUMS::mdatabaseVishnu = NULL;
ServerUMS *ServerUMS::minstance = NULL;
UMSMapper *ServerUMS::mmapper = NULL;
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
* \brief To get the vishnuId
* \fn int getVishnuId()
* \return the path of the configuration file
*/
int
ServerUMS::getVishnuId() const {
  return mvishnuId;
}

/**
* \brief To get the path to the sendmail script
* \fn std::string getSendmailScriptPath()
* \return the path of the configuration file
*/
std::string
ServerUMS::getSendmailScriptPath() const {
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

  msendmailScriptPath = sendmailScriptPath;

  DbFactory factory;

  try {
    //To get database instance
    mdatabaseVishnu = factory.getDatabaseInstance(dbType,
                                                            dbHost,
                                                            dbUsername,
                                                            dbPassword,
                                                            DATABASENAME);

    mvishnuId = vishnuId;

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));

    /*connection to the database*/
    mdatabaseVishnu->connect();

    //UMSMapper* mapper = new UMSMapper(MapperRegistry::getInstance(), UMSMAPPERNAME);
    mmapper = new UMSMapper(MapperRegistry::getInstance(), UMSMAPPERNAME);
    mmapper->registerMapper();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }

  } catch (VishnuException& e) {
      std::cout << e.buildExceptionString() << std::endl;
      exit(0);
  }

  // initialization of the service table
  diet_service_table_init(NB_SRV);

  /* solveSessionConnect */

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
  diet_profile_desc_free(mprofile);

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
  diet_profile_desc_free(mprofile);

  /* solveSessionClose */

  mprofile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSessionClose)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
  /* solveUserCreate */

  mprofile = diet_profile_desc_alloc(SRV[3], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserCreate)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
  /* solveUserUpdate */

  mprofile = diet_profile_desc_alloc(SRV[4], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserUpdate)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveUserDelete */

  mprofile = diet_profile_desc_alloc(SRV[5], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserDelete)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveUserPasswordChange */

  mprofile = diet_profile_desc_alloc(SRV[6], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserPasswordChange)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveUserPasswordReset */

  mprofile = diet_profile_desc_alloc(SRV[7], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveUserPasswordReset)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveMachineCreate */

  mprofile = diet_profile_desc_alloc(SRV[8], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveMachineCreate)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
  /* solveMachineUpdate */

  mprofile = diet_profile_desc_alloc(SRV[9], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveMachineUpdate)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveMachineDelete */

  mprofile = diet_profile_desc_alloc(SRV[10], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveMachineDelete)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveLocalAccountCreate */

  mprofile = diet_profile_desc_alloc(SRV[11], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLocalAccountCreate)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveLocalAccountUpdate */

  mprofile = diet_profile_desc_alloc(SRV[12], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLocalAccountUpdate)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveLocalAccountDelete */

  mprofile = diet_profile_desc_alloc(SRV[13], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLocalAccountDelete)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveConfigurationSave */

  mprofile = diet_profile_desc_alloc(SRV[14], 0, 0, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveConfigurationSave)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveConfigurationRestore */

  mprofile = diet_profile_desc_alloc(SRV[15], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveConfigurationRestore)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveOptionValueSet */

  mprofile = diet_profile_desc_alloc(SRV[16], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveOptionValueSet)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveOptionValueSetDefault */

  mprofile = diet_profile_desc_alloc(SRV[17], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveOptionValueSetDefault)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveListSessions */

  mprofile = diet_profile_desc_alloc(SRV[18], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListSessions)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveListLocalAccount */

  mprofile = diet_profile_desc_alloc(SRV[19], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListLocalAccount)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveListMachines */

  mprofile = diet_profile_desc_alloc(SRV[20], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListMachines)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveListHistoryCmd */

  mprofile = diet_profile_desc_alloc(SRV[21], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListHistoryCmd)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveListOptions */

  mprofile = diet_profile_desc_alloc(SRV[22], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListOptions)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
  /* solveListUsres */

  mprofile = diet_profile_desc_alloc(SRV[23], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveListUsers)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);
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
  if (minstance != NULL) {
    delete minstance;
  }
  if (mmapper != NULL) {
    delete mmapper;
  }
}

