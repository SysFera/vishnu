/*
 * tmsClientUtils.cpp
 *
 *  Created on: 23 juil. 2012
 *      Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 */


#include "api_ums.hpp"
#include "api_fms.hpp"
#include "api_tms.hpp"
#include "TMS_Data.hpp"
#include "SessionServer.hpp"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "utilVishnu.hpp"
#include "tmsClientUtils.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>

using namespace std;
namespace bfs = boost::filesystem;
/**
 * \brief Function to get the hostname of a machine id
 *  \param Id of the machine
 */
std::string getMachineName(const std::string & sessionKey, const std::string & machineId) {

  UMS_Data::ListMachines machines;
  UMS_Data::ListMachineOptions mopts;
  mopts.setListAllMachine(false);
  mopts.setMachineId(machineId);
  vishnu::listMachines(sessionKey, machines, mopts);
  if (machines.getMachines().size() == 0) {
    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "unable to get the information concerning the machine "+machineId);
  }
  return machines.getMachines().get(0)->getName();
}

/**
 * \brief Function to copy a list of remote files to a local directory
 * \param srcMid : Id of the remote machine
 * \param rfiles : List of the files to copy
 * \param ldestDir : Destination directory on the local machine
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \param startPos : Position of the file
 * \return Throw exception on error
 */
void copyFiles(const std::string & sessionKey,
               const std::string & srcMid,
               const ListStrings & rfiles,
               const std::string & ldestDir,
               const CpFileOptions& copts,
               const int & startPos) {

  int nbFiles = rfiles.size() ;
  for(int i=startPos; i<nbFiles; i++) {
    genericFileCopier(sessionKey, srcMid, rfiles[i], "", ldestDir, copts);
  }
}


/**
 * \brief Function to copy a remote file to a local directory
 * \param srcMachineId : Id of the source machine
 * \param srcPath : Path of the source file
 * \param destMachineId : Id of the destination machine
 * \param destPath : The destination path (may be a directory)
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \return The copied file or throw exception on error
 */
std::string genericFileCopier(const std::string & sessionKey,
                              const std::string & srcMachineId,
                              const std::string & srcPath,
                              const std::string & destMachineId,
                              const std::string & destPath,
                              const CpFileOptions& copts){

  string src = srcPath;
  if(srcMachineId.size() != 0) {
    src = srcMachineId + ":" + src;
  }

  string dest = destPath;
  if(destMachineId.size() != 0) {
    dest = destMachineId + ":" + dest;
  }

  if( vishnu::copyFile(sessionKey, src, dest, copts)!=0 ) {
    string srcMachine = (srcMachineId.size() != 0)? getMachineName(sessionKey, srcMachineId) : "localhost" ;
    string destMachine = (destMachineId.size() != 0)? getMachineName(sessionKey, destMachineId) : "localhost" ;
    string msg = boost::str(boost::format("error while copying the file %1 (machine: %2) to %3 (machine: %4)") % src % dest % srcMachine % destMachine);

    throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, msg);
  }

  return dest ;
}

/**
 * \brief Function to copy a remote file to a local directory
 * \param srcFiles : String describing the source files
 * \param destMachineId : Id of the destination machine
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \return A string describing the destination file. The function throw exception on error
 */
std::string
sendInputFiles(const std::string & sessionKey,
               const std::string & srcFiles,
               const std::string & destMachineId,
               const CpFileOptions& copts) {

  ListStrings listFiles ;
  boost::split(listFiles, srcFiles, boost::is_space()) ;
  string rdestDir = bfs::unique_path("/tmp/fms%%%%%%").string();

  if (listFiles.size() > 0 && srcFiles.size() != 0) {
    string fqdnDestDir = (boost::format("%1%:%2%")%destMachineId%rdestDir).str();
    if(vishnu::createDir(sessionKey, fqdnDestDir)){
      throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "unable to create the remote directory for input files: "+fqdnDestDir);
    }
  }

  std::ostringstream paramsBuf ;
  for(ListStrings::const_iterator it = listFiles.begin(); it != listFiles.end(); ++it){

    size_t pos = (*it).find("=") ; if(pos == std::string::npos) continue ; //*it would be in the form of param=path
    string param = (*it).substr(0, pos) ;
    string path = (*it).substr(pos+1, std::string::npos);

    size_t colonPos = path.find(":");
    std::string filerMachineId;
    if ((colonPos == string::npos) && !bfs::exists(path)) {
      throw FMSVishnuException(ERRCODE_FILENOTFOUND, path);
      /* TODO : the following two lines will never be executed, and
       * filerMachineId is used while not initialized in genericFileCopier
       */
      filerMachineId = path.substr(0, colonPos);
      path = path.substr(++colonPos, string::npos);
    }

    string rpath = rdestDir + "/" + bfs::path(path).filename().string();

    CpFileOptions copts;
    copts.setIsRecursive(true) ;
    copts.setTrCommand(0); // for using scp

    genericFileCopier(sessionKey, filerMachineId, path, destMachineId, rpath, copts);
    paramsBuf << ((paramsBuf.str().size() != 0)? " " : "") + param << "=" << rpath ;
  }

  return paramsBuf.str() ;
}

/**
 * \brief Function to select a machine for automatic submission
 * \param pb is a structure which corresponds to the descriptor of a profile
 * \param The selection criterion
 * \return the selected machine or raises an exception on error
 */
std::string
findMachine(const std::string& sessionKey,
            const TMS_Data::LoadCriterion_ptr & criterion) {

  // First list all active machines where we have a local account
  // FIXME: Update the selection option so to ensure that we do not select an inactive machine
  // or a machine where we don't have local account
  UMS_Data::ListMachineOptions mopts;
  mopts.setListAllMachine(false);
  mopts.setMachineId("");
  UMS_Data::ListMachines machines;
  vishnu::listMachines(sessionKey, machines, mopts) ;

  int machineCount = machines.getMachines().size() ;
  if( machineCount == 0) {
    throw UMSVishnuException(ERRCODE_UNKNOWN_MACHINE, "You have no local account on available machines");
  }

  std::string selectedMachine = "" ;
  long load = std::numeric_limits<long>::max();
  for(int i=0; i< machineCount; i++) {
    UMS_Data::Machine_ptr machine = machines.getMachines().get(i) ;
    try {
      if(getMachineLoadPerformance(sessionKey, machine, criterion) < load) {
        selectedMachine = machine->getMachineId();
      }
    } catch(...) {
      continue ;
    }
  }
  return selectedMachine;
}


/**
 * \brief Function to compute the load performance of a given machine
 * \param sessionKey The session key
 * \param pb the request profile
 * \param the criteria of (number of waiting jobs, running jobs and total jobs)
 */
long
getMachineLoadPerformance(const string& sessionKey,
                          const UMS_Data::Machine_ptr& machine,
                          const TMS_Data::LoadCriterion_ptr& criterion) {

  TMS_Data::ListJobs jobs ;
  TMS_Data::ListJobsOptions jobOtions ;

  try {
    vishnu::listJobs(sessionKey, machine->getMachineId(), jobs, jobOtions) ;
  } catch (SystemException& ex){
    throw SystemException (ERRCODE_RUNTIME_ERROR, ex.what());
  }

  long load = std::numeric_limits<long>::max();
  int criterionType = (criterion)? criterion->getLoadType(): jobs.getNbWaitingJobs() ;
  try {
    switch(criterionType) {
    case NBRUNNINGJOBS :
      load = jobs.getNbRunningJobs();
      break;
    case NBJOBS :
      load = jobs.getNbJobs();
      break;
    case NBWAITINGJOBS :
    default :
      load =jobs.getNbWaitingJobs();
      break;
    }
  } catch (VishnuException& ex) {
    std::cerr << ex.what() << std::endl;
  } catch(...) {
    std::cerr << "E: error while calculating the load performance of the machine "
              << machine->getMachineId() << " (" << machine->getName() <<")"<< std::endl;
  }
  return load ;
}
