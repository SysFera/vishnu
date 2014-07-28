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

/*
  Internationalzation, multibyte string, UTF8 and other funny stuffs.
  If we have to use some emphasis for display, the right method is wide string.
  The most standard of character set is UTF-8.
  But, the integration of UTF8 in C++ begins in C++ 11 !
  C++ can use wide char for compute but for input/output there is no standard.
  So you can :
   - use an externel library like ICU
   - use a framework like QT
   - use a good operating system like windows
   - use a prototype library like boost (be carefull to the version and the evolution)

  or you can use C.
  The usage of multibyte and wide string is normalize since C99.

  The good scenario is :
   - initialize the locale : setlocale(LC_ALL, "")
   - convert the multibyte string to wide string with mbstowcs
   - compute in C with wchar or construct a wstring and compute in C++
   - convert the wide string to multibyte string with wcstombs
*/
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

/**
 * \brief Display a '-' caracter
 * \param size: The number of '-' to diplay
 * \The output stream in which the display will be done.
 */

void
setFill(int size, std::ostream& os) {

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
std::string ConvertModeToString(const mode_t& mode) {

  std::string modeStr;
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
std::string ConvertFileTransferStatusToString(const int& status){

  std::string result("");

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
std::string ConvertFileTypeToString(const int& type){

  std::string result("");
  
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
std::string ConvertFileTransferCommandToString(const int& trCmd){

  std::string result("");
  
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
std::string convertDiffTimeToString(const long& diff) {

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
operator<<(std::ostream& os, FMS_Data::FileStat& fileStat) {

  size_t maxSize = std::string("group").size()+2;
  os << std::setw(maxSize) << "------------------------------" << std::endl;
  os << std::setw(maxSize) << "the file properties: " << std::endl;
  os << std::setw(maxSize) << std::left  << "path: " << fileStat.getPath() << std::endl;
  os << std::setw(maxSize) << std::left  << "owner: " << fileStat.getOwner() << std::endl;
  os << std::setw(maxSize) << std::left  << "group: " <<  fileStat.getGroup() << std::endl;
  os << std::setw(maxSize) << std::left  << "uid: " <<  fileStat.getUid() << std::endl;
  os << std::setw(maxSize) << std::left  << "gid: " <<  fileStat.getGid() << std::endl;
  os << std::setw(maxSize) << std::left  << "size: " <<  fileStat.getSize() << std::endl;
  mode_t perms = fileStat.getPerms();
  os << std::setw(maxSize) << std::left  << "perms: " << std::oct << perms << std::dec ;
  os << " (" << ConvertModeToString(perms) << ")" << std::endl;
  os << std::setw(maxSize) << std::left  << "type: " << ConvertFileTypeToString(fileStat.getType()) << std::endl;
  boost::posix_time::ptime pt;
  pt =  boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(fileStat.getAtime()));
  os << std::setw(maxSize) << std::left  << "atime: " << boost::posix_time::to_simple_string(pt);
  os << " +"
     << boost::posix_time::to_simple_string(boost::posix_time::from_time_t(vishnu::diffLocaltimeUTCtime())) << std::endl;
  pt =  boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(fileStat.getMtime()));
  os << std::setw(maxSize) << std::left  << "mtime: " << boost::posix_time::to_simple_string(pt);
  os << " +"
     << boost::posix_time::to_simple_string(boost::posix_time::from_time_t(vishnu::diffLocaltimeUTCtime())) << std::endl;
  os << std::setw(maxSize)
     << std::left
     << "ctime: "
     << boost::posix_time::to_simple_string(boost::posix_time::from_time_t(fileStat.getCtime()));
  os << " +"
     << boost::posix_time::to_simple_string(boost::posix_time::from_time_t(vishnu::diffLocaltimeUTCtime()))
     << std::endl;

  return os;
}



std::ostream&
operator<<(std::ostream& os, FMS_Data::DirEntry& dirEntry) {

  size_t maxSize = std::string("group").size()+2;
  os << std::setw(maxSize) << "------------------------------" << std::endl;
  os << std::setw(maxSize) << "the file properties: " << std::endl;
  os << std::setw(maxSize) << std::left  << "path: " << dirEntry.getPath() << std::endl;
  os << std::setw(maxSize) << std::left  << "owner: " << dirEntry.getOwner() << std::endl;
  os << std::setw(maxSize) << std::left  << "group: " <<  dirEntry.getGroup() << std::endl;
  os << std::setw(maxSize) << std::left  << "size: " <<  dirEntry.getSize() << std::endl;
  mode_t perms = dirEntry.getPerms();
  os << std::setw(maxSize) << std::left  << "perms: " << std::oct << perms << std::dec ;
  os << " (" << ConvertModeToString(perms) << ")" << std::endl;
  os << std::setw(maxSize) << std::left  << "type: " << ConvertFileTypeToString(dirEntry.getType()) << std::endl;
  os << std::setw(maxSize) << std::left  << "creation time: " << dirEntry.getCtime() << std::endl;

  return os;
}



/**
 * \brief Helper function to display the information of a file
 *\param os: The output stream in which the list will be printed
 *\param dirEntryList: The file info list
 *\return The output stream in which the list of file information has
 *been printed
 */
std::ostream& operator<<(std::ostream& os,  FMS_Data::DirEntryList& dirEntryList) {

  size_t maxPermsSize = std::string("Permissions").size();
  size_t maxPathSize = std::string("Path").size();
  size_t maxOwnerSize = std::string("Owner").size();
  size_t maxGroupSize = std::string("Group").size();
  size_t maxFileSize = std::string("Size").size();
  size_t maxTypeSize = std::string("Type").size();
  size_t maxCreationTimeSize = std::string("CreationTime").size();

  // Buffers for C conversion
  // THis length is good for most filesystem
 #define  LG_MAX_FILENAME 255
  
  wchar_t buf[LG_MAX_FILENAME];                  // Filename in wide char format
  char output[LG_MAX_FILENAME*sizeof(wchar_t)];  // Filename un multibyte format
  size_t lg;                                     // Size of output for a given line
  
  mode_t perms;
  std::string owner;
  std::string group;
  std::size_t FileSize;
  std::string type;
  std::string creationTime;

  // For good multibyte conversion, you have to initialize the locale
  // This iniitialization use the correct define in environment
  setlocale(LC_ALL, "");

  for(unsigned int i = 0; i < dirEntryList.getDirEntries().size(); i++) {

    perms = (dirEntryList.getDirEntries().get(i))->getPerms();
    maxPermsSize = std::max(maxPermsSize, ConvertModeToString(perms).size());

    // we compute the size by a conversion in wide char (depens of locale)
    maxPathSize = std::max(maxPathSize, mbstowcs(buf,((dirEntryList.getDirEntries().get(i))->getPath()).c_str(),LG_MAX_FILENAME));

    owner = (dirEntryList.getDirEntries().get(i))->getOwner();
    maxOwnerSize = std::max(maxOwnerSize, owner.size());

    group = (dirEntryList.getDirEntries().get(i))->getGroup();
    maxGroupSize = std::max(maxGroupSize, group.size());

    FileSize = (dirEntryList.getDirEntries().get(i))->getSize();
    maxFileSize = std::max(maxFileSize, vishnu::convertToString(FileSize).size());

    type = ConvertFileTypeToString((dirEntryList.getDirEntries().get(i))->getType());
    maxTypeSize = std::max(maxTypeSize, type.size());

    creationTime = (dirEntryList.getDirEntries().get(i))->getCtime();
    maxCreationTimeSize = std::max(maxCreationTimeSize, creationTime.size());

  }

  os << std::setw(maxPermsSize+2)
     << std::left
     << "permissions"
     << std::setw(maxPathSize+2)
     << std::left << "path"
     << std::setw(maxOwnerSize+2)
     << std::left << "owner"
     << std::setw(maxGroupSize+2)
     << std::left << "group";
  os << std::setw(maxFileSize+2)
     << std::left
     << "size"
     << std::setw(maxTypeSize+2)
     << std::left
     << "type"
     << std::setw(maxCreationTimeSize+2)
     << std::left << "creation time";
  os << std::endl;

  setFill(maxPermsSize, os);
  setFill(maxPathSize, os);
  setFill(maxOwnerSize, os);
  setFill(maxGroupSize, os);
  setFill(maxFileSize, os);
  setFill(maxTypeSize, os);
  setFill(maxCreationTimeSize, os);
  os << std::endl;

  for(unsigned int i = 0; i < dirEntryList.getDirEntries().size(); i++) {
    
    perms = (dirEntryList.getDirEntries().get(i))->getPerms();
 
    // Conversion in wide char and compute of the real size
    lg = mbstowcs(buf,((dirEntryList.getDirEntries().get(i))->getPath()).c_str(),LG_MAX_FILENAME);
 
    // Completion with withespace to a fixed size
    for (unsigned int j = lg; j < maxPathSize + 2; j++)
      buf[j] = L' ';
    // and null terminated
    buf[maxPathSize+2] = 0;

    owner = (dirEntryList.getDirEntries().get(i))->getOwner();
    group = (dirEntryList.getDirEntries().get(i))->getGroup();
    FileSize = (dirEntryList.getDirEntries().get(i))->getSize();
    type = ConvertFileTypeToString((dirEntryList.getDirEntries().get(i))->getType());
    creationTime = (dirEntryList.getDirEntries().get(i))->getCtime();
    
    
    os << std::setw(maxPermsSize+2) << std::left << ConvertModeToString(perms);

    // we make the conversion to multibyte string (depends of locale in most case UT8)
    wcstombs(output,buf,sizeof(output));
 
    // insertion of the string output in the stream
    os << output;
 
    os << std::setw(maxOwnerSize+2) << std::left << owner;
    os << std::setw(maxOwnerSize+2) << std::left << group;
    os << std::setw(maxFileSize+2) << std::left << FileSize;
    os << std::setw(maxTypeSize+2) << std::left << type;
    os << std::setw(maxCreationTimeSize+2) << std::left << creationTime;
    os << std::endl;
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
operator<<(std::ostream& os, FMS_Data::FileTransfer& fileTransfer) {

  size_t maxSize = std::string("destinationMachineId").size()+2;

  os << std::setw(maxSize) << "------------ transfer information for file " << fileTransfer.getTransferId() << std::endl;
  os << std::setw(maxSize) << std::left << "transferId: " << fileTransfer.getTransferId()   << std::endl;
  os << std::setw(maxSize) << std::left << "status: " << ConvertFileTransferStatusToString(fileTransfer.getStatus())   << std::endl;
  os << std::setw(maxSize) << std::left << "errorMsg: " << fileTransfer.getErrorMsg()  << std::endl;
  os << std::setw(maxSize) << std::left << "userId: " << fileTransfer.getUserId()   << std::endl;
  os << std::setw(maxSize) << std::left << "clientMachineId: " << fileTransfer.getClientMachineId()   << std::endl;
  os << std::setw(maxSize) << std::left << "sourceMachineId: " << fileTransfer.getSourceMachineId()   << std::endl;
  os << std::setw(maxSize) << std::left << "destinationMachineId: " << fileTransfer.getDestinationMachineId()   << std::endl;
  os << std::setw(maxSize) << std::left << "sourceFilePath: " << fileTransfer.getSourceFilePath()   << std::endl;
  os << std::setw(maxSize) << std::left << "destinationFilePath: " << fileTransfer.getDestinationFilePath()   << std::endl;
  os << std::setw(maxSize) << std::left << "size: " << fileTransfer.getSize()   << std::endl;
  if(fileTransfer.getStartTime() > 0) {
    boost::posix_time::ptime pt =  boost::posix_time::from_time_t(fileTransfer.getStartTime());
    os << std::setw(maxSize) << std::left << "start_time: " <<  boost::posix_time::to_simple_string(pt)  << std::endl;
  } else {
    os << std::setw(maxSize) << std::left << "start_time: " << "-----" << std::endl;
  }
  os << std::setw(maxSize) << std::left << "trCommand: " << ConvertFileTransferCommandToString(fileTransfer.getTrCommand())   << std::endl;

  return os;
}

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed
 * \param FileTransferList: The file transfer list
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, FMS_Data::FileTransferList& fileTransferlist) {

  for (size_t i = 0 ; i < fileTransferlist.getFileTransfers().size() ; i++){
    os << *(fileTransferlist.getFileTransfers().get(i));
  }

  return os;
}

