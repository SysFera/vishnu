/**
 * \file submit_job.cpp
 * This file defines the VISHNU submit job command
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com) and Daouda Traore (daouda.traore@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_tms.hpp"
#include "sessionUtils.hpp"
#include "displayer.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief To build options for the VISHNU submit job command
 * \param pgName : The name of the command
 * \param fname: The name of the job
 * \param fqueue : The queue to set the job
 * \param fwallTime : The wall time for the job
 * \param fmemory : The memory needed by the job
 * \param fnbCpu : The number of cpu per node needed
 * \param fnbNodeAndCpu : The number of node and processor per node
 * \param foutput : The output path
 * \param ferr : The error path
 * \param fmailNotif : The type of the nofication (BEGIN, END, ERROR, and ALL)
 * \param fmailUser : The name of user to receive email notification
 * \param fgroup : The job group name
 * \param fworkingDir : The job working directory
 * \param fcpuTime : The cpu time limit of the job
 * \param loadCriterionStr : The load value to use (for now three criterions are be used:
 *  minimum number of waiting jobs, minimum number of running jobs and the total number of job )
 * \param dietConfig: Represents the VISHNU config file
 * \param dietConfig: Represents the VISHNU config file
 * \return The description of all options allowed by the command
 */
boost::shared_ptr<Options>

makeSubJobOp(string pgName, 
		boost::function1<void, string>& fname,
		boost::function1<void, string>& fqueue,
		boost::function1<void, int>& fmemory,
		boost::function1<void, int>& fnbCpu,
		boost::function1<void, string>& fnbNodeAndCpu,
		boost::function1<void, string>& foutput,
		boost::function1<void, string>& ferr,
		boost::function1<void, string>& fmailNotif,
		boost::function1<void, string>& fmailUser,
		boost::function1<void, string>& fgroup,
		boost::function1<void, string>& fworkingDir,
		boost::function1<void, string>& fcpuTime,
		boost::function1<void, string>& ftextParams,
		boost::function1<void, string>& ffileParams,
		vector<string>& textParamsVector,
		vector<string>& fileParamsVector,
		string& loadCriterionStr,
		string& walltime,
		string& dietConfig){
	boost::shared_ptr<Options> opt(new Options(pgName));

	// Environement option
	opt->add("dietConfig,c",
			"The diet config file",
			ENV,
			dietConfig);

	// All cli options
	opt->add("name,n",
			"The name of the job",
			CONFIG,
			fname);
	opt->add("queue,q",
			"The queue to submit the job",
			CONFIG,
			fqueue);
	opt->add("wallTime,t",
			"The wall time for the job (in secondes or in the format [[HH:]MM:]SS)",
			CONFIG,
			walltime);
	opt->add("memory,m",
			"The memory needed by the job (in MegaBytes)",
			CONFIG,
			fmemory);
	opt->add("nbCpu,P",
			"The number of cpu per node needed by the job",
			CONFIG,
			fnbCpu);
	opt->add("nbNodesAndCpuPerNode,N",
			"The number of node and processor per node",
			CONFIG,
			fnbNodeAndCpu);
	opt->add("outPutPath,o",
			"The outputh path on the remote machine of the job",
			CONFIG,
			foutput);
	opt->add("errorPath,e",
			"The error path on the remote machine of the job",
			CONFIG,
			ferr);

	opt->add("mailNotification,M",
			"Assigns the notification type of the job. Valid type values are "
			"BEGIN, END, ERROR, and ALL (any state change).",
			CONFIG,
			fmailNotif);
	opt->add("mailNotifyUser,u",
			"The name of user to receive email notification of state changes as defined "
			"by the option mailNotification. The default value is the submitting user.",
			CONFIG,
			fmailUser);
	opt->add("group,g",
			"Assigns a job group name.",
			CONFIG,
			fgroup);
	opt->add("workingDir,D",
			"Assigns a job remote working dir",
			CONFIG,
			fworkingDir);
	opt->add("cpuTime,T",
			"Assigns a job cpu limit time (in seconds or in the format [[HH:]MM:]SS)",
			CONFIG,
			fcpuTime);
	opt->add("useLoad,L",
			" The criterion to automatically submit a job (for now three criterions are used:"
			" minimum number of waiting jobs, minimum number of running jobs and the total"
			"number of jobs). Predefined values are:\n"
			"0 or W: To select a machine that has minimum number of waiting jobs\n"
			"1 or T: To select a machine that has minimum number of total jobs (wainting and running)\n"
			"2 or R: To select a machine that has minimum number of running jobs",
			CONFIG,
			loadCriterionStr);
	opt->add("textParam,v",
			"Sets a textual parameter with a value passing in argument.\n"
			"E.g. --param PARAM1=value1. So at the execution, any environment variable named"
			"PARAM1 in the script will be set with 'value1'.\n"
			"The name of the parameter can be any word and is case-insensitive, but the corresponding "
			"environment variable must be in upper case in the script.\n\n"
			"SEE ALSO --textParams.",
			CONFIG,
			textParamsVector);
	opt->add("textParams,V",
			"Sets a list of space-separated textual parameters.\n"
			"E.g. --listParams=\"PARAM1=value1 PARAM2=value2.\n\n"
			"SEE ALSO --params.",
			CONFIG,
			ftextParams);
	opt->add("fileParam,f",
			"Sets a local file as a file parameter for the script. This file will be uploaded onto the server "
			"before computing the script.\n"
			"E.g. --file PFILE1=/path/to/file will allow to upload the file located at '/path/to/file' "
			"onto the server.\n"
			"Then within the script, the path of the destination files can be accessed through an "
			"environment named PFILE1.\n"
			"The name of the parameter can be any word and is case-insensitive, but the corresponding "
			"environment variable must be in upper case in the script.\n\n"
			"SEE ALSO --fileParams.",
			CONFIG,
			fileParamsVector);
	opt->add("fileParams,F",
			"Sets a list of local files as parameters for the script. These files will be  uploaded onto the server "
			"before computing the script.\n"
			"E.g. --listFile=\"PFILE1=/path/to/file1 PFILE2=/path/to/file2\".\n"
			"Then within the script, the paths of the destination files can be accessed through the environment"
			"variables VISHNU_PFILE1 and VISHNU_PFILE2, respectively.\n\n"
			"SEE ALSO --file.",
			CONFIG,
			ffileParams);

	return opt;
}


int main (int argc, char* argv[]){
	CLICmd cmd = CLICmd (argc, argv, opt);

	// Process command
	try {

		// Parse the cli and setting the options found
		ret = cmd.parse(env_name_mapper());

		if (ret != CLI_SUCCESS){
			helpUsage(*opt,"[options] machineId script");
			return ret;
		}

		// PreProcess (adapt some parameters if necessary)
		checkVishnuConfig(*opt);
		if ( opt->count("help")){
			helpUsage(*opt,"[options] machineId script");
			return 0;
		}

		if(walltime.size()!=0) {
			subOp.setWallTime(convertStringToWallTime(walltime));
		}

		//To set the load criterion
		int loadCriterionType;
		TMS_Data::LoadCriterion_ptr loadCriterion_ptr = new TMS_Data::LoadCriterion();
		if(loadCriterionStr.size()!=0) {
			size_t pos = loadCriterionStr.find_first_not_of("0123456789");
			if(pos!=std::string::npos) {
				if(loadCriterionStr.size()==1) {
					switch(loadCriterionStr[0]) {
					case 'W' :
						loadCriterionType = 0;
						break;
					case 'T' :
						loadCriterionType = 1;
						break;
					case 'R' :
						loadCriterionType = 2;
						break;
					default:
						loadCriterionType = 0;
						break;
					}
				}
				if ((loadCriterionStr.size() > 1)) {
					std::cerr << "Unknown load criterion " << loadCriterionStr << std::endl;
					return -1;
				}
			} else {
				loadCriterionType = convertToInt(loadCriterionStr);
			}
			loadCriterion_ptr->setLoadType(loadCriterionType);
			subOp.setCriterion(loadCriterion_ptr);
		}

		if(opt->count("selectQueueAutom")) {
			subOp.setSelectQueueAutom(true);
		}

		//Validate textual parameters syntax, if any
		string paramOptName = "textParams" ;

		if(opt->count(paramOptName) || textParamsVector.size() != 0) {
			string paramStr ;
			int ret = vishnu::validateParameters(opt, paramStr, paramOptName, textParamsVector, false);
			if( ret != 0 ) return ret ;
			subOp.setTextParams(paramStr) ;
		}

		//Validate file parameters syntax, if any
		paramOptName = "fileParams" ;
		if(opt->count(paramOptName) || fileParamsVector.size() != 0 ) {
			string paramStr ;
			int ret = vishnu::validateParameters(opt, paramStr, paramOptName, fileParamsVector, true) ;
			if( ret != 0 ) return ret ;
			subOp.setFileParams(paramStr) ;
		}

		// initializing DIET
		if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), argc, argv)) {
			errorUsage(argv[0],dietErrorMsg,EXECERROR);
			return  CLI_ERROR_DIET ;
		}

		// get the sessionKey
		sessionKey=getLastSessionKey(getppid());

		// DIET call : submit
		if(false==sessionKey.empty()){
			printSessionKeyMessage();
			submitJob(sessionKey, machineId, scriptPath, job, subOp);
		}

		displaySubmit(job);
		printSuccessMessage();
	} catch(VishnuException& except){// catch all Vishnu runtime error
		std::string  submitErrmsg = except.getMsg()+" ["+except.getMsgComp()+"]";
		errorUsage(argv[0], submitErrmsg,EXECERROR);
		//check the bad session key
		if (checkBadSessionKeyError(except)){
			removeBadSessionKeyFromFile(getppid());
		}
		return except.getMsgI() ;
	} catch(std::exception& except){// catch all std runtime error
		errorUsage(argv[0],except.what());
		return CLI_ERROR_RUNTIME;
	}

  int ret; // Return value

  /******* Parsed value containers ****************/
  string dietConfig;
  string sessionKey;
  string machineId;
  string scriptPath;
  string walltime;

  /********** EMF data ************/
  TMS_Data::SubmitOptions subOp;

  /******** Callback functions ******************/
  boost::function1<void,string> fname(boost::bind(&TMS_Data::SubmitOptions::setName,boost::ref(subOp),_1));
  boost::function1<void,string> fqueue(boost::bind(&TMS_Data::SubmitOptions::setQueue,boost::ref(subOp),_1));
  boost::function1<void,int> fmemory(boost::bind(&TMS_Data::SubmitOptions::setMemory,boost::ref(subOp),_1));
  boost::function1<void,int> fnbCpu(boost::bind(&TMS_Data::SubmitOptions::setNbCpu,boost::ref(subOp),_1));
  boost::function1<void,string> fnbNodeAndCpu(boost::bind(&TMS_Data::SubmitOptions::setNbNodesAndCpuPerNode,boost::ref(subOp),_1));
  boost::function1<void,string> foutput(boost::bind(&TMS_Data::SubmitOptions::setOutputPath,boost::ref(subOp),_1));
  boost::function1<void,string> ferr(boost::bind(&TMS_Data::SubmitOptions::setErrorPath,boost::ref(subOp),_1));
  boost::function1<void,string> fmailNotif(boost::bind(&TMS_Data::SubmitOptions::setMailNotification,boost::ref(subOp),_1));
  boost::function1<void,string> fmailUser(boost::bind(&TMS_Data::SubmitOptions::setMailNotifyUser,boost::ref(subOp),_1));
  boost::function1<void,string> fgroup(boost::bind(&TMS_Data::SubmitOptions::setGroup,boost::ref(subOp),_1));
  boost::function1<void,string> fworkingDir(boost::bind(&TMS_Data::SubmitOptions::setWorkingDir,boost::ref(subOp),_1));
  boost::function1<void,string> fcpuTime(boost::bind(&TMS_Data::SubmitOptions::setCpuTime,boost::ref(subOp),_1));
  std::string loadCriterionStr;
  /*********** Out parameters *********************/
  Job job;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt=makeSubJobOp(argv[0],fname,fqueue,
                fmemory, fnbCpu, fnbNodeAndCpu,
                foutput, ferr, fmailNotif, fmailUser, fgroup, fworkingDir, fcpuTime, loadCriterionStr, walltime, dietConfig);

  opt->add("selectQueueAutom,Q",
      "allows to select automatically a queue which has the number of nodes requested by the user.",
      CONFIG);

  opt->add("machineId,i",
	   "represents the id of the machine",
	   HIDDEN,
	   machineId,1);
  opt->setPosition("machineId",1);

  opt->add("scriptPath,p",
	   "represents the script of submission",
	   HIDDEN,
	   scriptPath,1);
  opt->setPosition("scriptPath",1);

  CLICmd cmd = CLICmd (argc, argv, opt);

  // Process command
  try {

    // Parse the cli and setting the options found
    ret = cmd.parse(env_name_mapper());

    if (ret != CLI_SUCCESS){
      helpUsage(*opt,"[options] machineId script");
      return ret;
    }

    // PreProcess (adapt some parameters if necessary)
    checkVishnuConfig(*opt);
    if ( opt->count("help")){
      helpUsage(*opt,"[options] machineId script");
      return 0;
    }

    if(walltime.size()!=0) {
      subOp.setWallTime(convertStringToWallTime(walltime));
    }

    //To set the load criterion
    int loadCriterionType;
    TMS_Data::LoadCriterion_ptr loadCriterion_ptr = new TMS_Data::LoadCriterion();
    if(loadCriterionStr.size()!=0) {
      size_t pos = loadCriterionStr.find_first_not_of("0123456789");
      if(pos!=std::string::npos) {
        if(loadCriterionStr.size()==1) {
          switch(loadCriterionStr[0]) {
            case 'W' :
              loadCriterionType = 0;
              break;
            case 'T' :
              loadCriterionType = 1;
              break;
            case 'R' :
              loadCriterionType = 2;
              break;
            default:
              loadCriterionType = 0;
              break;
          }
        }
        if ((loadCriterionStr.size() > 1)) {
          std::cerr << "Unknown load criterion " << loadCriterionStr << std::endl;
          return 0;
        }
      } else {
        loadCriterionType = convertToInt(loadCriterionStr);
      }
      loadCriterion_ptr->setLoadType(loadCriterionType);
      subOp.setCriterion(loadCriterion_ptr);
    }

    if(opt->count("selectQueueAutom")) {
      subOp.setSelectQueueAutom(true);
    }

    // initializing DIET
    if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), argc, argv)) {
      errorUsage(argv[0],dietErrorMsg,EXECERROR);
      return  CLI_ERROR_DIET ;
    }

    // get the sessionKey
    sessionKey=getLastSessionKey(getppid());

    // DIET call : submit
    if(false==sessionKey.empty()){
      printSessionKeyMessage();
      submitJob(sessionKey, machineId, scriptPath, job, subOp);
    }
    displaySubmit(job);
    printSuccessMessage();
  } catch(VishnuException& except){// catch all Vishnu runtime error
    std::string  submitErrmsg = except.getMsg()+" ["+except.getMsgComp()+"]";
    errorUsage(argv[0], submitErrmsg,EXECERROR);
    //check the bad session key
    if (checkBadSessionKeyError(except)){
      removeBadSessionKeyFromFile(getppid());
    }
    return except.getMsgI() ;
  } catch(std::exception& except){// catch all std runtime error
    errorUsage(argv[0],except.what());
    return CLI_ERROR_RUNTIME;
  }

  return 0;
}
