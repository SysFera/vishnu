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

using namespace std;
using namespace vishnu;
using namespace FMS_Data;

/**
 * \brief function to convert string value to mode_t
 * \param modeStr The mode value to convert
 * \return converted mode value
 */
mode_t stringToModType(const string& modeStr) {

  std::string modeToCvt(9,'-');
  size_t posComma=0;

  size_t pos = modeStr.find_first_not_of("ugo=xwr,");
  if(pos!=std::string::npos) {
    throw UserException(ERRCODE_INVALID_PARAM, ("Invalid mode: "+modeStr));
  }

  do {
    pos = modeStr.find("=",posComma);
    string ugo = modeStr.substr(posComma, pos-posComma+1);
    size_t nextComma = modeStr.find(",",pos+1);
    string perms;
    if(nextComma!=std::string::npos) {
      perms = modeStr.substr(pos+1, nextComma-pos-1);
    }
    else {
      perms = modeStr.substr(pos+1);
    }

    if((ugo.find_first_not_of("ugo=")!=std::string::npos && ugo.compare("=")!=0) ||
       perms.find_first_not_of("xwr")!=std::string::npos || perms.size()==0) {
      throw UserException(ERRCODE_INVALID_PARAM, ("Invalid mode: "+modeStr));
    } else {
      //For user permission mode
      if(ugo.find("u")!=std::string::npos || ugo.compare("=")==0) {
        modeToCvt.replace(0,3,"---");//re-initializes the user permission
        if(perms.find("r")!=std::string::npos) {
          modeToCvt[0]='r';
        }
        if(perms.find("w")!=std::string::npos) {
          modeToCvt[1]='w';
        } 
        if(perms.find("x")!=std::string::npos) {
          modeToCvt[2]='x';
        }
      }
      //For group permission mode
      if(ugo.find("g")!=std::string::npos) {
        modeToCvt.replace(3,3,"---");//re-initializes the user permission
        if(perms.find("r")!=std::string::npos) {
          modeToCvt[3]='r';
        } 
        if(perms.find("w")!=std::string::npos) {
          modeToCvt[4]='w';
        } 
        if(perms.find("x")!=std::string::npos) {
          modeToCvt[5]='x';
        }
      }
      //For other permission mode
      if(ugo.find("o")!=std::string::npos) {
        modeToCvt.replace(6,3,"---");//re-initializes the user permission
        if(perms.find("r")!=std::string::npos) {
          modeToCvt[6]='r';
        }
        if(perms.find("w")!=std::string::npos) {
          modeToCvt[7]='w';
        }
        if(perms.find("x")!=std::string::npos) {
          modeToCvt[8]='x';
        }
      }
    }

    posComma = modeStr.find(",", posComma);
    if(posComma!=std::string::npos) {
      posComma = posComma +1;
    }
  } while(posComma!=std::string::npos);

  mode_t mode = 0;
  const mode_t usrR = 1 << 8;
  const mode_t usrW = 1 << 7;
  const mode_t usrX = 1 << 6;
  const mode_t grpR = 1 << 5;
  const mode_t grpW = 1 << 4;
  const mode_t grpX = 1 << 3;
  const mode_t othR = 1 << 2;
  const mode_t othW = 1 << 1;
  const mode_t othX = 1;

  string::const_iterator iter=modeToCvt.begin();
  string::const_iterator end=modeToCvt.end();  

  //For user permission mode
  if(iter!=end){
    if(*iter=='r') {
      mode |=usrR; 
    }
    ++iter;
  }
  if(iter!=end){
    if(*iter=='w') {
      mode |=usrW;
    }
    ++iter;
  }
  if(iter!=end){
    if(*iter=='x') {
      mode |=usrX;
    }
    ++iter;
  }

  //For group permission mode
  if(iter!=end){
    if(*iter=='r') {
      mode |=grpR;
    }
    ++iter;
  }
  if(iter!=end){
    if(*iter=='w') {
      mode |=grpW;
    }
    ++iter;
  }
  if(iter!=end){
    if(*iter=='x') {
      mode |=grpX;
    }
    ++iter;
  }

  //For other permission mode
  if(iter!=end){
    if(*iter=='r') {
      mode |=othR;
    }
    ++iter;
  }
  if(iter!=end){
    if(*iter=='w') {
      mode |=othW;
    }
    ++iter;
  }
  if(iter!=end){
    if(*iter=='x') {
      mode |=othX;
    }
    ++iter;
  }

  std::ostringstream os;
  os << oct << mode;

  std::istringstream is(os.str());
  is >> mode;

  return mode; 
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

  //For user permission mode
  if (mode & usrR){
    modeStr+="r";
  } else {
    modeStr+="-";
  }
  if (mode & usrW) {
    modeStr+="w";
  } else {
    modeStr+="-";
  } if (mode & usrX) {
    modeStr+="x";
  } else { 
    modeStr+="-";
  }

 //For other groupe mode
  if (mode & grpR) { 
    modeStr+="r";
  } else {
    modeStr+="-";
  } if (mode & grpW) {
    modeStr+="w";
  } else {
    modeStr+="-";
  } 
  if (mode & grpX) { 
    modeStr+="x";
  } else { 
    modeStr+="-";
  }

  //For other permission mode
  if (mode & othR) {
    modeStr+="r";
  } else { 
    modeStr+="-";
  } if (mode & othW) { 
    modeStr+="w";
  } else {
    modeStr+="-";
  }
  if (mode & othX){ 
    modeStr+="x";
  } else {
    modeStr+="-";
  }

  return modeStr;
}

/**
 * \brief function to convert file transfer status to string
 * \param mode The status value to convert
 * \return converted status value
 */
string ConvertFileTransferStatusToString(const int& status){

  switch(status) {
    case 0:
      return "INPROGRESS";
    case 1:
      return "COMPLETED";
    case 2:
      return "CANCELED";
    case 3:
      return "FAILED";
    default:
      return "UNDEFINED";
  }

}


/**
 * \brief function to convert file type to string
 * \param mode The type value to convert
 * \return converted type value
 */
string ConvertFileTypeToString(const int& type){

   switch(type) {
    case 0:
      return "BLOCK";
    case 1:
      return "CHARACTER";
    case 2:
      return "DIRECTORY";
    case 3:
      return "SYMBOLICKINK";
    case 4:
      return "SCKT";
    case 5:
      return "FIFO";
    case 6:
      return "REGULAR";
    default:
      return "UNDEFINED";
  }
 
}
/**
 * \brief function to convert file transfer command to string
 * \param trCmd The status value to convert
 * \return converted command value
 */
string ConvertFileTransferCommandToString(const int& trCmd){

  switch(trCmd) {
    case 0:
      return "SCP";
    case 1:
      return "RSYNC";
    default:
      return "UNDEFINED";
  }

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
  os << setw(maxSize) << right  << "perms: " << oct << perms;
  os << " (" << ConvertModeToString(perms) << ")" << std::endl;
  os << setw(maxSize) << right  << "type: " << ConvertFileTypeToString(fileStat.getType()) << std::endl;
  boost::posix_time::ptime pt;
  pt =  boost::posix_time::from_time_t(convertUTCtimeINLocaltime(fileStat.getAtime()));
  //pt =  boost::posix_time::from_time_t(fileStat.getAtime());
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

/**
 * \brief Helper function to display the information of a file
 * \param os: The output stream in which the list will be printed 
 * \param dirContent: The content of the directory 
 * \return The output stream in which the list of users has been printed
 */
std::ostream&
operator<<(std::ostream& os, StringList& dirContent) {

  std::vector<std::string> dirContentvec=dirContent.getStrings();
  std::vector<string>::const_iterator iter;
  for (iter=dirContentvec.begin(); iter!=dirContentvec.end(); ++iter){
    os << *iter << std::endl;
  }

  return os;
}
