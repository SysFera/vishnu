/**
 * \file ServerIMS.cpp
 * \brief This file presents the implementation of the IMS server.
 * \author Kevin COULOMB (kevin.coulomb@sysfera.com)
 * \date 12/03/2011
 */

#include <boost/scoped_ptr.hpp>

#include "ServerIMS.hpp"
#include "utilServer.hpp"
#include "IMSMapper.hpp"
#include "MapperRegistry.hpp"
#include "internalApiIMS.hpp"
#include "DbFactory.hpp"


using namespace vishnu;

// {{RELAX<MISRA_0_1_3> Three static variables
ServerIMS *ServerIMS::minstance       = NULL;
Database  *ServerIMS::mdatabaseVishnu = NULL;
UMSMapper *ServerIMS::mmapperUMS      = NULL;
TMSMapper *ServerIMS::mmapperTMS      = NULL;
FMSMapper *ServerIMS::mmapperFMS      = NULL;
IMSMapper *ServerIMS::mmapperIMS      = NULL;
// }}RELAX<MISRA_0_1_3>

ServerIMS*
ServerIMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerIMS;
  }
  return minstance;
}

/**
 * \brief To get the vishnuId
 * \fn int getVishnuId()
 * \return the path of the configuration file
 */
int
ServerIMS::getVishnuId() {
  return mvishnuId;
}

/**
 * \brief To get the path to the sendmail script
 * \fn std::string getSendmailScriptPath()
 * \return the path of the configuration file
 */
std::string
ServerIMS::getSendmailScriptPath() {
  return msendmailScriptPath;
}

/**
 * \brief Constructor (private)
 * \fn ServerIMS()
 */
ServerIMS::ServerIMS()  {;}


int
ServerIMS::init(int vishnuId,
		DbConfiguration dbConfig,
		std::string sendmailScriptPath,
		std::string mid) {

  msendmailScriptPath = sendmailScriptPath;

  DbFactory factory;
  mvishnuId = vishnuId;

  string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));


  try {
    //To get database instance
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);

    /*connection to the database*/
    mdatabaseVishnu->connect();
    static const int hostsize = 200;
    char hostname[hostsize];
    if (gethostname(hostname, hostsize-1) == -1){
      throw SystemException(ERRCODE_SYSTEM,
                            "Cannot get hostname to check process");
    }

    /*Mapper registration*/
    mmapperTMS = new TMSMapper(MapperRegistry::getInstance(), TMSMAPPERNAME);
    mmapperTMS->registerMapper();
    mmapperIMS = new IMSMapper(MapperRegistry::getInstance(), IMSMAPPERNAME);
    mmapperIMS->registerMapper();
    mmapperFMS = new FMSMapper(MapperRegistry::getInstance(), FMSMAPPERNAME);
    mmapperFMS->registerMapper();
    mmapperUMS = new UMSMapper(MapperRegistry::getInstance(), UMSMAPPERNAME);
    mmapperUMS->registerMapper();
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
  initMap(mid);

  return 0;
}

/**
* \fn ~ServerIMS()
* \brief Destructor, raises an exception on error
*/
ServerIMS::~ServerIMS() {
  if (mmapperUMS != NULL) {
    delete mmapperUMS;
  }
  if (mmapperTMS != NULL) {
    delete mmapperTMS;
  }
  if (mmapperFMS != NULL) {
    delete mmapperFMS;
  }
  if (mmapperIMS != NULL) {
    delete mmapperIMS;
  }
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
}

void
ServerIMS::initMap(std::string mid) {
  mcb[SERVICES_IMS[INT_EXPORTCOMMANDS]] = solveExport;
  mcb[std::string(SERVICES_IMS[INT_GETMETRICCURENTVALUE])+"@"+mid] = solveCurMetric;
  mcb[SERVICES_IMS[INT_GETMETRICHISTORY]] = solveOldMetric;
  mcb[SERVICES_IMS[INT_GETPROCESSES]] = solvePS;
  mcb[SERVICES_IMS[INT_SETSYSTEMINFO]] = solveSetSysInfo;
  mcb[SERVICES_IMS[INT_SETSYSTEMTHRESHOLD]] = solveSetThreshold;
  mcb[SERVICES_IMS[INT_GETSYSTEMTHRESHOLD]] = solveGetThreshold;
  mcb[SERVICES_IMS[INT_DEFINEUSERIDENTIFIER]] = solveSetUID;
  mcb[SERVICES_IMS[INT_DEFINEJOBIDENTIFIER]] = solveSetJID;
  mcb[SERVICES_IMS[INT_DEFINETRANSFERIDENTIFIER]] = solveSetTID;
  mcb[SERVICES_IMS[INT_DEFINEMACHINEIDENTIFIER]] = solveSetMID;
  mcb[std::string(SERVICES_IMS[INT_LOADSHED])+"@"+mid] = solveLoadShed;
  mcb[SERVICES_IMS[INT_SETUPDATEFREQUENCY]] = solveGetUpFreq;
  mcb[SERVICES_IMS[INT_GETUPDATEFREQUENCY]] = solveSetUpFreq;
  mcb[std::string(SERVICES_IMS[INT_RESTART])+"@"+mid] = solveRestart;
  mcb[std::string(SERVICES_IMS[INT_STOP])+"@"+mid] = solveStop;
  mcb[SERVICES_IMS[INT_GETSYSTEMINFO]] = solveGetSysInfo;
  mcb[SERVICES_IMS[INT_DEFINEAUTHIDENTIFIER]] = solveSetAID;
  mcb[SERVICES_IMS[INT_DEFINEWORKIDENTIFIER]] = solveSetWID;
}
