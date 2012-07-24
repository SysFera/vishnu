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
  initMap();

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
ServerIMS::initMap() {
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[0], solveExport));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[1], solveCurMetric));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[2], solveOldMetric));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[3], solvePS));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[4], solveSetSysInfo));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[5], solveSetThreshold));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[6], solveGetThreshold));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[7], solveSetUID));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[8], solveSetJID));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[9], solveSetTID));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[10], solveSetMID));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[11], solveLoadShed));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[13], solveGetUpFreq));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[12], solveSetWID));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[13], solveLoadShed));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[14], solveRestart));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[15], solveStop));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[16], solveGetSysInfo));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[17], solveSetAID));
  mcb.insert(std::pair<std::string, functionPtr_t>(SRV[18], solveSetWID));
}
