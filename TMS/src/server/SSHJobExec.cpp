/**
 * \file SSHJobExec.cpp
 * \brief This file presents the implementation of the SSHJobExec.
 * \author Daouda Traore (daouda.traore@sysfera.com)
 * \date April
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <stdlib.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "TMS_Data.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "SystemException.hpp"
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "SSHJobExec.hpp"

const std::string TMS_SERVER_FILES_DIR="/tmp";
const int SSH_CONNECT_RETRY_INTERVAL = 5;
const int SSH_CONNECT_MAX_RETRY = 20;
const std::string DEFAULT_SSH_OPTIONS =
		" -o UserKnownHostsFile=/dev/null"
		" -o StrictHostKeyChecking=no"
		" -o PasswordAuthentication=no";

/**
 * \brief Constructor
 * \param user the user login
 * \param hostname the hostname of the machine
 * \param batchType the type of the batch scheduler
 * \param jobSerialized the job serialized
 * \param submitOptionsSerialized the job options serialized
 */
SSHJobExec::SSHJobExec(const std::string& user,
                       const std::string& hostname,
                       const BatchType& batchType,
                       const std::string& batchVersion,
                       const std::string& jobSerialized,
                       const std::string& submitOptionsSerialized):
  muser(user), mhostname(hostname), mbatchType(batchType),
  mjobSerialized(jobSerialized),
  msubmitOptionsSerialized(submitOptionsSerialized),
  mcloudEndpoint("")
{
  if (batchVersion.empty()) {
    mbatchVersion = "-";  // mbatchVersion must never be empty
  } else {
    mbatchVersion = batchVersion;
  }
}

/**
 * \brief Destructor
 */
SSHJobExec::~SSHJobExec() {

}
/**
 * \brief Function to return the job serialized content
 * \return  job serialized content
 */
std::string
SSHJobExec::getJobSerialized() {
	return mjobSerialized;
}

/**
 * \brief Function to return the error message of a service
 * \return error message information
 */
std::string
SSHJobExec::getErrorInfo() {
	return merrorInfo;
}

/**
 * \brief Function to check the parameters before launching ssh
 * \return raises an exception on error
 */
void
SSHJobExec::checkSshParams() {
	if (muser.empty()) {
		throw SystemException(ERRCODE_SSH, "User login is empty");
	}
	if (mhostname.empty()) {
		throw SystemException(ERRCODE_SSH, "Server hostname is empty");
	}
}


/**
 * \brief Function to execute command by using ssh
 * \param slaveDirectory the path to the command executable
 * \param serviceName the name of the service to execute
 * \param script_path the path to script to submit
 * \return raises an exception on error
 */
void
SSHJobExec::sshexec(const std::string& slaveDirectory,
		const std::string& serviceName,
		const std::string& script_path) {

	checkSshParams();

	std::string jobSerializedPath;
	std::string submitOptionsSerializedPath;
	std::string jobUpdateSerializedPath;
	std::string errorPath;
	std::string stderrFilePath;
	std::string cmdDetails;
	bool wellSubmitted = false;
	bool errorMsgIsSet = false;

	jobSerializedPath = TMS_SERVER_FILES_DIR+"/jobSerializedXXXXXX";
	vishnu::createTmpFile(const_cast<char*>(jobSerializedPath.c_str()), mjobSerialized);

	submitOptionsSerializedPath = TMS_SERVER_FILES_DIR+"/submitOptionsSerializedXXXXXX";
	vishnu::createTmpFile(const_cast<char*>(submitOptionsSerializedPath.c_str()), msubmitOptionsSerialized);

	jobUpdateSerializedPath = TMS_SERVER_FILES_DIR+"/jobUpdateSerializedXXXXXX";
	vishnu::createTmpFile(const_cast<char*>(jobUpdateSerializedPath.c_str()));

	errorPath = TMS_SERVER_FILES_DIR+"/errorPathXXXXXX";
	vishnu::createTmpFile(const_cast<char*>(errorPath.c_str()));

	std::ostringstream cmd;
	cmdDetails = "" ;
	if(serviceName.compare("SUBMIT")==0) {
		// set specific arguments for submit job
		cmdDetails += jobUpdateSerializedPath
				+ " " +  submitOptionsSerializedPath
				+ " " + script_path;
	}

	if(mbatchType != DELTACLOUD) {
		// For traditional batch scheduler we need to submit the job through ssh
		cmd << "ssh -l " << muser << " " << mhostname << " "
				<< " -o NoHostAuthenticationForLocalhost=yes "
				<< " -o PasswordAuthentication=no ";
	} else {
		// set the information to authenticate against the cloud frontend
		if(muser.size() >0) {
			setenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_USER].c_str(), muser.c_str(), 1);
		}
		if(mcloudEndpoint.size() > 0) {
			setenv(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_ENDPOINT].c_str(), mcloudEndpoint.c_str(), 1);
		}
	}

	stderrFilePath = TMS_SERVER_FILES_DIR+"/stderrFilePathXXXXXX";
	vishnu::createTmpFile(const_cast<char*>(stderrFilePath.c_str()));

	cmd << slaveDirectory << "/tmsSlave "
			<< serviceName << " "
			<< convertBatchTypeToString(mbatchType) << " "
			<< mbatchVersion << " "
			<< jobSerializedPath << " " <<  errorPath << " "
			<< cmdDetails
			<< " 2> " << stderrFilePath;

	int ret;
        std::cerr << cmd.str() << std::endl;
	if((ret=system((cmd.str()).c_str()))) {
		vishnu::deleteFile(jobSerializedPath.c_str());
		vishnu::deleteFile(submitOptionsSerializedPath.c_str());
		vishnu::deleteFile(jobUpdateSerializedPath.c_str());

		//begin
		boost::filesystem::path errorFile(errorPath.c_str());
		if (!boost::filesystem::is_empty(errorFile)) {
			merrorInfo = vishnu::get_file_content(errorPath);
			merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
			return;
		}
		//end

		vishnu::deleteFile(errorPath.c_str());
		vishnu::deleteFile(script_path.c_str());
		boost::filesystem::path stderrFile(stderrFilePath.c_str());
		if(!boost::filesystem::is_empty(stderrFile)) {
			merrorInfo = vishnu::get_file_content(stderrFilePath);
			if(merrorInfo.find("password")!=std::string::npos) {
				merrorInfo.append("  You must copy the VISHNU publickey in your authorized_keys file.");
			}
		}
		if ( WEXITSTATUS(ret) == 1 && mbatchType==SLURM) {//ATTENTION: 1 corresponds of the error_exit value in ../slurm_parser/opt.c
			merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
			vishnu::deleteFile(stderrFilePath.c_str());
			throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR, "SLURM ERROR: "+merrorInfo);
		}
		vishnu::deleteFile(stderrFilePath.c_str());
		throw SystemException(ERRCODE_SSH, merrorInfo);
	}

	boost::filesystem::path jobUpdateSerializedFile(jobUpdateSerializedPath);
	if (!boost::filesystem::is_empty(jobUpdateSerializedFile)) {
		std::string jobSerialized = vishnu::get_file_content(jobUpdateSerializedPath);
		TMS_Data::Job_ptr job = NULL;
		if(!vishnu::parseEmfObject(std::string(jobSerialized), job)) {
			vishnu::deleteFile(jobSerializedPath.c_str());
			vishnu::deleteFile(submitOptionsSerializedPath.c_str());
			vishnu::deleteFile(jobUpdateSerializedPath.c_str());
			vishnu::deleteFile(errorPath.c_str());
			vishnu::deleteFile(stderrFilePath.c_str());
			throw SystemException(ERRCODE_INVDATA, "SSHJobExec::sshexec: job object is not well built");
		}
		::ecorecpp::serializer::serializer _ser;//("job");
		mjobSerialized = _ser.serialize_str(job);
		wellSubmitted = true;
		delete job;
	}

	boost::filesystem::path errorFile(errorPath.c_str());
	if (!boost::filesystem::is_empty(errorFile)) {
		merrorInfo = vishnu::get_file_content(errorPath);
		merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
		errorMsgIsSet=true;
	}

	if ((mbatchType==LOADLEVELER || mbatchType==LSF) && (wellSubmitted==false) && (errorMsgIsSet==false)) {
		boost::filesystem::path stderrFile(stderrFilePath.c_str());
		if(!boost::filesystem::is_empty(stderrFile)) {
			merrorInfo = vishnu::get_file_content(stderrFilePath);

			std::ostringstream errorMsgSerialized;
			if(mbatchType==LOADLEVELER){
				errorMsgSerialized << ERRCODE_BATCH_SCHEDULER_ERROR << "#" << "LOADLEVELER ERROR: ";
			}
			if(mbatchType==LSF){
				errorMsgSerialized << ERRCODE_BATCH_SCHEDULER_ERROR << "#" << "LSF ERROR: ";
			}
			errorMsgSerialized << merrorInfo;
			merrorInfo = errorMsgSerialized.str();
			merrorInfo = merrorInfo.substr(0, merrorInfo.find_last_of('\n'));
		}
	}

	vishnu::deleteFile(jobSerializedPath.c_str());
	vishnu::deleteFile(submitOptionsSerializedPath.c_str());
	vishnu::deleteFile(jobUpdateSerializedPath.c_str());
	vishnu::deleteFile(errorPath.c_str());
	vishnu::deleteFile(stderrFilePath.c_str());
}

/**
 * \brief Function to execute a script remotely
 * \param scriptPath the path to script to submit
 * \param nfsServer: The NFS server
*  \param nfsMountPointthe mount point on the NFS server
 * \return raises an exception on error
 */
int
SSHJobExec::execRemoteScript(const std::string& scriptPath,
		const std::string & nfsServer,
		const std::string nfsMountPoint) {

	const std::string machineStatusFile = "/tmp/"+mhostname+".status";
	std::ostringstream sshCmd;

	sshCmd << "ssh -q -o ConnectTimeout=2 " << DEFAULT_SSH_OPTIONS << " "
			<< muser << "@" << mhostname
			<< " exit;"
			<< " echo $? >" << machineStatusFile;

	int retry = SSH_CONNECT_MAX_RETRY;
	while(retry >0) {
		system(sshCmd.str().c_str());
		std::string content = vishnu::get_file_content(machineStatusFile);
		size_t pos = content.find("\n");
		int ret = -1;
		if(pos != std::string::npos) {
			ret = vishnu::convertToInt(content.substr(0, pos));
		}
		if(ret == 0) {
			break;
		}
		sleep(SSH_CONNECT_RETRY_INTERVAL);
		retry--;
	}

	// If not succeed throw exception
	if(retry == 0) {
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"execRemoteScript:: can't log into the machine "+mhostname+" after "
				+ vishnu::convertToString(SSH_CONNECT_MAX_RETRY*SSH_CONNECT_RETRY_INTERVAL)+" seconds");
	}

	// Mount the NFS repository
	if(nfsServer.size()>0 &&
			nfsMountPoint.size()> 0) {
		mountNfsDir(nfsServer, nfsMountPoint);
	}

	// If succeed execute the script to the virtual machine
	// This assumes that the script is located on a shared DFS
	int pid = -1;
	if(execCmd(scriptPath, true, &pid)){
		throw TMSVishnuException(ERRCODE_BATCH_SCHEDULER_ERROR,
				"execRemoteScript:: failed when executing the script " + scriptPath + " in the virtual machine "+mhostname);
	}

	return pid;
}


/**
 * \brief Function to convert the batch type to string
 * \param BatchType the batch type to convert
 * \return the converted batch type
 */
std::string SSHJobExec::convertBatchTypeToString(BatchType batchType) {
	std::string value;
	switch(batchType) {
	case TORQUE:
		value = "TORQUE";
		break;
	case LOADLEVELER:
		value = "LOADLEVELER";
		break;
	case SLURM:
		value = "SLURM";
		break;
	case LSF:
		value = "LSF";
		break;
	case SGE:
		value = "SGE";
		break;
        case PBSPRO:
                value = "PBSPRO";
                break;
	case DELTACLOUD:
		value = "DELTACLOUD";
		break;
        case POSIX:
                value = "POSIX";
                break;
	default:
		value = "UNKNOWN_BATCH_TYPE";
		break;
	}
	return value;
}

/**
 * \brief Function to copy files from remote machine
 * \param outputPath the output path to get
 * \param errorPath the error path to get
 * \param copyOfOutputPath the copy of the outputPath
 * \param copyOfErrorPath the copy of errorPath
 * \return raises an exception on error
 */
int
SSHJobExec::copyFiles(const std::string& outputPath,
		const std::string& errorPath,
		const char* copyOfOutputPath,
		const char* copyOfErrorPath) {

	std::ostringstream cmd1;
	cmd1 << "scp " << DEFAULT_SSH_OPTIONS << " "
			<< muser << "@" << mhostname << ":" << outputPath << " " << copyOfOutputPath;

	if (system((cmd1.str()).c_str())) {
		return -1;
	}

	std::ostringstream cmd2;
	cmd2 << "scp "<< DEFAULT_SSH_OPTIONS << " "
			<< muser << "@" << mhostname << ":" << errorPath << " " << copyOfErrorPath;
	if (system((cmd2.str()).c_str())) {
		return -1;
	}

	return 0;
}

/**
 * \brief Function to copy files from remote machine
 * \param path the path of the file
 * \return raises an exception on error
 */
int
SSHJobExec::copyFile(const std::string& path, const std::string& dest) {

	std::ostringstream cmd1;
	cmd1 << "scp " << DEFAULT_SSH_OPTIONS << " "
			<< muser << "@" << mhostname << ":" << path << " " << dest;
	if (system((cmd1.str()).c_str())) {
		return -1;
	}
	return 0;
}

/**
 * \brief Function to execute a command via ssh
 * \param cmd the command to execute
 * \param background: Tell whether launch the script is background
 * \param pidFile: The path of the file containing the process pid
 */
int
SSHJobExec::execCmd(const std::string& cmd, const bool & background, int* pid){

	std::string pidFile = "vishnu.pid";
	std::ostringstream sshCmd;
	sshCmd << "ssh " << DEFAULT_SSH_OPTIONS << " "
			<< muser << "@" << mhostname << " '"
			<< cmd ;

	if( ! background) {
		sshCmd << "'";
	} else {
		try {
			pidFile =  bfs::unique_path("/tmp/vishnu.pid%%%%%%").string();
		} catch(...) {
			// The pid file will be created in $HOME/vishnu.pid
		}
		sshCmd << "& exit $!'; echo $? > " << pidFile;
	}

	std::cout << "SSH CMD:::" << sshCmd.str() << std::endl;
	if(system((sshCmd.str()).c_str())) {
		return -1;
	}

	std::string content = vishnu::get_file_content(pidFile);
	size_t pos = content.find("\n");

	if(pid != NULL) {
		if(pos != std::string::npos) {
			*pid = vishnu::convertToInt(content.substr(0, pos));
		} else {
			*pid = -1;
		}
	}

	return 0;
}

/**
 * \brief Set the value of the cloud endpoint
 */
void
SSHJobExec::setCloudEndpoint(const std::string & cloupApiUrl) {
	mcloudEndpoint = cloupApiUrl;
}

/**
* \brief To mount a NFS directory to a remote server
* \param host: The NFS server
* \param point the mount point on the NFS server
*/
void
SSHJobExec::mountNfsDir(const std::string & host, const std::string point) {
	execCmd("mkdir "+point+" 2>vishnu.log", false); // run in foreground
	execCmd("mount -t nfs -o rw "+host+":"+point+" "+point+" 2>>vishnu.log", false);
}
