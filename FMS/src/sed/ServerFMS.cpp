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
  mcb[SERVICES_FMS[FILECOPYASYNC]] = functionPtr;

  functionPtr = solveTransferFile<File::move,File::async>;
  mcb[SERVICES_FMS[FILEMOVEASYNC]] = functionPtr;

  functionPtr = solveTransferFile<File::move,File::sync>;
  mcb[SERVICES_FMS[FILEMOVE]] = functionPtr;

  functionPtr = solveTransferFile<File::copy,File::sync>;
  mcb[SERVICES_FMS[FILECOPY]] = functionPtr;

  functionPtr = get_infos;
  mcb[SERVICES_FMS[FILEGETINFOS]] = functionPtr;

  functionPtr = solveChangeGroup;
  mcb[SERVICES_FMS[FILECHANGEGROUP]] = functionPtr;

  functionPtr = solveChangeMode;
  mcb[SERVICES_FMS[FILECHANGEMODE]] = functionPtr;

  functionPtr = headFile;
  mcb[SERVICES_FMS[FILEHEAD]] = functionPtr;

  functionPtr = contentFile;
  mcb[SERVICES_FMS[FILECONTENT]] = functionPtr;

  functionPtr = solveCreateFile;
  mcb[SERVICES_FMS[FILECREATE]] = functionPtr;

  functionPtr = solveCreateDir;
  mcb[SERVICES_FMS[DIRCREATE]] = functionPtr;

  functionPtr = solveRemoveFile;
  mcb[SERVICES_FMS[FILEREMOVE]] = functionPtr;

  functionPtr = solveRemoveDir;
  mcb[SERVICES_FMS[DIRREMOVE]] = functionPtr;

  functionPtr = tailFile;
  mcb[SERVICES_FMS[FILETAIL]] = functionPtr;

  functionPtr = solveListDir;
  mcb[SERVICES_FMS[DIRLIST]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::copy,File::async>;
  mcb[SERVICES_FMS[REMOTEFILECOPYASYNC]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::move,File::async>;
  mcb[SERVICES_FMS[REMOTEFILEMOVEASYNC]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::copy,File::sync>;
  mcb[SERVICES_FMS[REMOTEFILECOPY]] = functionPtr;

  functionPtr = solveTransferRemoteFile<File::move,File::sync>;
  mcb[SERVICES_FMS[REMOTEFILEMOVE]] = functionPtr;

  functionPtr = solveGetListOfFileTransfers;
  mcb[SERVICES_FMS[FILETRANSFERSLIST]] = functionPtr;

  functionPtr = solveFileTransferStop;
  mcb[SERVICES_FMS[FILETRANSFERSTOP]] = functionPtr;
}
