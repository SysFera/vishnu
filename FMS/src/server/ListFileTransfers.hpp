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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include "QueryServer.hpp"
#include "FMS_Data.hpp"
#include "FileTypes.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
/**
 * \class ListFileTransfers
 * \brief ListFileTransfers class implementation
 */
class ListFileTransfers: public QueryServer<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList> {

public:

  /**
   * \brief Constructor, raises an exception on error
   * \param authKey The session token
   */
  ListFileTransfers(const std::string& authKey)
    :  mauthKey(authKey), QueryServer<FMS_Data::LsTransferOptions, FMS_Data::FileTransferList>(),
      mcommandName("vishnu_list_file_transfers")
  {
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
    if (! options->getTransferId().empty()) {
      checkTransferId(options->getTransferId());
      addOptionRequest("numfiletransferid", options->getTransferId(), sqlRequest);
      onlyProgressFile = false;
    }

    //To check if the fromMachineId is defined
    if (! options->getFromMachineId().empty()) {
      sqlRequest.append(" AND (sourceMachineId='"+mdatabase->escapeData(options->getFromMachineId())+"'"
                        + " OR destinationMachineId='"+mdatabase->escapeData(options->getFromMachineId())+"')");
      onlyProgressFile = false;
    }

    // check if the userId filter is defined
    if (! options->getUserId().empty()) {
      if (muserSessionInfo.user_privilege == vishnu::PRIVILEGE_ADMIN) {
        throw UMSVishnuException (ERRCODE_NO_ADMIN);
      }

      getNumUser(options->getUserId());
      addOptionRequest("users.userId", options->getUserId(), sqlRequest);
      onlyProgressFile = false;
    }

    //To check the file transfer status
    if (options->getStatus() != 4) { // UNDEFINED FILE TRANSFER STATUS
      //To check the file status
      checkStatus(options->getStatus());
      addOptionRequest("filetransfer.status", vishnu::convertToString(options->getStatus()), sqlRequest);
    } else {
      if(onlyProgressFile) {
        addOptionRequest("filetransfer.status", "0", sqlRequest);
      }
    }

  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  FMS_Data::FileTransferList*
  list(FMS_Data::LsTransferOptions_ptr options) {

    std::string sqlListOfFiles = "SELECT DISTINCT numfiletransferid, filetransfer.status, users.userid, clientMachineId, "
                                 "   sourceMachineId, destinationMachineId, sourceFilePath,"
                                 "   destinationFilePath, fileSize, startTime, errorMsg, trCommand "
                                 " FROM filetransfer, vsession, users "
                                 " WHERE vsession.numsessionid  = filetransfer.vsession_numsessionid"
                                 "   AND vsession.users_numuserid=users.numuserid";

    std::vector<std::string>::iterator iter;
    std::vector<std::string> results;
    vishnu::validateAuthKey(mauthKey, mdatabase, muserSessionInfo);

    FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();
    mlistObject = ecoreFactory->createFileTransferList();

    processOptions(options, sqlListOfFiles);
    sqlListOfFiles.append(" ORDER BY startTime");


    boost::scoped_ptr<DatabaseResult> ListOfFiles (mdatabase->getResult(sqlListOfFiles));

    time_t startTime;


    if (ListOfFiles->getNbTuples() != 0){


      for (size_t i = 0; i < ListOfFiles->getNbTuples(); ++i) {
        results.clear();
        results = ListOfFiles->get(i);
        iter = results.begin();

        FMS_Data::FileTransfer_ptr filetransfer = ecoreFactory->createFileTransfer();


        filetransfer->setTransferId(*iter);
        int trStatus=vishnu::convertToInt(*(++iter));
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
        filetransfer->setSize(boost::lexical_cast<file_size_t>(*(++iter)));
        //convert the endDate into UTC date
        std::string tmpTime = *(++iter);
        startTime = vishnu::string_to_time_t(tmpTime);
        filetransfer->setStartTime(startTime);
        filetransfer->setErrorMsg(*(++iter));
        // Check the transfer Command enum value
        int trCommand=vishnu::convertToInt(*(++iter));

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
    std::string sqlTransferRequest = "SELECT numfiletransferid"
                                     " FROM filetransfer"
                                     " WHERE numfiletransferid='"+mdatabase->escapeData(transferId)+"'";
    boost::scoped_ptr<DatabaseResult> transfer(mdatabase->getResult(sqlTransferRequest));
    if (transfer->getNbTuples()==0) {
      throw UserException(ERRCODE_INVALID_PARAM, "Invalid transfer identifier");;
    }
  }

  /**
   * \brief Function to check the file transfer status
   * \param status the file transfer status
   */
  void checkStatus(int status) {

    if (status < 0 || status > 4) {
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

  /**
   * @brief Session info
   */
  std::string mauthKey;

  /**
   * @brief Information about the user and the session
   */
  UserSessionInfo muserSessionInfo;
};

#endif
