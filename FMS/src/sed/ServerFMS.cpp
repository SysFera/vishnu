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
  mcb.insert( pair<string, functionPtr_t> (string("DirCreate"),functionPtr));
  functionPtr = solveCreateFile;
  mcb.insert( pair<string, functionPtr_t> (string("FileCreate"),functionPtr));
  functionPtr = tailFile;
  mcb.insert( pair<string, functionPtr_t> (string("FileTail"),functionPtr));
  functionPtr = get_infos;
  mcb.insert( pair<string, functionPtr_t> (string("FileGetInfos"),functionPtr));
  functionPtr = contentFile;
  mcb.insert( pair<string, functionPtr_t> (string("FileContent"),functionPtr));
  functionPtr = headFile;
  mcb.insert( pair<string, functionPtr_t> (string("FileHead"),functionPtr));
  functionPtr = solveRemoveFile;
  mcb.insert( pair<string, functionPtr_t> (string("FileRemove"),functionPtr));
  functionPtr = solveRemoveDir;
  mcb.insert( pair<string, functionPtr_t> (string("DirRemove"),functionPtr));
  functionPtr = solveChangeGroup;
  mcb.insert( pair<string, functionPtr_t> (string("FileChangeGroup"),functionPtr));
  functionPtr = solveChangeMode;
  mcb.insert( pair<string, functionPtr_t> (string("FileChangeMode"),functionPtr));
  functionPtr = solveListDir;
  mcb.insert( pair<string, functionPtr_t> (string("DirList"),functionPtr));
  functionPtr = solveTransferFile<File::copy,File::sync>;
  mcb.insert( pair<string, functionPtr_t> (string("FileCopy"),functionPtr));
  functionPtr = solveTransferRemoteFile<File::copy,File::sync>;
  mcb.insert( pair<string, functionPtr_t> (string("RemoteFileCopy"),functionPtr));
  functionPtr = solveTransferFile<File::move,File::sync>;
  mcb.insert( pair<string, functionPtr_t> (string("FileMove"),functionPtr));
  functionPtr = solveTransferRemoteFile<File::move,File::sync>;
  mcb.insert( pair<string, functionPtr_t> (string("RemoteFileMove"),functionPtr));
  functionPtr = solveTransferFile<File::copy,File::async>;
  mcb.insert( pair<string, functionPtr_t> (string("FileCopyAsync"),functionPtr));
  functionPtr = solveTransferRemoteFile<File::copy,File::async>;
  mcb.insert( pair<string, functionPtr_t> (string("RemoteFileCopyAsync"),functionPtr));
  functionPtr = solveTransferFile<File::move,File::async>;
  mcb.insert( pair<string, functionPtr_t> (string("FileMoveAsync"),functionPtr));
  functionPtr = solveTransferRemoteFile<File::move,File::async>;
  mcb.insert( pair<string, functionPtr_t> (string("RemoteFileMoveAsync"),functionPtr));
  functionPtr = solveGetListOfFileTransfers;
  mcb.insert( pair<string, functionPtr_t> (string("FileTransfersList"),functionPtr));
  functionPtr = solveFileTransferStop;
  mcb.insert( pair<string, functionPtr_t> (string("FileTransferStop"),functionPtr));
}
