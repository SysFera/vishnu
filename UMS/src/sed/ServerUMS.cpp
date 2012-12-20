/**
 * \file ServerUMS.cpp
 * \brief This file presents the implementation of the UMS server.
 * \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
 * \date 31/01/2001
 */

#include "ServerUMS.hpp"
#include "internalApi.hpp"
#include "Authenticator.hpp"
#include "AuthenticatorFactory.hpp"

//{{RELAX<MISRA_0_1_3> Because these variables are used in this class
Database *ServerUMS::mdatabaseVishnu = NULL;
ServerUMS *ServerUMS::minstance = NULL;
UMSMapper *ServerUMS::mmapper = NULL;
TMSMapper *ServerUMS::mmapperTMS = NULL;
FMSMapper *ServerUMS::mmapperFMS = NULL;
IMSMapper *ServerUMS::mmapperIMS = NULL;
Authenticator *ServerUMS::mauthenticator = NULL;

//}}RELAX<MISRA_0_1_3>

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
ServerUMS::ServerUMS() {
}

/**
* \brief To initialize the UMS server with individual parameters
* \fn int init(std::string vishnuid)
* \param vishnuId The id of the vishnu configuration registered in the database
* \param dbConfig  The configuration of the database
* \param sendmailScriptPath The path to the script for sending emails
* \param authenticatorConfig The configuration of the authenticator
* \return an error code (0 if success and 1 if an error occurs)
*/
int
ServerUMS::init(int vishnuId,
                DbConfiguration dbConfig,
                std::string sendmailScriptPath,
                AuthenticatorConfiguration authenticatorConfig) {

  msendmailScriptPath = sendmailScriptPath;

  DbFactory factory;
  AuthenticatorFactory authfactory;

  try {
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
    mauthenticator = authfactory.createAuthenticatorInstance(authenticatorConfig);

    mvishnuId = vishnuId;

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));

    /*connection to the database*/
    mdatabaseVishnu->connect();
    mmapperTMS = new TMSMapper(MapperRegistry::getInstance(), TMSMAPPERNAME);
    mmapperTMS->registerMapper();
    mmapperIMS = new IMSMapper(MapperRegistry::getInstance(), IMSMAPPERNAME);
    mmapperIMS->registerMapper();
    mmapperFMS = new FMSMapper(MapperRegistry::getInstance(), FMSMAPPERNAME);
    mmapperFMS->registerMapper();
    mmapper = new UMSMapper(MapperRegistry::getInstance(), UMSMAPPERNAME);
    mmapper->registerMapper();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }

  } catch (VishnuException& e) {
      std::cout << e.what() << "\n";
      exit(0);
  }

// initialization of the service table
  initMap();

  return 0;
}

/**
* \fn ~ServerUMS()
* \brief Destructor, raises an exception on error
*/
ServerUMS::~ServerUMS() {
  if (mmapper != NULL) {
    delete mmapper;
  }
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
  if (mauthenticator != NULL) {
    delete mauthenticator;
  }
}

void
ServerUMS::initMap() {
  int (*functionPtr)(diet_profile_t*);

  functionPtr = solveSessionConnect;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[0]),functionPtr));
  functionPtr = solveSessionReconnect;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[1]),functionPtr));
  functionPtr = solveSessionClose;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[2]),functionPtr));
  functionPtr = solveUserCreate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[3]),functionPtr));
  functionPtr = solveUserUpdate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[4]),functionPtr));
  functionPtr = solveUserDelete;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[5]),functionPtr));
  functionPtr = solveUserPasswordChange;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[6]),functionPtr));
  functionPtr = solveUserPasswordReset;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[7]),functionPtr));
  functionPtr = solveMachineCreate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[8]),functionPtr));
  functionPtr = solveMachineUpdate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[9]),functionPtr));
  functionPtr = solveMachineDelete;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[10]),functionPtr));
  functionPtr = solveLocalAccountCreate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[11]),functionPtr));
  functionPtr = solveLocalAccountUpdate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[12]),functionPtr));
  functionPtr = solveLocalAccountDelete;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[13]),functionPtr));
  functionPtr = solveConfigurationSave;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[14]),functionPtr));
  functionPtr = solveConfigurationRestore;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[15]),functionPtr));
  functionPtr = solveOptionValueSet;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[16]),functionPtr));
  functionPtr = solveOptionValueSetDefault;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[17]),functionPtr));
  functionPtr = solveListSessions;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[18]),functionPtr));
  functionPtr = solveListLocalAccount;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[19]),functionPtr));
  functionPtr = solveListMachines;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[20]),functionPtr));
  functionPtr = solveListHistoryCmd;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[21]),functionPtr));
  functionPtr = solveListOptions;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[22]),functionPtr));
  functionPtr = solveListUsers;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[23]),functionPtr));
  functionPtr = solveRestore;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[24]),functionPtr));
  functionPtr = solveSystemAuthCreate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[25]),functionPtr));
  functionPtr = solveSystemAuthUpdate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[26]),functionPtr));
  functionPtr = solveSystemAuthDelete;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[27]),functionPtr));
  functionPtr = solveSystemAuthList;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[28]),functionPtr));
  functionPtr = solveAccountAuthCreate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[29]),functionPtr));
  functionPtr = solveAccountAuthUpdate;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[30]),functionPtr));
  functionPtr = solveAccountAuthDelete;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[31]),functionPtr));
  functionPtr = solveAccountAuthList;
  mcb.insert( pair<string, functionPtr_t> (string(SRV[32]),functionPtr));

}
