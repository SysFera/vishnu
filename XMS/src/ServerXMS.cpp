#include "ServerXMS.hpp"
#include <boost/algorithm/string.hpp>
#include "AuthenticatorFactory.hpp"
#include "DbFactory.hpp"
#include "internalApiUMS.hpp"
#include "internalApiTMS.hpp"
#include "utilVishnu.hpp"
#include "UMSServices.hpp"
#include "TMSServices.hpp"
#include "FMSServices.hpp"
#include "internalApiFMS.hpp"


Database *ServerXMS::mdatabaseVishnu = NULL;
ServerXMS *ServerXMS::minstance = NULL;
UMSMapper *ServerXMS::mmapperUMS = NULL;
TMSMapper *ServerXMS::mmapperTMS = NULL;
FMSMapper *ServerXMS::mmapperFMS = NULL;
Authenticator *ServerXMS::mauthenticator = NULL;


ServerXMS*
ServerXMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerXMS();
  }
  return minstance;
}

int
ServerXMS::getVishnuId() const {
  return mvishnuId;
}

std::string
ServerXMS::getSendmailScriptPath() const {
  return msendmailScriptPath;
}

ExecConfiguration_Ptr
ServerXMS::getSedConfig() const {
  return msedConfig;
}

std::vector<std::string>
ServerXMS::getDefaultBatchOption() const {
  return mdefaultBatchOption;
}

BatchType
ServerXMS::getBatchType() const {
  return mbatchType;
}

std::string
ServerXMS::getBatchVersion() const {
  return mbatchVersion;
}

Database*
ServerXMS::getDatabaseVishnu() {
    return mdatabaseVishnu;
}

int
ServerXMS::init(SedConfig& cfg) {
  using vishnu::convertToString;
  using vishnu::UMSMAPPERNAME;
  using vishnu::FMSMAPPERNAME;
  using vishnu::TMSMAPPERNAME;

  msendmailScriptPath = cfg.sendmailScriptPath;
  DbFactory factory;
  AuthenticatorFactory authfactory;
  int errorCode(0);

  setenv("VISHNU_DEBUG_LEVEL", vishnu::convertToString(mdebugLevel).c_str(), 0);

  //set the machineId
  mmachineId = cfg.mid;

  // initialize the SeD configuration object
  msedConfig = &cfg.config;

  mhasUMS = cfg.hasUMS;
  mhasTMS = cfg.hasTMS;
  mhasFMS = cfg.hasFMS;

  // set the debug level
  bool isSetDebugLevel = msedConfig->getConfigValue(vishnu::DEBUG_LEVEL, mdebugLevel);
  if (!isSetDebugLevel || mdebugLevel < 0) {
    mdebugLevel = 0;
  }

  //initialization of the batchType
  mbatchType = cfg.batchType;

  if (mhasTMS) {
    //initialization of the batchVersion
    if (mbatchType != DELTACLOUD && mbatchType != OPENNEBULA) {
      msedConfig->getRequiredConfigValue<std::string>(vishnu::BATCHVERSION, mbatchVersion);
    }

    std::string batchDefaultConfigFile;
    if (msedConfig->getConfigValue(vishnu::DEFAULTBATCHCONFIGFILE, batchDefaultConfigFile)) {
      switch(mbatchType) {
      case TORQUE:
        getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#PBS");
        break;
      case LOADLEVELER:
        getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "# @");
        break;
      case SLURM:
        getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#SBATCH");
        break;
      case LSF:
        getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#BSUB");
        break;
      case SGE:
        getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#$");
        break;
      case PBSPRO:
        getConfigOptions(batchDefaultConfigFile.c_str(), mdefaultBatchOption, "#PBS");
        break;
      case DELTACLOUD:
        //No yet supported
        break;
      case POSIX:
        //No yet supported
        break;
      default:
        break;
      }
    }
  }

  try {
    mdatabaseVishnu = factory.createDatabaseInstance(cfg.dbConfig);
    mauthenticator = authfactory.createAuthenticatorInstance(cfg.authenticatorConfig);

    mvishnuId = cfg.vishnuId;

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));

    /*connection to the database*/
    mdatabaseVishnu->connect();
    mmapperTMS = new TMSMapper(MapperRegistry::getInstance(), TMSMAPPERNAME);
    mmapperTMS->registerMapper();
    mmapperFMS = new FMSMapper(MapperRegistry::getInstance(), FMSMAPPERNAME);
    mmapperFMS->registerMapper();
    mmapperUMS = new UMSMapper(MapperRegistry::getInstance(), UMSMAPPERNAME);
    mmapperUMS->registerMapper();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }

  } catch (VishnuException& e) {
    std::cout << e.what() << "\n";
    errorCode = 1;
  }

// initialization of the service table
  initMap(cfg.mid);

  return errorCode;
}

ServerXMS::ServerXMS() {
}

ServerXMS::~ServerXMS() {
  delete mmapperUMS;
  delete mdatabaseVishnu;
  delete mauthenticator;
}

void
ServerXMS::initMap(const std::string& mid) {
  int (*functionPtr)(diet_profile_t*);
  mcb["heartbeatxmssed@"+mmachineId] = boost::ref(heartbeat);
  if (mhasUMS) {
      functionPtr = solveSessionConnect;
      mcb[SERVICES_UMS[SESSIONCONNECT]] = functionPtr;
      functionPtr = solveSessionReconnect;
      mcb[SERVICES_UMS[SESSIONRECONNECT]] = functionPtr;
      functionPtr = solveSessionClose;
      mcb[SERVICES_UMS[SESSIONCLOSE]] = functionPtr;
      functionPtr = solveUserCreate;
      mcb[SERVICES_UMS[USERCREATE]] = functionPtr;
      functionPtr = solveUserUpdate;
      mcb[SERVICES_UMS[USERUPDATE]] = functionPtr;
      functionPtr = solveUserDelete;
      mcb[SERVICES_UMS[USERDELETE]] = functionPtr;
      functionPtr = solveUserPasswordChange;
      mcb[SERVICES_UMS[USERPASSWORDCHANGE]] = functionPtr;
      functionPtr = solveUserPasswordReset;
      mcb[SERVICES_UMS[USERPASSWORDRESET]] = functionPtr;
      functionPtr = solveMachineCreate;
      mcb[SERVICES_UMS[MACHINECREATE]] = functionPtr;
      functionPtr = solveMachineUpdate;
      mcb[SERVICES_UMS[MACHINEUPDATE]] = functionPtr;
      functionPtr = solveMachineDelete;
      mcb[SERVICES_UMS[MACHINEDELETE]] = functionPtr;
      functionPtr = solveLocalAccountCreate;
      mcb[SERVICES_UMS[LOCALACCOUNTCREATE]] = functionPtr;
      functionPtr = solveLocalAccountUpdate;
      mcb[SERVICES_UMS[LOCALACCOUNTUPDATE]] = functionPtr;
      functionPtr = solveLocalAccountDelete;
      mcb[SERVICES_UMS[LOCALACCOUNTDELETE]] = functionPtr;
      functionPtr = solveOptionValueSet;
      mcb[SERVICES_UMS[OPTIONVALUESET]] = functionPtr;
      functionPtr = solveOptionValueSetDefault;
      mcb[SERVICES_UMS[OPTIONVALUESETDEFAULT]] = functionPtr;
      functionPtr = solveListSessions;
      mcb[SERVICES_UMS[SESSIONLIST]] = functionPtr;
      functionPtr = solveListLocalAccount;
      mcb[SERVICES_UMS[LOCALACCOUNTLIST]] = functionPtr;
      functionPtr = solveListMachines;
      mcb[SERVICES_UMS[MACHINELIST]] = functionPtr;
      functionPtr = solveListHistoryCmd;
      mcb[SERVICES_UMS[COMMANDLIST]] = functionPtr;
      functionPtr = solveListOptions;
      mcb[SERVICES_UMS[OPTIONVALUELIST]] = functionPtr;
      functionPtr = solveListUsers;
      mcb[SERVICES_UMS[USERLIST]] = functionPtr;
      functionPtr = solveRestore;
      mcb[SERVICES_UMS[RESTORE]] = functionPtr;
      functionPtr = solveSystemAuthCreate;
      mcb[SERVICES_UMS[AUTHSYSTEMCREATE]] = functionPtr;
      functionPtr = solveSystemAuthUpdate;
      mcb[SERVICES_UMS[AUTHSYSTEMUPDATE]] = functionPtr;
      functionPtr = solveSystemAuthDelete;
      mcb[SERVICES_UMS[AUTHSYSTEMDELETE]] = functionPtr;
      functionPtr = solveSystemAuthList;
      mcb[SERVICES_UMS[AUTHSYSTEMLIST]] = functionPtr;
      functionPtr = solveAccountAuthCreate;
      mcb[SERVICES_UMS[AUTHACCOUNTCREATE]] = functionPtr;
      functionPtr = solveAccountAuthUpdate;
      mcb[SERVICES_UMS[AUTHACCOUNTUPDATE]] = functionPtr;
      functionPtr = solveAccountAuthDelete;
      mcb[SERVICES_UMS[AUTHACCOUNTDELETE]] = functionPtr;
      functionPtr = solveAccountAuthList;
      mcb[SERVICES_UMS[AUTHACCOUNTLIST]] = functionPtr;
      mcb[SERVICES_UMS[INT_DEFINEUSERIDENTIFIER]] = solveSetUID;
      mcb[SERVICES_UMS[INT_DEFINEJOBIDENTIFIER]] = solveSetJID;
      mcb[SERVICES_UMS[INT_DEFINETRANSFERIDENTIFIER]] = solveSetTID;
      mcb[SERVICES_UMS[INT_DEFINEMACHINEIDENTIFIER]] = solveSetMID;
      mcb[SERVICES_UMS[INT_DEFINEAUTHIDENTIFIER]] = solveSetAID;
      mcb[SERVICES_UMS[INT_DEFINEWORKIDENTIFIER]] = solveSetWID;
      mcb[SERVICES_UMS[EXPORT]] = solveExport;
  }
  // TMS services
  if (mhasTMS) {
      functionPtr = solveSubmitJob;
      mcb[std::string(SERVICES_TMS[JOBSUBMIT])+"@"+mid] = functionPtr;
      functionPtr = solveCancelJob;
      mcb[std::string(SERVICES_TMS[JOBCANCEL])+"@"+mid] = functionPtr;
      functionPtr = solveJobInfo;
      mcb[std::string(SERVICES_TMS[JOBINFO])+"@"+mid] = functionPtr;
      functionPtr = solveGetListOfJobsProgression;
      mcb[std::string(SERVICES_TMS[GETJOBSPROGRESSION])+"@"+mid] = functionPtr;
      functionPtr = solveListOfQueues;
      mcb[std::string(SERVICES_TMS[GETLISTOFQUEUES])+"@"+mid] = functionPtr;
      functionPtr = solveJobOutPutGetResult;
      mcb[std::string(SERVICES_TMS[JOBOUTPUTGETRESULT])+"@"+mid] = functionPtr;
      functionPtr = solveJobOutPutGetCompletedJobs;
      mcb[std::string(SERVICES_TMS[JOBOUTPUTGETCOMPLETEDJOBS])+"@"+mid] = functionPtr;
      // Remove ?
      functionPtr = solveGetListOfJobs;
      mcb[SERVICES_TMS[GETLISTOFJOBS_ALL]] = functionPtr;
      functionPtr = solveAddWork;
      mcb[SERVICES_TMS[ADDWORK]] = functionPtr;
  }

  if (mhasFMS){
    functionPtr = solveTransferFile<File::copy,File::async>;
    mcb[SERVICES_FMS[FILECOPYASYNC]] = functionPtr;
    functionPtr = solveTransferFile<File::move,File::async>;
    mcb[SERVICES_FMS[FILEMOVEASYNC]] = functionPtr;
    functionPtr = solveTransferFile<File::move,File::sync>;
    mcb[SERVICES_FMS[FILEMOVE]] = functionPtr;
    functionPtr = solveTransferFile<File::copy,File::sync>;
    mcb[SERVICES_FMS[FILECOPY]] = functionPtr;
    functionPtr = solveGetInfos;
    mcb[SERVICES_FMS[FILEGETINFOS]] = functionPtr;
    functionPtr = solveChangeGroup;
    mcb[SERVICES_FMS[FILECHANGEGROUP]] = functionPtr;
    functionPtr = solveChangeMode;
    mcb[SERVICES_FMS[FILECHANGEMODE]] = functionPtr;
    functionPtr = solveHeadFile;
    mcb[SERVICES_FMS[FILEHEAD]] = functionPtr;
    functionPtr = solveGetFileContent;
    mcb[SERVICES_FMS[FILECONTENT]] = functionPtr;
    functionPtr = solveCreateFile;
    mcb[SERVICES_FMS[FILECREATE]] = functionPtr;
    functionPtr = solveCreateDir;
    mcb[SERVICES_FMS[DIRCREATE]] = functionPtr;
    functionPtr = solveRemoveFile;
    mcb[SERVICES_FMS[FILEREMOVE]] = functionPtr;
    functionPtr = solveRemoveDir;
    mcb[SERVICES_FMS[DIRREMOVE]] = functionPtr;
    functionPtr = solveTailFile;
    mcb[SERVICES_FMS[FILETAIL]] = functionPtr;
    functionPtr = solveListDir;
    mcb[SERVICES_FMS[DIRLIST]] = functionPtr;
    functionPtr = solveTransferRemoteFile<File::copy,File::async>;
    mcb[SERVICES_FMS[REMOTEFILECOPYASYNC]] = functionPtr;
    functionPtr = solveTransferRemoteFile<File::move,File::async>;
    mcb[SERVICES_FMS[REMOTEFILEMOVEASYNC]] = functionPtr;
    functionPtr = solveTransferRemoteFile<File::copy,File::sync>;
    mcb[SERVICES_FMS[REMOTEFILECOPY]] = functionPtr;
    functionPtr = solveTransferRemoteFile<File::move,File::sync>;
    mcb[SERVICES_FMS[REMOTEFILEMOVE]] = functionPtr;
    functionPtr = solveGetListOfFileTransfers;
    mcb[SERVICES_FMS[FILETRANSFERSLIST]] = functionPtr;
    functionPtr = solveFileTransferStop;
    mcb[SERVICES_FMS[FILETRANSFERSTOP]] = functionPtr;
    functionPtr = solveUpdateClientSideTransfer;
    mcb[SERVICES_FMS[UPDATECLIENTSIDETRANSFER]] = functionPtr;
  }

}

void
ServerXMS::getConfigOptions(const char* configPath,
                            std::vector<std::string>& defaultOptions,
                            const char* batchKey) {
  try {
    std::string scriptContent = vishnu::get_file_content(configPath);
    std::istringstream iss(scriptContent);
    std::string line;
    std::string value;
    std::string key;
    while (!iss.eof()) {
      getline(iss, line);
      size_t pos = line.find('#');
      if (pos == std::string::npos) {
        continue;
      }

      line = line.erase(0, pos);
      if (boost::algorithm::starts_with(line, batchKey)) {
        line = line.substr(strlen(batchKey));
        boost::algorithm::trim_left(line);
        pos = line.find(" ");
        if(pos != std::string::npos) {
          key = line.substr(0,pos);
          boost::algorithm::trim(key);
          defaultOptions.push_back(key);
          line = line.substr(pos);
          boost::algorithm::trim(line);
          while((pos = line.find(",")) != std::string::npos) {
            value = line.substr(0,pos-1);
            defaultOptions.push_back(value);
            defaultOptions.push_back(key);
            line = line.erase(0,pos);
          }
          value = line;
          defaultOptions.push_back(value);
        }
      }
    }
  } catch (...) {}
}
