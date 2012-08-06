/*
 * tmsClientUtils.cpp
 *
 *  Created on: 23 juil. 2012
 *      Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 */


#include "api_ums.hpp"
#include "api_fms.hpp"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include "utilVishnu.hpp"
#include "tmsClientUtils.hpp"

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


