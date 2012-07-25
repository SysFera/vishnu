/*
 * tmsClientUtils.cpp
 *
 *  Created on: 23 juil. 2012
 *      Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 */


#include "api_ums.hpp"
#include "api_fms.hpp"
#include <boost/filesystem.hpp>
#include "utilVishnu.hpp"

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
	if(vishnu::listMachines(sessionKey, machines, mopts) || !machines.getMachines().size()) {
		throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "unable to get the information concerning the machine "+machineId);
	}
	return machines.getMachines().get(0)->getName();
}


/**
 * \brief Function to copy a remote file to a local directory
 * \param srcMid : Id of the remote machine
 * \param rfile : Path of the file to copy
 * \param ldestDir : Destination directory on the local machine
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \return Path of the copied file or Throw exception on error
 */
std::string copyFile(const std::string & sessionKey,
		const std::string & srcMid,
		const std::string & rfile,
		const std::string & ldestDir,
		const CpFileOptions& copts) {

	string src = srcMid + ":" + rfile;
	string dest = ldestDir + "/" + bfs::path(rfile).filename().string();

	if( bfs::exists(dest) ) {
		dest = bfs::unique_path(dest+".gen%%").string() ;
	}
	if( vishnu::copyFile(sessionKey, src, dest, copts)!=0 ) {
		throw FMSVishnuException(ERRCODE_RUNTIME_ERROR, "error while copying the file " + src
				+" to "+dest+" (machine name : "+ getMachineName(sessionKey, srcMid) + ")");
	}

	return dest ;
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
		copyFile(sessionKey, srcMid, rfiles[i], ldestDir, copts) ;
	}
}

