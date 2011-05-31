/**
 * \file ListFileTransfers.hpp
 * \brief This file contains the VISHNU ListFileTransfers class.
 * \author Daouda Traore (daouda.traore@sysfera.com) and
 *   Eugène Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * \date May 2011
 */
#ifndef _LIST_FILE_TRANSFERS_SERVER_
#define _LIST_FILE_TRANSFERS_SERVER_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "QueryServer.hpp"
#include "SessionServer.hpp"
#include "FMS_Data.hpp"

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
   * \brief Function to treat the listJobServer options
   * \param options the object which contains the ListFileTransfers options values
   * \param sqlRequest the sql data base request
   * \return raises an exception on error
   */
  void
  processOptions(const FMS_Data::LsTransferOptions_ptr& options, std::string& sqlRequest) {

    //To check if the transferId is defined
    if (options->getTransferId().size() != 0) {
      //To add the transferId on the request
      addOptionRequest("transferId", options->getTransferId(), sqlRequest);
    }

    //To check if the fromMachineId is defined
    if (options->getFromMachineId().size() != 0) {
      //To add the fromMachineId on the request
      addOptionRequest("fromMachineId", options->getFromMachineId(), sqlRequest);
    }

    //To check if the userId is defined
    if (options->getUserId().size() != 0) {
      //To add the userId on the request
      addOptionRequest("userId", options->getUserId(), sqlRequest);
    }

    //To check the file transfer status
    if (options->getStatus() != -1) {
      //To add the status on the request
      addOptionRequest("status", convertToString(options->getStatus()), sqlRequest);
    }

  }

  /**
   * \brief Function to list sessions information
   * \return The pointer to the UMS_Data::ListSessions containing sessions information
   * \return raises an exception on error
   */
  FMS_Data::FileTransferList*
    list() {
      std::string sqlListOfFiles = "SELECT vsessionid, transferId, status, userId, clientMachineId, sourceMachineId, "
        "destinationMachineId, sourceFilePath, destinationFilePath, fileSize, startTime, "
        " trCommand, processId from fileTransfer, vsession "
          "where vsession.numsessionid=fileTransfer.vsession_numsessionid and status > 0";

      std::vector<std::string>::iterator iter;
      std::vector<std::string> results;

      FMS_Data::FMS_DataFactory_ptr ecoreFactory = FMS_Data::FMS_DataFactory::_instance();
      mlistObject = ecoreFactory->createFileTransferList();

      msessionServer.check();

      processOptions(mparameters, sqlListOfFiles);
      sqlListOfFiles.append(" order by startTime");

      boost::scoped_ptr<DatabaseResult> ListOfFiles (mdatabaseVishnu->getResult(sqlListOfFiles.c_str()));
      int fileStatus ;
      time_t startTime;

      if (ListOfFiles->getNbTuples() != 0){
        for (size_t i = 0; i < ListOfFiles->getNbTuples(); ++i) {
          results.clear();
          results = ListOfFiles->get(i);
          iter = results.begin();

          FMS_Data::FileTransfer_ptr filetransfer = ecoreFactory->createFileTransfer();

          filetransfer->setTransferId(*iter);
          filetransfer->setStatus(convertToInt(*(++iter)));
          filetransfer->setUserId(*(++iter));
          filetransfer->setClientMachineId(*(++iter));
          filetransfer->setSourceMachineId(*(++iter));
          filetransfer->setDestinationMachineId(*(++iter));
          filetransfer->setSourceFilePath(*(++iter));
          filetransfer->setDestinationFilePath(*(++iter));
          filetransfer->setSize(convertToInt(*(++iter)));
          //convert the endDate into UTC date
          startTime = convertLocaltimeINUTCtime(convertToTimeType(*(++iter))); 
          filetransfer->setStart_time(startTime);
          filetransfer->setTrCommand(convertToInt(*(++iter)));

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

  /////////////////////////////////
  // Attributes
  /////////////////////////////////


  /**
  * \brief The name of the ListFileTransfers command line
  */
  std::string mcommandName;
};

#endif
