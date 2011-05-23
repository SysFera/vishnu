
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_time.hpp"
#include "utilVishnu.hpp"
#include "displayer.hpp"

using namespace std;
using namespace vishnu;

/**
 * \file get_file_info.cpp
 * This file defines the VISHNU  FMS displayer functions 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */


/**
 * \brief function to convert differnce time (seconds) to simple string (hour)
 * \param diff The a given difference time 
 * \return The converted value in to string (hour) 
 */
string convertDiffTimeToString(const long& diff) {

  int h = diff/3600;
  int m = (diff-3600*h)/60;

  std::ostringstream diffStr;
  if(h < 10) {
    diffStr << "0" << h;
  } else {
    diffStr << h;
  }

  if(m < 10) {
    diffStr << "0" << m;
  } else {
    diffStr << m;
  }

 return diffStr.str();
}

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param fstat: The inode information
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileStat& fileStat) {
 
  size_t maxSize = std::string("group").size()+2; 
  os << setw(maxSize) << "------------------------------" << "\n";
  os << setw(maxSize) << "the file properties: \n";
  os << setw(maxSize) << right  << "path: " << fileStat.getPath() << "\n";
  os << setw(maxSize) << right  << "owner: " << fileStat.getOwner() << "\n";
  os << setw(maxSize) << right  << "group: " <<  fileStat.getGroup() << "\n";
  os << setw(maxSize) << right  << "uid: " <<  fileStat.getUid() << "\n";
  os << setw(maxSize) << right  << "gid: " <<  fileStat.getGid() << "\n";
  os << setw(maxSize) << right  << "size: " <<  fileStat.getSize() << "\n";
  os << setw(maxSize) << right  << "perms: " << oct << fileStat.getPerms() << "\n";
  boost::posix_time::ptime pt;
  pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getAtime()));
  //pt =  boost::posix_time::from_time_t(fileStat.getAtime());
  os << setw(maxSize) << right  << "atime: " << boost::posix_time::to_simple_string(pt);
  os << " +" << convertDiffTimeToString(diffLocaltimeUTCtime()) << "\n";
  pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getMtime()));
  os << setw(maxSize) << right  << "mtime: " << boost::posix_time::to_simple_string(pt);
  os << " +" << convertDiffTimeToString(diffLocaltimeUTCtime()) << "\n";
  time_t time = vishnu::convertUTCtimeINLocaltime(fileStat.getCtime());
  pt =  boost::posix_time::from_time_t(time);
  os << setw(maxSize) << right  << "ctime: " << boost::posix_time::to_simple_string(pt);
  os << " +" << convertDiffTimeToString(diffLocaltimeUTCtime()) << "\n";

return os;
}

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param fileTransfer: The content of file transfer
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileTransfer& fileTransfer) {

 size_t maxSize = std::string("destinationMachineId").size()+2;

 os << setw(maxSize) << "------------ transfer infomation for file " << fileTransfer.getTransferId() << "\n";
 os << setw(maxSize) << right << "transferId: " << fileTransfer.getTransferId()   << "\n";
 os << setw(maxSize) << right << "status: " << fileTransfer.getStatus()   << "\n";
 os << setw(maxSize) << right << "userId: " << fileTransfer.getUserId()   << "\n";
 os << setw(maxSize) << right << "clientMachineId: " << fileTransfer.getClientMachineId()   << "\n";
 os << setw(maxSize) << right << "sourceMachineId: " << fileTransfer.getSourceMachineId()   << "\n";
 os << setw(maxSize) << right << "destinationMachineId: " << fileTransfer.getDestinationMachineId()   << "\n";
 os << setw(maxSize) << right << "destinationFilePath: " << fileTransfer.getTransferId()   << "\n";
 os << setw(maxSize) << right << "size: " << fileTransfer.getSize()   << "\n";
 boost::posix_time::ptime pt;
 pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileTransfer.getStart_time()));
 os << setw(maxSize) << right << "start_time: " <<  boost::posix_time::to_simple_string(pt)  << "\n";
 os << setw(maxSize) << right << "trCommand: " << fileTransfer.getTrCommand()   << "\n";

return os;
}

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param FileTransferList: The file transfer list
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FileTransferList& fileTransferlist) {

 for (size_t i = 0 ; i < fileTransferlist().getFileTransfers().size() ; i++){
     os << (fileTransferlist().getFileTransfers().get(i));
  }

 return os;
}
