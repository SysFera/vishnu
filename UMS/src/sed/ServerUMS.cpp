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
#include "UMSServices.hpp"

//{{RELAX<MISRA_0_1_3> Because these variables are used in this class
Database *ServerUMS::mdatabaseVishnu = NULL;
ServerUMS *ServerUMS::minstance = NULL;
UMSMapper *ServerUMS::mmapper = NULL;
TMSMapper *ServerUMS::mmapperTMS = NULL;
FMSMapper *ServerUMS::mmapperFMS = NULL;
IMSMapper *ServerUMS::mmapperIMS = NULL;
Authenticator *ServerUMS::mauthenticator = NULL;

//}}RELAX<MISRA_0_1_3>

using namespace vishnu;


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
  mcb[SERVICES_UMS[SESSIONCONNECT]] = functionPtr;
  functionPtr = solveSessionReconnect;
  mcb[SERVICES_UMS[SESSIONRECONNECT]] = functionPtr;
  functionPtr = solveSessionClose;
  mcb[SERVICES_UMS[SESSIONCLOSE]] = functionPtr;
  functionPtr = solveUserCreate;
  mcb[SERVICES_UMS[USERCREATE]] = functionPtr;
  functionPtr = solveUserUpdate;
  mcb[SERVICES_UMS[USERUPDATE]] = functionPtr;
  functionPtr = solveUserDelete;
  mcb[SERVICES_UMS[USERDELETE]] = functionPtr;
  functionPtr = solveUserPasswordChange;
  mcb[SERVICES_UMS[USERPASSWORDCHANGE]] = functionPtr;
  functionPtr = solveUserPasswordReset;
  mcb[SERVICES_UMS[USERPASSWORDRESET]] = functionPtr;
  functionPtr = solveMachineCreate;
  mcb[SERVICES_UMS[MACHINECREATE]] = functionPtr;
  functionPtr = solveMachineUpdate;
  mcb[SERVICES_UMS[MACHINEUPDATE]] = functionPtr;
  functionPtr = solveMachineDelete;
  mcb[SERVICES_UMS[MACHINEDELETE]] = functionPtr;
  functionPtr = solveLocalAccountCreate;
  mcb[SERVICES_UMS[LOCALACCOUNTCREATE]] = functionPtr;
  functionPtr = solveLocalAccountUpdate;
  mcb[SERVICES_UMS[LOCALACCOUNTUPDATE]] = functionPtr;
  functionPtr = solveLocalAccountDelete;
  mcb[SERVICES_UMS[LOCALACCOUNTDELETE]] = functionPtr;
  functionPtr = solveConfigurationSave;
  mcb[SERVICES_UMS[CONFIGURATIONSAVE]] = functionPtr;
  functionPtr = solveConfigurationRestore;
  mcb[SERVICES_UMS[CONFIGURATIONRESTORE]] = functionPtr;
  functionPtr = solveOptionValueSet;
  mcb[SERVICES_UMS[OPTIONVALUESET]] = functionPtr;
  functionPtr = solveOptionValueSetDefault;
  mcb[SERVICES_UMS[OPTIONVALUESETDEFAULT]] = functionPtr;
  functionPtr = solveListSessions;
  mcb[SERVICES_UMS[SESSIONLIST]] = functionPtr;
  functionPtr = solveListLocalAccount;
  mcb[SERVICES_UMS[LOCALACCOUNTLIST]] = functionPtr;
  functionPtr = solveListMachines;
  mcb[SERVICES_UMS[MACHINELIST]] = functionPtr;
  functionPtr = solveListHistoryCmd;
  mcb[SERVICES_UMS[COMMANDLIST]] = functionPtr;
  functionPtr = solveListOptions;
  mcb[SERVICES_UMS[OPTIONVALUELIST]] = functionPtr;
  functionPtr = solveListUsers;
  mcb[SERVICES_UMS[USERLIST]] = functionPtr;
  functionPtr = solveRestore;
  mcb[SERVICES_UMS[RESTORE]] = functionPtr;
  functionPtr = solveSystemAuthCreate;
  mcb[SERVICES_UMS[AUTHSYSTEMCREATE]] = functionPtr;
  functionPtr = solveSystemAuthUpdate;
  mcb[SERVICES_UMS[AUTHSYSTEMUPDATE]] = functionPtr;
  functionPtr = solveSystemAuthDelete;
  mcb[SERVICES_UMS[AUTHSYSTEMDELETE]] = functionPtr;
  functionPtr = solveSystemAuthList;
  mcb[SERVICES_UMS[AUTHSYSTEMLIST]] = functionPtr;
  functionPtr = solveAccountAuthCreate;
  mcb[SERVICES_UMS[AUTHACCOUNTCREATE]] = functionPtr;
  functionPtr = solveAccountAuthUpdate;
  mcb[SERVICES_UMS[AUTHACCOUNTUPDATE]] = functionPtr;
  functionPtr = solveAccountAuthDelete;
  mcb[SERVICES_UMS[AUTHACCOUNTDELETE]] = functionPtr;
  functionPtr = solveAccountAuthList;
  mcb[SERVICES_UMS[AUTHACCOUNTLIST]] = functionPtr;
}
