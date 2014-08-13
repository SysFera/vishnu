#include "MonitorXMS.hpp"
#include <csignal>
#include "AuthenticatorConfiguration.hpp"
#include "AuthenticatorFactory.hpp"
#include "Authenticator.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"
#include "ServerXMS.hpp"
#include "Logger.hpp"


MonitorXMS::MonitorXMS(int interval) :
  minterval(interval),
  mdatabaseVishnu(NULL),
  mauthenticator(NULL) {}

MonitorXMS::~MonitorXMS() {
  delete mdatabaseVishnu;
}

void
MonitorXMS::init(const SedConfig& cfg) {
  using boost::format;
  using boost::str;

  DbConfiguration dbConfig = cfg.dbConfig;
  AuthenticatorConfiguration authenticatorConfig = cfg.authenticatorConfig;
  mhasUMS = cfg.hasUMS;
  mhasTMS = cfg.hasTMS;
  mhasFMS = cfg.hasFMS;


  DbFactory factory;
  mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
  mmachineId = cfg.mid;
  mbatchType = cfg.batchType;
  mbatchVersion = cfg.batchVersion;

  AuthenticatorFactory authfactory;
  mauthenticator = authfactory.createAuthenticatorInstance(authenticatorConfig);
  try {
    /*initialized the connection to the database*/
    mdatabaseVishnu->connect();
  } catch (VishnuException& e) {
    exit(0);
  }
}

void
MonitorXMS::checkJobs(int batchtype){
  std::string query = boost::str(boost::format(
                                   "SELECT job.id, batchJobId, vmIp, vmId, account.aclogin "
                                   " FROM job, vsession, machine, account "
                                   " WHERE batchType=%1% "
                                   " AND job.status >= %2% "
                                   " AND job.status < %3% "
                                   " AND machine.machineid='%4%' "
                                   " AND vsession.numsessionid=job.vsession_numsessionid "
                                   " AND job.machine_nummachineid=machine.nummachineid "
                                   " AND job.users_numuserid=account.users_numuserid;")
                                 % vishnu::convertToString(batchtype)
                                 % vishnu::STATE_UNDEFINED
                                 % vishnu::STATE_COMPLETED
                                 % mmachineId);

  try {
    BatchFactory factory;
    boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(batchtype, mbatchVersion));
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(query.c_str()));

    std::vector<std::string> buffer;
    std::vector<std::string>::iterator item;
    for (size_t i = 0; i < result->getNbTuples(); ++i) {
      buffer.clear();
      buffer = result->get(i);
      item = buffer.begin();
      TMS_Data::Job job;
      job.setId( *item++ );
      job.setBatchJobId( *item++ );
      job.setVmIp( *item++ );
      job.setVmId( *item++ );
      job.setLocalAccount( "root"); // job.setLocalAccount( *item );

      try {
        int state;
        switch (batchtype) {
          case DELTACLOUD:
          case OPENNEBULA:
            state = batchServer->getJobState(JsonObject::serialize(job));
            break;
          default:
            state = batchServer->getJobState(job.getBatchJobId());
            break;
        }
        std::string query = "";

        if (state == vishnu::STATE_COMPLETED) {
          query.append(boost::str(
                         boost::format("UPDATE job"
                                       " SET endDate=CURRENT_TIMESTAMP "
                                       " WHERE job.id='%1%';")
                         % job.getId()));
        }


        query = boost::str(boost::format("UPDATE job SET status=%1% WHERE id='%2%';")
                           % vishnu::convertToString(state)
                           % job.getId());

        if (state == vishnu::STATE_COMPLETED) {
          query.append( boost::str(
                          boost::format("UPDATE job"
                                        " SET endDate=CURRENT_TIMESTAMP"
                                        " WHERE job.id='%1%';")
                          % job.getId()) );
        } else {

          if (batchtype == OPENNEBULA) {
            std::string executionOutput = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_SCRIPT_SUBMISSION_OUTPUT], true, "");
            if (job.getBatchJobId().empty()) {
              switch (state) {
                case vishnu::STATE_RUNNING:
                  query.append( boost::str(boost::format("UPDATE job SET batchJobId='%1%' WHERE job.id='%2%';")
                                           % executionOutput
                                           % job.getId()) );
                  break;
                case vishnu::STATE_FAILED:
                case vishnu::STATE_CANCELLED:
                  query.append( boost::str(boost::format("UPDATE job SET jobdescription='%1%' WHERE job.id='%2%';")
                                           % executionOutput
                                           % job.getId()) );
                  break;
                default:
                  break;
              }
            }
          } // if opennebula

        }


        mdatabaseVishnu->process(query);
      } catch (VishnuException& ex) {
        LOG(boost::str(boost::format("[TMSMONITOR][ERROR] %1%") % ex.what()), LogErr);
      }
    }
  } catch (VishnuException& ex) {
    LOG(boost::str(boost::format("[TMSMONITOR][ERROR] %1%") % ex.what()), LogErr);
  } catch (...) {
    LOG("[TMSMONITOR][ERROR] Unknow error", LogErr);
  }
}


void
MonitorXMS::checkSession(){
  SessionServer closer;
  std::vector<std::string> tmp;
  std::vector<std::string>::iterator ii;
  try {
    boost::scoped_ptr<DatabaseResult> result(closer.getSessionToclosebyTimeout());

    if (result->getNbTuples() != 0) {
      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        tmp.clear();
        tmp = result->get(i);

        ii = tmp.begin();
        SessionServer sessionServer (*ii);

        try {
          //closure of the session
          sessionServer.close();
        }
        catch (VishnuException& e) {
          std::string errorInfo =  e.buildExceptionString();
          std::cerr << errorInfo << "\n";
          if (e.getMsgI() == ERRCODE_AUTHENTERR) {
            //FIXME: Do something for the sed UMS when the monitor goes out
            std::cerr << errorInfo << "\n";
            exit(1);
          }
        }
      }
    }

  } catch (VishnuException& e) {
    std::string errorInfo =  e.buildExceptionString();

    if ((e.getMsgI() == ERRCODE_DBERR) || (e.getMsgI() == ERRCODE_AUTHENTERR)) {
      std::cerr << errorInfo << "\n";
      exit(1);
    }
  }
}

void
MonitorXMS::checkFile(){

  std::string sqlRequest = "SELECT numfiletransferid, processid "
                           " FROM filetransfer,vsession"
                           " WHERE vsession.numsessionid=filetransfer.vsession_numsessionid "
                           " AND filetransfer.status=0";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlRequest));
    if (result->getNbTuples() != 0) {
      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        std::vector<std::string> dbResultEntry = result->get(i);
        std::vector<std::string>::iterator dbResultEntryIter = dbResultEntry.begin();
        std::string transferId=*dbResultEntryIter;
        ++dbResultEntryIter;
        std::string pid = *dbResultEntryIter;
        ++dbResultEntryIter;
        try {
          if(false==vishnu::process_exists(vishnu::convertToString(pid))) {
            std::string query = "UPDATE filetransfer SET status=3"
                                " WHERE numfiletransferid='"+transferId+"'";
            mdatabaseVishnu->process(query);
          }
        } catch (VishnuException& ex) {
          std::clog << boost::format("[FMSMONITOR][ERROR] %1%\n")%ex.what();
        }
      }
    }
  } catch (VishnuException& ex) {
    std::clog << boost::format("[FMSMONITOR][ERROR] %1%\n")%ex.what();
  }
}



int
MonitorXMS::run() {
  while (kill(getppid(), 0) == 0) {
    if (mhasUMS) {
      checkSession();
    }
    if (mhasTMS) {
      checkJobs(mbatchType);
      if (mbatchType != POSIX){
        checkJobs(POSIX);
      }
    }
    if (mhasFMS) {
      checkFile();
    }
    sleep(minterval);
  }
  return 0;
}
