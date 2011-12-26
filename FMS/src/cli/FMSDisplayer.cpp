/**
 * \file FMSDisplayer.cpp
 * This file defines the VISHNU  FMS FMSDisplayer functions 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/c_time.hpp"
#include "utilVishnu.hpp"
#include "FMS_Data.hpp"
#include "FMSDisplayer.hpp"
#include <iomanip>
#include <iostream>

using namespace std;
using namespace vishnu;
using namespace FMS_Data;

/**
 * \brief Display a '-' caracter
 * \param size: The number of '-' to diplay
 * \The output stream in which the display will be done.
 */

void
setFill(int size, ostream& os) {

  for(int i=0; i < size; i++) {
    os << "-";
  }
  os << "  ";

}




/**
 * \brief function to convert mode value to string
 * \param mode The mode value to convert
 * \return converted mode value
 */
string ConvertModeToString(const mode_t& mode) {

  string modeStr;
  const mode_t usrR = 1 << 8;
  const mode_t usrW = 1 << 7;
  const mode_t usrX = 1 << 6;
  const mode_t grpR = 1 << 5;
  const mode_t grpW = 1 << 4;
  const mode_t grpX = 1 << 3;
  const mode_t othR = 1 << 2;
  const mode_t othW = 1 << 1;
  const mode_t othX = 1;

  //For owner permission 
  modeStr+=(mode & usrR ? "r":"-");
  modeStr+=(mode & usrW ? "w":"-");
  modeStr+=(mode & usrX ? "x":"-");

  //For group owner permissions
  modeStr+=(mode & grpR ? "r":"-");
  modeStr+=(mode & grpW ? "w":"-");
  modeStr+=(mode & grpX ? "x":"-");

  //For other users permissions
  modeStr+=(mode & othR ? "r":"-");
  modeStr+=(mode & othW ? "w":"-");
  modeStr+=(mode & othX ? "x":"-");

  return modeStr;
}

/**
 * \brief function to convert file transfer status to string
 * \param mode The status value to convert
 * \return converted status value
 */
string ConvertFileTransferStatusToString(const int& status){

  string result("");

  switch(status) {
    case 0:
      result="INPROGRESS";
      break;
    case 1:
      result="COMPLETED";
      break;
    case 2:
      result="CANCELED";
      break;
    case 3:
      result="FAILED";
      break;
    default:
      result= "UNDEFINED";
      break;
  }
return result;
}


/**
 * \brief function to convert file type to string
 * \param mode The type value to convert
 * \return converted type value
 */
string ConvertFileTypeToString(const int& type){

  string result("");
  
  switch(type) {
    case 0:
      result= "BLOCK";
      break;
    case 1:
      result="CHARACTER";
      break;
    case 2:
      result="DIRECTORY";
      break;
    case 3:
      result="SYMBOLICKINK";
      break;
    case 4:
      result="SCKT";
      break;
    case 5:
      result="FIFO";
      break;
    case 6:
      result="REGULAR";
      break;
    default:
      result= "UNDEFINED";
      break;
  }
 
return result;
}
/**
 * \brief function to convert file transfer command to string
 * \param trCmd The status value to convert
 * \return converted command value
 */
string ConvertFileTransferCommandToString(const int& trCmd){

  string result("");
  
  switch(trCmd) {
    case 0:
      result= "SCP";
      break;
    case 1:
      result="RSYNC";
      break;
    default:
      result= "UNDEFINED";
      break;
  }

return result;
}
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
  os << setw(maxSize) << "------------------------------" << std::endl;
  os << setw(maxSize) << "the file properties: " << std::endl;
  os << setw(maxSize) << right  << "path: " << fileStat.getPath() << std::endl;
  os << setw(maxSize) << right  << "owner: " << fileStat.getOwner() << std::endl;
  os << setw(maxSize) << right  << "group: " <<  fileStat.getGroup() << std::endl;
  os << setw(maxSize) << right  << "uid: " <<  fileStat.getUid() << std::endl;
  os << setw(maxSize) << right  << "gid: " <<  fileStat.getGid() << std::endl;
  os << setw(maxSize) << right  << "size: " <<  fileStat.getSize() << std::endl;
  mode_t perms = fileStat.getPerms();
  os << setw(maxSize) << right  << "perms: " << oct << perms << dec ;
  os << " (" << ConvertModeToString(perms) << ")" << std::endl;
  os << setw(maxSize) << right  << "type: " << ConvertFileTypeToString(fileStat.getType()) << std::endl;
  boost::posix_time::ptime pt;
  pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getAtime()));
  os << setw(maxSize) << right  << "atime: " << boost::posix_time::to_simple_string(pt);
  os << " +" << convertDiffTimeToString(diffLocaltimeUTCtime()) << std::endl;
  pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getMtime()));
  os << setw(maxSize) << right  << "mtime: " << boost::posix_time::to_simple_string(pt);
  os << " +" << convertDiffTimeToString(diffLocaltimeUTCtime()) << std::endl;
  time_t time = vishnu::convertUTCtimeINLocaltime(fileStat.getCtime());
  pt =  boost::posix_time::from_time_t(time);
  os << setw(maxSize) << right  << "ctime: " << boost::posix_time::to_simple_string(pt);
  os << " +" << convertDiffTimeToString(diffLocaltimeUTCtime()) << std::endl;

return os;
}



std::ostream&
operator<<(std::ostream& os, DirEntry& dirEntry) {
 
  size_t maxSize = std::string("group").size()+2; 
  os << setw(maxSize) << "------------------------------" << std::endl;
  os << setw(maxSize) << "the file properties: " << std::endl;
  os << setw(maxSize) << right  << "path: " << dirEntry.getPath() << std::endl;
  os << setw(maxSize) << right  << "owner: " << dirEntry.getOwner() << std::endl;
  os << setw(maxSize) << right  << "group: " <<  dirEntry.getGroup() << std::endl;
  os << setw(maxSize) << right  << "size: " <<  dirEntry.getSize() << std::endl;
  mode_t perms = dirEntry.getPerms();
  os << setw(maxSize) << right  << "perms: " << oct << perms << dec ;
  os << " (" << ConvertModeToString(perms) << ")" << std::endl;
  os << setw(maxSize) << right  << "type: " << ConvertFileTypeToString(dirEntry.getType()) << std::endl;
  os << setw(maxSize) << right  << "creation time: " << dirEntry.getCreationTime() << std::endl;

return os;
}



/**
 * \brief Helper function to display the information of a file
 *\param os: The output stream in which the list will be printed 
 *\param dirEntryList: The file info list
 *\return The output stream in which the list of file information has
 *been printed
 */
 ostream& operator<<(ostream& os,  DirEntryList& dirEntryList) {

  size_t maxPermsSize = std::string("Permissions").size();
  size_t maxPathSize = std::string("Path").size();
  size_t maxOwnerSize = std::string("Owner").size();
  size_t maxGroupSize = std::string("Group").size();
  size_t maxFileSize = std::string("Size").size();
  size_t maxTypeSize = std::string("Type").size();
  size_t maxCreationTimeSize = std::string("CreationTime").size();
  
  mode_t perms;
  std::string path;
  std::string owner;
  std::string group;
  std::size_t FileSize;
  std::string type;
  std::string creationTime;

  for(unsigned int i = 0; i < dirEntryList.getDirEntries().size(); i++) {

     perms = (dirEntryList.getDirEntries().get(i))->getPerms();
     maxPermsSize = max(maxPermsSize, ConvertModeToString(perms).size());
     
     path = (dirEntryList.getDirEntries().get(i))->getPath();
     maxPathSize = max(maxPathSize, path.size());

     owner = (dirEntryList.getDirEntries().get(i))->getOwner();
     maxOwnerSize = max(maxOwnerSize, owner.size());

     group = (dirEntryList.getDirEntries().get(i))->getGroup();
     maxGroupSize = max(maxGroupSize, group.size());

     FileSize = (dirEntryList.getDirEntries().get(i))->getSize();
     maxFileSize = max(maxFileSize, vishnu::convertToString(FileSize).size());
     
     type = ConvertFileTypeToString((dirEntryList.getDirEntries().get(i))->getType());
     maxTypeSize = max(maxTypeSize, type.size());
     
     creationTime = (dirEntryList.getDirEntries().get(i))->getCreationTime();
     maxCreationTimeSize = max(maxCreationTimeSize, creationTime.size());
     
  }

  os << setw(maxPermsSize+2) << left << "permissions"<< setw(maxPathSize+2) << left << "path" << setw(maxOwnerSize+2) << left << "owner" << setw(maxGroupSize+2) << left << "group";
  os <<setw(maxFileSize+2) << left << "size"<<setw(maxTypeSize+2) << left << "type"<<setw(maxCreationTimeSize+2) << left << "creation time";    
  os << endl;

  setFill(maxPermsSize, os);
  setFill(maxPathSize, os);
  setFill(maxOwnerSize, os);
  setFill(maxGroupSize, os);
  setFill(maxFileSize, os);
  setFill(maxTypeSize, os);
  setFill(maxCreationTimeSize, os);
  os << endl;


  for(unsigned int i = 0; i < dirEntryList.getDirEntries().size(); i++) {
    
     perms = (dirEntryList.getDirEntries().get(i))->getPerms();
     path = (dirEntryList.getDirEntries().get(i))->getPath();
     owner = (dirEntryList.getDirEntries().get(i))->getOwner();
     group = (dirEntryList.getDirEntries().get(i))->getGroup();
     FileSize = (dirEntryList.getDirEntries().get(i))->getSize();
     type = ConvertFileTypeToString((dirEntryList.getDirEntries().get(i))->getType());
     creationTime = (dirEntryList.getDirEntries().get(i))->getCreationTime();
    
    
    os << setw(maxPermsSize+2) << left << ConvertModeToString(perms);
    os << setw(maxPathSize+2) << left <<  path;
    os << setw(maxOwnerSize+2) << left << owner;
    os << setw(maxOwnerSize+2) << left << group;
    os << setw(maxFileSize+2) << left << FileSize;
    os << setw(maxTypeSize+2) << left << type;
    os << setw(maxCreationTimeSize+2) << left << creationTime;
    os << endl;
  }

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

  os << setw(maxSize) << "------------ transfer infomation for file " << fileTransfer.getTransferId() << std::endl;
  os << setw(maxSize) << right << "transferId: " << fileTransfer.getTransferId()   << std::endl;
  os << setw(maxSize) << right << "status: " << ConvertFileTransferStatusToString(fileTransfer.getStatus())   << std::endl;
  os << setw(maxSize) << right << "errorMsg: " << fileTransfer.getErrorMsg()  << std::endl;
  os << setw(maxSize) << right << "userId: " << fileTransfer.getUserId()   << std::endl;
  os << setw(maxSize) << right << "clientMachineId: " << fileTransfer.getClientMachineId()   << std::endl;
  os << setw(maxSize) << right << "sourceMachineId: " << fileTransfer.getSourceMachineId()   << std::endl;
  os << setw(maxSize) << right << "destinationMachineId: " << fileTransfer.getDestinationMachineId()   << std::endl;
  os << setw(maxSize) << right << "sourceFilePath: " << fileTransfer.getSourceFilePath()   << std::endl;
  os << setw(maxSize) << right << "destinationFilePath: " << fileTransfer.getDestinationFilePath()   << std::endl;
  os << setw(maxSize) << right << "size: " << fileTransfer.getSize()   << std::endl;
  long start_time = fileTransfer.getStart_time();
  if(start_time > 0) {
    boost::posix_time::ptime pt;
    pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(start_time));
    os << setw(maxSize) << right << "start_time: " <<  boost::posix_time::to_simple_string(pt)  << std::endl;
  } else {
    os << setw(maxSize) << right << "start_time: " << "-----" << std::endl;
  }
  os << setw(maxSize) << right << "trCommand: " << ConvertFileTransferCommandToString(fileTransfer.getTrCommand())   << std::endl;

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

 for (size_t i = 0 ; i < fileTransferlist.getFileTransfers().size() ; i++){
     os << *(fileTransferlist.getFileTransfers().get(i));
  }

 return os;
}

