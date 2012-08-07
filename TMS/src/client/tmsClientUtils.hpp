/*
 * tmsClientUtils.hpp
 *
 *  Created on: 23 juil. 2012
 *      Author: Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 */

#ifndef TMSCLIENTUTILS_HPP_
#define TMSCLIENTUTILS_HPP_


#include <string>
#include "utilVishnu.hpp"

/**
 * \brief Function to get the hostname of a machine id
 *  \param Id of the machine
 */
inline std::string getMachineName(const std::string & sessionKey, const std::string & machineId);

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
		const int & startPos=0);

/**
 * \brief Function to copy a remote file to a local directory
 * \param srcMachineId : Id of the source machine
 * \param srcPath : Path of the source file
 * \param destMachineId : Id of the destination machine
 * \param destPath : The destination path (may be a directory)
 * \param copts : Copy option (false => non recursive, 0 => scp)
 * \return The copied file or throw exception on error
 */
std::string
genericFileCopier(const std::string & sessionKey,
                  const std::string & srcMachineId,
                  const std::string & srcPath,
                  const std::string & destMachineId,
                  const std::string & destPath,
                  const CpFileOptions& copts);


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
                  const CpFileOptions & copts);

#endif /* TMSCLIENTUTILS_HPP_ */
