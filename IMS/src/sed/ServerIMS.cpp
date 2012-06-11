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
    minstance = new ServerIMS();
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
ServerIMS::ServerIMS() : mprofile(NULL) {
}

int
ServerIMS::init(int vishnuId,
		DbConfiguration dbConfig,
		string sendmailScriptPath,
		string mid) {

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
    if (gethostname(hostname, hostsize-1)==-1){
      throw SystemException(ERRCODE_SYSTEM, "Cannot get hostname to check process");
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
  diet_service_table_init(NB_SRV);

  /* solveExport */

  mprofile = diet_profile_desc_alloc(SRV[0], 3, 3, 5);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveExport)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveCurMetric */

  mprofile = diet_profile_desc_alloc((SRV[1]+mid).c_str(), 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveCurMetric)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveOldMetric */

  mprofile = diet_profile_desc_alloc(SRV[2], 2, 2, 4);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveOldMetric)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solvePS */

  mprofile = diet_profile_desc_alloc(SRV[3], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solvePS)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetSysInfo */

  mprofile = diet_profile_desc_alloc(SRV[4], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetSysInfo)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetThreshold */

  mprofile = diet_profile_desc_alloc(SRV[5], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetThreshold)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveGetThreshold */

  mprofile = diet_profile_desc_alloc(SRV[6], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveGetThreshold)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetUID */

  mprofile = diet_profile_desc_alloc(SRV[7], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetUID)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetJIB */

  mprofile = diet_profile_desc_alloc(SRV[8], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetJID)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetTID */

  mprofile = diet_profile_desc_alloc(SRV[9], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetTID)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetMID */

  mprofile = diet_profile_desc_alloc(SRV[10], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetMID)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveLoadShed */

  mprofile = diet_profile_desc_alloc(SRV[11], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveLoadShed)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetUpFreq */

  mprofile = diet_profile_desc_alloc(SRV[12], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetUpFreq)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveGetUpFreq */

  mprofile = diet_profile_desc_alloc(SRV[13], 0, 0, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveGetUpFreq)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveRestart */

  mprofile = diet_profile_desc_alloc(SRV[14], 2, 2, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveRestart)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);


  /* solveStop */
  mprofile = diet_profile_desc_alloc(SRV[15], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveStop)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveGetSystemInfo */
  mprofile = diet_profile_desc_alloc(SRV[16], 1, 1, 3);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveGetSysInfo)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetAID */

  mprofile = diet_profile_desc_alloc(SRV[17], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetAID)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

  /* solveSetAID */

  mprofile = diet_profile_desc_alloc(SRV[18], 1, 1, 2);
  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);

  if (diet_service_table_add(mprofile, NULL, solveSetWID)) {
    return 1;
  }
  diet_profile_desc_free(mprofile);

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
