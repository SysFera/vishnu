#include "MonitorXMS.hpp"
#include <csignal>
#include "AuthenticatorConfiguration.hpp"
#include "AuthenticatorFactory.hpp"
#include "Authenticator.hpp"
#include "BatchServer.hpp"
#include "BatchFactory.hpp"



MonitorXMS::MonitorXMS(int interval) :
  minterval(interval),
  mdatabaseVishnu(NULL),
  mauthenticator(NULL) {}

MonitorXMS::~MonitorXMS() {
  delete mdatabaseVishnu;
}

void
MonitorXMS::init(int vishnuId, DbConfiguration dbConfig,
                 AuthenticatorConfiguration authenticatorConfig, const std::string& machineId,
                 const BatchType& batchType, const std::string& batchVersion) {
  using boost::format;
  using boost::str;

  DbFactory factory;
  mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
  mmachineId = machineId;
  mbatchType = batchType;
  mbatchVersion = batchVersion;

  AuthenticatorFactory authfactory;
  mauthenticator = authfactory.createAuthenticatorInstance(authenticatorConfig);

  std::string sqlCommand = 
   str(format("SELECT * FROM vishnu where vishnuid=%1%")
     % vishnu::convertToString(vishnuId));

  try {
    /*connection to the database*/
    mdatabaseVishnu->connect();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      throw SystemException(ERRCODE_DBERR, "The vishnuid is unrecognized");
    }
  } catch (VishnuException& e) {
    exit(0);
  }
}


int
MonitorXMS::run() {
  std::vector<std::string>::iterator ii;
  std::vector<std::string> tmp;
  SessionServer closer;
  
  int state;
  std::string query;
  std::string sqlRequest = boost::str(boost::format(
    "SELECT jobId, batchJobId, vmIp, vmId "
    " FROM job, vsession "
    " WHERE vsession.numsessionid=job.vsession_numsessionid "
    " AND submitMachineId='%1%+' "
    " AND batchType=%2% "
    " AND status > %3% "
    " AND status < %4% ")
      % mmachineId
      % vishnu::convertToString(mbatchType)
      % vishnu::STATE_UNDEFINED
      % vishnu::STATE_COMPLETED);

  std::string vmUser = "root";
  if (mbatchType == DELTACLOUD) {
    vmUser = vishnu::getVar(vishnu::CLOUD_ENV_VARS[vishnu::CLOUD_VM_USER], true, "root");
  }

  BatchFactory factory;
  boost::scoped_ptr<BatchServer> batchServer(factory.getBatchServerInstance(mbatchType, mbatchVersion));


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
    sleep(minterval);

  } catch (VishnuException& e) {
    std::string errorInfo =  e.buildExceptionString();

    if ((e.getMsgI() == ERRCODE_DBERR) || (e.getMsgI() == ERRCODE_AUTHENTERR)) {
      std::cerr << errorInfo << "\n";
      exit(1);
    }
    sleep(minterval);
  }

  while (kill(getppid(), 0) == 0) {
    try {
      boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlRequest.c_str()));
      if (result->getNbTuples() == 0) {
        sleep(minterval);
        continue;
      }

      std::vector<std::string> buffer;
      std::vector<std::string>::iterator item;

      for (size_t i = 0; i < result->getNbTuples(); ++i) {
        buffer.clear();
        buffer = result->get(i);
        item = buffer.begin();
        TMS_Data::Job job;
        job.setJobId( *item++ );
        job.setBatchJobId( *item++ );
        job.setVmIp( *item++ );
        job.setVmId( *item++ );
        job.setOwner( *item );

        try {
          int state;
          switch (mbatchType) {
          case DELTACLOUD:
          case OPENNEBULA:
            state = batchServer->getJobState( JsonObject::serialize(job) );
            break;
          default:
            state = batchServer->getJobState(job.getBatchJobId());
            break;
          }
          std::string query = boost::str(boost::format("UPDATE job SET status=%1%"
                                                       " WHERE jobId='%2%';")
                                         % vishnu::convertToString(state) % job.getJobId());
          if (state == vishnu::STATE_COMPLETED) {
            query.append(boost::str(boost::format("UPDATE job SET endDate=CURRENT_TIMESTAMP"
                                                  " WHERE jobId='%1%';") % job.getJobId()));
          }
          mdatabaseVishnu->process(query.c_str());
        } catch (VishnuException& ex) {
          std::clog << boost::format("[TMSMONITOR][ERROR] %1%\n")%ex.what();
        }
      }
    } catch (VishnuException& ex) {
      std::clog << boost::format("[TMSMONITOR][ERROR] %1%\n")%ex.what();
    }
    sleep(minterval);
  }
  return 0;
}
