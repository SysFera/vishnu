/**
 * \file ListFileTransfers.hpp
 * \brief This file contains the VISHNU ListFileTransfers class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * \date May 2011
 */
#ifndef _LIST_FILE_TRANSFERS_SERVER_
#define _LIST_FILE_TRANSFERS_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "QueryServer.hpp"
#include "SessionServer.hpp"
#include "FMS_Data.hpp"
#include "FileTypes.hpp"
#include "utilVishnu.hpp"
/**
 * \class ListFileTransfers
 * \brief ListFileTransfers class implementation
 */
class ListFileTransfers: public QueryServer<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList> {

public:

  /**
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListFileTransfers(const SessionServer session):
    QueryServer<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList>(session) {
   mcommandName = "vishnu_list_file_transfers";
  }
  /**
   * \param params The object which encapsulates the information of ListFileTransfers options
   * \param session The object which encapsulates the session information (ex: identifier of the session)
   * \brief Constructor, raises an exception on error
   */
  ListFileTransfers(FMS_Data::LsTransferOptions_ptr params, const SessionServer& session):
    QueryServer<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList>(params, session) {
    mcommandName = "vishnu_list_file_transfers";
  }

  /**
   * \brief Function to treat the listFile transfer options
   * \param options the object which contains the ListFileTransfers options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(const FMS_Data::LsTransferOptions_ptr& options, std::string& sqlRequest) {

    bool onlyProgressFile = true;

    //To check if the transferId is defined
    if (options->getTransferId().size() != 0) {
      //To check the transfer Id
      checkTransferId(options->getTransferId());
      //To add the transferId on the request
      addOptionRequest("transferid", options->getTransferId(), sqlRequest);
      onlyProgressFile = false;
    }

    //To check if the fromMachineId is defined
    if (options->getFromMachineId().size() != 0) {
      //To add the fromMachineId on the request
      
      sqlRequest.append(" and (sourceMachineId='"+options->getFromMachineId()+"'"+" or destinationMachineId='"+options->getFromMachineId()+"')");
      
      onlyProgressFile = false;
    }

    //To check if the userId is defined
    if (options->getUserId().size() != 0) {

      //Creation of the object user
      UserServer userServer = UserServer(msessionServer);   

      userServer.init();
      if (!userServer.isAdmin()) {
        UMSVishnuException e (ERRCODE_NO_ADMIN);
        throw e;
      } 
    
      //To check the user Id
      checkUserId(options->getUserId());
      //To add the userId on the request
      addOptionRequest("userId", options->getUserId(), sqlRequest);
      onlyProgressFile = false;
    }

    //To check the file transfer status
    if (options->getStatus() != 4) { // UNDEFINED FILE TRANSFER STATUS
      //To check the file status
      checkStatus(options->getStatus());
      //To add the status on the request
      addOptionRequest("status", convertToString(options->getStatus()), sqlRequest);
    } else {
      if(onlyProgressFile) {
        addOptionRequest("status", "0", sqlRequest);
      }
    }

  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  FMS_Data::FileTransferList*
    list() {
  
      std::string sqlListOfFiles = "SELECT transferId, filetransfer.status, userId, clientMachineId, sourceMachineId, "
        "destinationMachineId, sourceFilePath, destinationFilePath, fileSize, startTime,errorMsg,"
        " trCommand from filetransfer, vsession "
          "where vsession.numsessionid=filetransfer.vsession_numsessionid";

      std::vector<std::string>::iterator iter;
      std::vector<std::string> results;

      FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();
      mlistObject = ecoreFactory->createFileTransferList();

      msessionServer.check();

      processOptions(mparameters, sqlListOfFiles);
      sqlListOfFiles.append(" order by startTime");

      
      boost::scoped_ptr<DatabaseResult> ListOfFiles (mdatabaseVishnu->getResult(sqlListOfFiles.c_str()));
      
      time_t startTime;
        

      if (ListOfFiles->getNbTuples() != 0){
      
        
        for (size_t i = 0; i < ListOfFiles->getNbTuples(); ++i) {
          results.clear();
          results = ListOfFiles->get(i);
          iter = results.begin();

          FMS_Data::FileTransfer_ptr filetransfer = ecoreFactory->createFileTransfer();

          
          filetransfer->setTransferId(*iter);
          int trStatus=convertToInt(*(++iter));
          filetransfer->setStatus((trStatus >=0&& trStatus<5 ? trStatus:4));
          filetransfer->setUserId(*(++iter));
          filetransfer->setClientMachineId(*(++iter));
          filetransfer->setSourceMachineId(*(++iter));
          filetransfer->setDestinationMachineId(*(++iter));
          filetransfer->setSourceFilePath(*(++iter));
          filetransfer->setDestinationFilePath(*(++iter));
          //std::istringstream  iss(*(++iter));
         // file_size_t fileSize;

          //iss >> fileSize;
          filetransfer->setSize(vishnu::lexical_convertor<file_size_t>(*(++iter)));
          //convert the endDate into UTC date
          std::string tmpTime = *(++iter);
          startTime = convertLocaltimeINUTCtime(convertToTimeType(tmpTime)); 
          filetransfer->setStart_time(startTime);
          filetransfer->setErrorMsg(*(++iter));
          // Check the transfer Command enum value
          int trCommand=convertToInt(*(++iter));

          filetransfer->setTrCommand( (trCommand >=0&& trCommand<3 ? trCommand:2) );
          mlistObject->getFileTransfers().push_back(filetransfer);
        }
      }

      return mlistObject;
    }

  /**
   * \brief Function to get the name of the ListFileTransfers command line
   * \return The the name of the ListFileTransfers command line
   */
  std::string getCommandName()
  {
    return mcommandName;
  }

  /**
   * \brief Destructor, raises an exception on error
   */
  ~ListFileTransfers()
  {
  }

private:

  /**
   * \brief Function to check if a given transfer identifier exists
   * \param transferId the file transfer identifier
   */
  void checkTransferId(std::string transferId) {
    std::string sqlTransferRequest = "SELECT transferId from filetransfer where transferId='"+transferId+"'";
    boost::scoped_ptr<DatabaseResult> transfer(mdatabaseVishnu->getResult(sqlTransferRequest.c_str()));
    if(transfer->getNbTuples()==0) {
      throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer identifier");;
    }
  }

  /**
   * \brief Function to check the file transfer status
   * \param status the file transfer status
   */
  void checkStatus(int status) {
      
    if(status < 0 || status > 4) {
      throw UserException(ERRCODE_INVALID_PARAM, "The file transfer status option value is incorrect");
    }
  }

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListFileTransfers command line
  */
  std::string mcommandName;
};

#endif
