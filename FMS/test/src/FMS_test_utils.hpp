
#ifndef FMS_TEST_UTILS_HPP
#define FMS_TEST_UTILS_HPP

#include "UMS_Data_forward.hpp"
// FMS forward Headers
#include "FMS_Data_forward.hpp"
#include "FMS_fixtures.hpp"
#define STATUS_INPROGRESS 0
#define STATUS_COMPLETED 1
#define STATUS_CANCELED 2
#define STATUS_FAILED 3

// C++ Headers
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

// Boost Headers
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign/list_of.hpp>

// Vishnu Headers

#include "utilVishnu.hpp"
#include "vishnuTestUtils.hpp"


namespace ba = boost::assign;
namespace bpt= boost::posix_time;
namespace bfs= boost::filesystem;


/**
 * \brief Check if given strings are found in directory
 * \param sessionKey  the session key
 * \param dirFullPath the directory complete path (host:path)
 * \param names       the strings to search for (vector)
 * \return true if all names are found within the directory long content
 */
bool areFoundInDir(const std::string& sessionKey,
                   const std::string& dirFullPath,
                   const std::vector<std::string>& names) {
  FMS_Data::DirEntryList dirContent;
  FMS_Data::LsDirOptions lsOptions;
  lsOptions.setLongFormat(true);
  BOOST_REQUIRE( vishnu::ls(sessionKey, dirFullPath, dirContent) ==0  );
  bool areFound = true;
  unsigned int i;
  for (std::vector<std::string>::const_iterator iterNames = names.begin();
    iterNames != names.end();
    ++iterNames) {
      bool isFound = false;
      i=0;
      while (!isFound &&  i < dirContent.getDirEntries().size()) {
        isFound = ((*iterNames) == (dirContent.getDirEntries().get(i))->getPath());
        ++i;
      }
      if (!isFound) {
        areFound = false;
      }
  }
  return areFound;
}
/**
 * \brief Check if a given string is found in directory
 * \param sessionKey  the session key
 * \param dirFullPath the directory complete path (host:path)
 * \param name        the string to search for
 * \return true name is found within the directory long content
 */
bool isFoundInDir(const std::string& sessionKey,
                  const std::string& dirFullPath,
                  const std::string& name) {
  return areFoundInDir(sessionKey, dirFullPath, ba::list_of(name));
}
/**
 * \brief Check if a given string is found in a local directory (must match a FILE name)
 * \param dirLocalPath the directory local path
 * \param name        the string to search for
 * \return true name is found within the directory long content
 */
bool isFoundInLocalDir(const std::string& dirLocalPath,
                       const std::string& name) {
  bfs::path dir(dirLocalPath);
  bfs::directory_iterator end_iter;
  if ( bfs::exists(dir) && bfs::is_directory(dir)) {
    for( bfs::directory_iterator dir_iter(dir) ; dir_iter != end_iter ; ++dir_iter) {
      if ( bfs::is_regular_file(dir_iter->status())
            && dir_iter->path().leaf() == name) {
        return true;
      }
    }
  }
  return false;
}

/**
 * \brief Add prefix to a vector of strings
 * \param prefix  the common prefix
 * \param suffixes  the vector of strings
 * \return the modified vector of strings (prefix + suffixes[i])
 */
std::vector<std::string>&
addPrefix(const std::string& prefix, std::vector<std::string>& suffixes) {
  for (std::vector<std::string>::iterator iter = suffixes.begin();
    iter != suffixes.end();
    ++iter  )
    {
      (*iter) = prefix + (*iter);
   }
  return suffixes;
}
/**
 * \brief Create several files on remote machine
 * \param sessionKey  the session key
 * \param localFilePaths  the local paths for the files
 * \param remoteFullPath  the destination full path (directory)
 */
void
createRemoteFiles(const std::string& sessionKey,
                  const std::vector<std::string>& localFilePaths,
                  const std::string& remoteFullPath) {
  for (std::vector<std::string>::const_iterator iter = localFilePaths.begin();
    iter != localFilePaths.end();
    ++iter) {
    const std::string& localFilePath = *iter;
    createFile<1>(localFilePath);
    BOOST_MESSAGE("CreateRemoteFile: " << localFilePath);
    std::string errorMsg =  "Could not copy file ";
    errorMsg.append(localFilePath);
    errorMsg.append(" on remote host");
    BOOST_REQUIRE_MESSAGE(vishnu::cp(sessionKey, localFilePath, remoteFullPath) == 0,
      errorMsg);
  }
}

/*****************************************************************************/
/************************** T R A N S F E R S ********************************/
/*****************************************************************************/
/**
 * \brief Wait until termination of an asynchronous file transfer
 * \param sessionKey the session Key
 * \param transferInfo  the file transfer information
 * \return the final status value of the file transfer (-1 if error)
 */

int
waitAsyncCopy(const std::string& sessionKey, const FMS_Data::FileTransfer& transferInfo) {
  unsigned int        pollCounter = 10;
  bool                terminated = false;
  FMS_Data::LsTransferOptions   options;
  FMS_Data::FileTransferList    fileTransferList;
  options.setTransferId(transferInfo.getTransferId());
  while (!terminated && pollCounter--) {
    if (vishnu::listFileTransfers(sessionKey, fileTransferList, options) != 0) {
      BOOST_MESSAGE("ERROR: Could not retrieve file transfer information");
      return -1;
    }
    if (fileTransferList.getFileTransfers().size() == 1) {
      FMS_Data::FileTransfer* ft = fileTransferList.getFileTransfers().get(0);
      if (ft->getStatus() != STATUS_INPROGRESS) {
        BOOST_MESSAGE("Async transfer is terminated! - status = " + vishnu::convertToString(ft->getStatus()));
        if (ft->getStatus() == STATUS_FAILED) {
          BOOST_MESSAGE("Transfer failed message: " + ft->getErrorMsg());
        }
        terminated = true;
      }
    } else if (fileTransferList.getFileTransfers().size() == 0) {
      BOOST_MESSAGE("ERROR: File transfer list is empty for a given transferId!");
      return -1;
    } else {
      BOOST_MESSAGE("ERROR: File transfer list contains more than 1 item for a given transferId!");
      return -1;
    }
    bpt::seconds sleepTime(5);
    boost::this_thread::sleep(sleepTime);
  }
  if (!terminated) {
    BOOST_MESSAGE("ERROR: End of polling for file transfer");
    return -1;
  }
  return fileTransferList.getFileTransfers().get(0)->getStatus();
}


#endif  // FMS_TEST_UTILS_HPP
