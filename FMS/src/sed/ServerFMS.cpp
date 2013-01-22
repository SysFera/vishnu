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

  functionPtr = solveCreateDir;
  mcb["DirCreate"] = functionPtr;
  functionPtr = solveCreateFile;
  mcb["FileCreate"] = functionPtr;
  functionPtr = tailFile;
  mcb["FileTail"] = functionPtr;
  functionPtr = get_infos;
  mcb["FileGetInfos"] = functionPtr;
  functionPtr = contentFile;
  mcb["FileContent"] = functionPtr;
  functionPtr = headFile;
  mcb["FileHead"] = functionPtr;
  functionPtr = solveRemoveFile;
  mcb["FileRemove"] = functionPtr;
  functionPtr = solveRemoveDir;
  mcb["DirRemove"] = functionPtr;
  functionPtr = solveChangeGroup;
  mcb["FileChangeGroup"] = functionPtr;
  functionPtr = solveChangeMode;
  mcb["FileChangeMode"] = functionPtr;
  functionPtr = solveListDir;
  mcb["DirList"] = functionPtr;
  functionPtr = solveTransferFile<File::copy,File::sync>;
  mcb["FileCopy"] = functionPtr;
  functionPtr = solveTransferRemoteFile<File::copy,File::sync>;
  mcb["RemoteFileCopy"] = functionPtr;
  functionPtr = solveTransferFile<File::move,File::sync>;
  mcb["FileMove"] = functionPtr;
  functionPtr = solveTransferRemoteFile<File::move,File::sync>;
  mcb["RemoteFileMove"] = functionPtr;
  functionPtr = solveTransferFile<File::copy,File::async>;
  mcb["FileCopyAsync"] = functionPtr;
  functionPtr = solveTransferRemoteFile<File::copy,File::async>;
  mcb["RemoteFileCopyAsync"] = functionPtr;
  functionPtr = solveTransferFile<File::move,File::async>;
  mcb["FileMoveAsync"] = functionPtr;
  functionPtr = solveTransferRemoteFile<File::move,File::async>;
  mcb["RemoteFileMoveAsync"] = functionPtr;
  functionPtr = solveGetListOfFileTransfers;
  mcb["FileTransfersList"] = functionPtr;
  functionPtr = solveFileTransferStop;
  mcb["FileTransferStop"] = functionPtr;
}
