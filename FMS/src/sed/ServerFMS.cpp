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
ServerFMS::init(int vishnuId,
                DbConfiguration dbConfig) {


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

//  // initialization of the service table
//  diet_service_table_init(NB_SRV);
//
//  // getFileInfos
//
//  mprofile = getInfosProfile();
//
//  if(diet_service_table_add(mprofile, NULL, get_infos)){
//
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  // content of file
//
//  mprofile = getContentProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, contentFile)){
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//
//  // Head of file
//
//  mprofile = getHeadProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, headFile)){
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//  // Tail of file
//
//  mprofile = getTailProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, tailFile)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//
//  // Create file
//
//  mprofile = getCreateFileProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveCreateFile)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Create dir
//
//  mprofile = getCreateDirProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveCreateDir)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//
//  // Remove file
//
//  mprofile = getRemoveFileProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveRemoveFile)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Remove dir
//
//  mprofile = getRemoveDirProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveRemoveDir)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Change group file
//
//  mprofile = getChangeGroupProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveChangeGroup)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Change mode file
//
//  mprofile = getChangeModeProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveChangeMode)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// List the directory content
//
//  mprofile = getListDirProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveListDir)){
//
//    return 1;
//
//  }
//
//  diet_profile_desc_free(mprofile);
//
//
//// Copy file
//
//  mprofile = getTransferFileProfile("FileCopy");
//
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferFile<File::copy,File::sync>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Copy Remote file
//
//  mprofile = getTransferRemoteFileProfile("RemoteFileCopy");
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferRemoteFile<File::copy,File::sync>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Move file
//
//  mprofile = getTransferFileProfile("FileMove");
//
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferFile<File::move,File::sync>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Move Remote file
//
//  mprofile = getTransferRemoteFileProfile("RemoteFileMove");
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferRemoteFile<File::move,File::sync>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//// Asynchronous command area
//
//// CopyAsync file
//
//  mprofile = getTransferFileAsyncProfile("FileCopyAsync");
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferFile<File::copy,File::async>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//  // CopyAsync Remote file
//
//  mprofile = getTransferRemoteFileAsyncProfile("RemoteFileCopyAsync");
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferRemoteFile<File::copy,File::async>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//
//  // Move Async file
//
//  mprofile = getTransferFileAsyncProfile("FileMoveAsync");
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferFile<File::move,File::async>)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//
//
//
//  // MoveAsync Remote file
//
//  mprofile = getTransferRemoteFileAsyncProfile("RemoteFileMoveAsync");
//
//
//  if ( diet_service_table_add(mprofile, NULL, solveTransferRemoteFile<File::move,File::async>)){
//
//    return 1;
//  }
//
//
//
//  // List file transfer
//
//  mprofile = getFileTransfersListProfile();
//
//  if ( diet_service_table_add(mprofile, NULL, solveGetListOfFileTransfers)){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);
//
//  // Stop file transfer
//
//  mprofile = getFileTransferStopProfile();
//
//  if ( diet_service_table_add(mprofile, NULL,solveFileTransferStop ) ){
//
//    return 1;
//  }
//
//  diet_profile_desc_free(mprofile);



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
  mcb.insert( pair<string, functionPtr_t> (string("DirRemove"),functionPtr));
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

int
ServerFMS::call(diet_profile_t* profile){
  map<string, functionPtr_t>::iterator it;
  it = mcb.find(string(profile->name));
  if (it == mcb.end()) {
    std::cerr << "not found service : " << profile->name << std::endl;
    return UNKNOWN_SERVICE;
  }
  int (*functionPtr)(diet_profile_t*);
  functionPtr = it->second;
  return (*functionPtr)(profile);
}
