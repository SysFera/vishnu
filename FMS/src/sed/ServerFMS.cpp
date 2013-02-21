/**
 * \file ServerFMS.cpp
 * \brief This file presents the implementation of the FMS server.
 * \author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * \date 16/05/2011
 */

#include "ServerFMS.hpp"
#include "utilVishnu.hpp"
#include "DbFactory.hpp"
#include <boost/scoped_ptr.hpp>
#include "SystemException.hpp"
#include "internalApiFMS.hpp"
#include "FMSServices.hpp"

using namespace vishnu;
using namespace FMS_Data;

// {{RELAX<MISRA_0_1_3> Three static variables
Database *ServerFMS::mdatabaseVishnu = NULL;
ServerFMS* ServerFMS::minstance = NULL;
FMSMapper *ServerFMS::mmapper = NULL;
// }}RELAX<MISRA_0_1_3>



/**
 * \brief To get the unique instance of the server
 */
ServerFMS*
ServerFMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerFMS();
  }
  return minstance;
}

/**
 * \brief To get the vishnuId
 * \return the path of the configuration file
 */
int
ServerFMS::getVishnuId() const {
  return mvishnuId;
}


/**
 * \brief Constructor (private)
 */
ServerFMS::ServerFMS()  {
}

/**
 * \brief To initialize the FMS server with individual parameters
 * \fn int init(std::string vishnuid)
 * \param vishnuId The id of the vishnu configuration registered in the database
 * \param dbConfig  The configuration of the database
 * \return an error code (0 if success and 1 if an error occurs)
 */
int
ServerFMS::init(int vishnuId, DbConfiguration dbConfig) {

  DbFactory factory;

  try {
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);

    mvishnuId = vishnuId;

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));

    /*connection to the database*/
    mdatabaseVishnu->connect();

    mmapper = new FMSMapper(MapperRegistry::getInstance(), FMSMAPPERNAME);
    mmapper->registerMapper();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }

  } catch (VishnuException& e) {
      std::cout << e.what() << std::endl;
      exit(0);
  }
  initMap();


  return 0;
}

/**
 * \brief Destructor, raises an exception on error
 */
ServerFMS::~ServerFMS() {
  if (mmapper != NULL) {
    delete mmapper;
  }
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
}

void
ServerFMS::initMap() {
  int (*functionPtr)(diet_profile_t*);

  functionPtr = solveTransferFile<File::copy,File::async>;
  mcb[SERVICES_FMS[0]] = functionPtr;

  functionPtr = solveTransferFile<File::move,File::async>;
  mcb[SERVICES_FMS[1]] = functionPtr;

  functionPtr = solveTransferFile<File::move,File::sync>;
  mcb[SERVICES_FMS[2]] = functionPtr;

  functionPtr = solveTransferFile<File::copy,File::sync>;
  mcb[SERVICES_FMS[3]] = functionPtr;

  functionPtr = get_infos;
  mcb[SERVICES_FMS[4]] = functionPtr;

  functionPtr = solveChangeGroup;
  mcb[SERVICES_FMS[5]] = functionPtr;

  functionPtr = solveChangeMode;
  mcb[SERVICES_FMS[6]] = functionPtr;

  functionPtr = headFile;
  mcb[SERVICES_FMS[7]] = functionPtr;

  functionPtr = contentFile;
  mcb[SERVICES_FMS[8]] = functionPtr;

  functionPtr = solveCreateFile;
  mcb[SERVICES_FMS[9]] = functionPtr;

  functionPtr = solveCreateDir;
  mcb[SERVICES_FMS[10]] = functionPtr;

  functionPtr = solveRemoveFile;
  mcb[SERVICES_FMS[11]] = functionPtr;

  functionPtr = solveRemoveDir;
  mcb[SERVICES_FMS[12]] = functionPtr;

  functionPtr = tailFile;
  mcb[SERVICES_FMS[13]] = functionPtr;

  functionPtr = solveListDir;
  mcb[SERVICES_FMS[14]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::copy,File::async>;
  mcb[SERVICES_FMS[15]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::move,File::async>;
  mcb[SERVICES_FMS[16]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::copy,File::sync>;
  mcb[SERVICES_FMS[17]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::move,File::sync>;
  mcb[SERVICES_FMS[18]] = functionPtr;

  functionPtr = solveGetListOfFileTransfers;
  mcb[SERVICES_FMS[19]] = functionPtr;

  functionPtr = solveFileTransferStop;
  mcb[SERVICES_FMS[20]] = functionPtr;
}
