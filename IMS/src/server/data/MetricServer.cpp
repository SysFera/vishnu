#include "MetricServer.hpp"
#include "IMSVishnuException.hpp"
#include "DbFactory.hpp"
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sigar.h>
#include "QueryServer.hpp"

using namespace vishnu;

MetricServer::MetricServer(const UserServer session, std::string mail)
  : msession(session), msendmail(mail) {
  IMS_Data::IMS_DataFactory_ptr ecoreFactory =
    IMS_Data::IMS_DataFactory::_instance();
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mvishnuId = 1;
  mfreq = 0;
  mcop = ecoreFactory->createCurMetricOp();
  mhop = ecoreFactory->createMetricHistOp();
  sigar_open(&sigar);
}

MetricServer::MetricServer(const UserServer session,
                           IMS_Data::MetricHistOp_ptr op,
                           std::string mail)
  : msession(session), msendmail(mail) {
  IMS_Data::IMS_DataFactory_ptr ecoreFactory =
    IMS_Data::IMS_DataFactory::_instance();
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mhop = op;
  mvishnuId = 1;
  mfreq = 0;
  mcop = ecoreFactory->createCurMetricOp();
  sigar_open(&sigar);
}

MetricServer::MetricServer(const UserServer session,
                           IMS_Data::CurMetricOp_ptr op,
                           std::string mail)
  : msession(session), msendmail(mail) {
  IMS_Data::IMS_DataFactory_ptr ecoreFactory =
    IMS_Data::IMS_DataFactory::_instance();
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mcop = op;
  mvishnuId = 1;
  mfreq = 0;
  mhop = ecoreFactory->createMetricHistOp();
  sigar_open(&sigar);
}

MetricServer::~MetricServer() {
  sigar_close(sigar);
}

void
MetricServer::setUpFreq(int freq) {
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "set update frequency is an admin function. A user cannot call it");
  }

  if (freq < 1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Error invalid frequency.");
  }

  std::string request =
  "update vishnu set updatefreq ='" + convertToString(freq) + "' where  vishnuid ='";
  request += convertToString(mvishnuId);
  request += "'";
  try {
    mdatabase->process(request.c_str());
  } catch(SystemException& e) {
    e.appendMsgComp("Failed to set frequency to " + convertToString(freq));
    throw(e);
  }
}


void
MetricServer::addMetricSet(IMS_Data::ListMetric* set, std::string mid){
  std::string nmid;
  double cpu;
  double mem;
  double disk;

  // The thresholds
  IMS_Data::Threshold cpu_thre;
  IMS_Data::Threshold mem_thre;
  IMS_Data::Threshold disk_thre;

  UMS_Data::User cpu_user;
  UMS_Data::User mem_user;
  UMS_Data::User disk_user;

  std::vector<std::string> results;
  std::vector<std::string>::iterator iter;

  // Getting the num machine id to insert
  string reqThre = "SELECT typet, value, userid, email from threshold, users, machine where machine.machineid='" + mid + "' and threshold.machine_nummachineid=machine.nummachineid and users.numuserid=threshold.users_numuserid";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqThre.c_str()));
    for (size_t i = 0; i < result->getNbTuples(); i++){
      results.clear();
      results = result->get(i);
      iter = results.begin();
      switch (convertToInt(*(iter))) {
      case 1://cpu
	cpu_thre.setType(convertToInt(*(iter)));
        cpu_thre.setValue(convertToInt(*(iter+1)));
        cpu_user.setUserId(*(iter+2));
        cpu_user.setEmail(*(iter+3));
	break;
      case 2://disk
	disk_thre.setType(convertToInt(*(iter)));
        disk_thre.setValue(convertToInt(*(iter+1)));
        disk_user.setUserId(*(iter+2));
        disk_user.setEmail(*(iter+3));
	break;
      case 3://memory
	mem_thre.setType(convertToInt(*(iter)));
        mem_thre.setValue(convertToInt(*(iter+1)));
        mem_user.setUserId(*(iter+2));
        mem_user.setEmail(*(iter+3));
	break;
      default :
	break;
      }
    }
  } catch (SystemException &e) {
    throw (e);
  }


  // Getting the num machine id to insert
  string reqnmid = "SELECT nummachineid from machine where  machineid ='" + mid + "'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqnmid.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown machine id");
  }
  // numerical index always in position 0 in tables
  nmid = result->get(0).at(0);
  // Filling values (If various in list, the latest is kept
  for (unsigned int i = 0 ; i < set->getMetric().size() ; i++){
    switch (set->getMetric().get(i)->getType()) {
    case 1 :  // cpu
      cpu = set->getMetric().get(i)->getValue();
      if (static_cast<int>(cpu)<cpu_thre.getValue()) {
	try {
	  sendMail(static_cast<int>(cpu), cpu_thre.getValue(), 1,
                   cpu_user.getEmail(), cpu_user.getUserId(), mid);
	} catch (SystemException& e) {
	  std::cerr << e.what() << "\n";
	}
      }
      break;
    case 2 :  // disk
      disk = set->getMetric().get(i)->getValue();
      if (static_cast<int>(disk)<disk_thre.getValue()) {
	try {
	  sendMail(static_cast<int>(disk), disk_thre.getValue(), 2,
                   disk_user.getEmail(), disk_user.getUserId(), mid);
	} catch (SystemException& e) {
	  std::cerr << e.what() << "\n";
	}
      }
      break;
    case 3:  // mem
      mem = set->getMetric().get(i)->getValue();
      if (static_cast<int>(mem)<mem_thre.getValue()) {
	try {
	  sendMail(static_cast<int>(mem), mem_thre.getValue(), 3,
                   mem_user.getEmail(), mem_user.getUserId(), mid);
	} catch (SystemException& e) {
	  std::cerr << e.what() << "\n";
	}
      }
      break;
    default:
      throw SystemException (ERRCODE_SYSTEM, "Unknown metric type");
      break;
    }
  }

  // Inserting the value
  std::string req = "insert into state(machine_nummachineid, memory, diskspace, cpuload, time) values('"+nmid+"', '"+convertToString(static_cast<int>(mem))+"', '"+convertToString(static_cast<int>(disk))+"', '"+convertToString(static_cast<int>(cpu))+"', CURRENT_TIMESTAMP) ";

  try{
    mdatabase->process(req.c_str());
  }catch(SystemException& e){
    e.appendMsgComp(" Error inserting new system states ");
    throw(e);
  }
}

unsigned int
MetricServer::checkUpFreq(){
  // Get the corresponding frequency
  string request = "select updatefreq from vishnu where vishnuid='";
  request += convertToString(mvishnuId);
  request += "'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(request.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVVISHNU, "Unknown VISHNU id");
  }
  std::vector<std::string> res;
  res = result->get(0);
  // Updating the frequency value
  mfreq = convertToInt(res.at(0));
  // Returning the new frequency value
  return mfreq;
}


IMS_Data::ListMetric*
MetricServer::getCurMet(){
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject = ecoreFactory->createListMetric();
  IMS_Data::Metric_ptr met;
  double disk = 0.0;
  double cpu  = 0.0;
  double mem  = 0.0;
  ptime p =  second_clock::local_time();

  sigar_file_system_usage_t fsu;
  sigar_file_system_usage_get(sigar, "/", &fsu);
  disk = fsu.avail / (1024.0 * 1024.0);
  met = ecoreFactory->createMetric();
  met->setType(2);
  met->setValue(static_cast<int>(disk));
  met->setTime(string_to_time_t(boost::posix_time::to_simple_string(p)));
  if (mcop->getMetricType() != 1 && mcop->getMetricType() != 3) {
    mlistObject->getMetric().push_back(met);
  }

  sigar_mem_t memory;
  sigar_mem_get(sigar, &memory);
  mem = memory.free / (1024.0 * 1024.0);
  met = ecoreFactory->createMetric();
  met->setType(3);
  met->setValue(static_cast<int>(mem));
  met->setTime(string_to_time_t(boost::posix_time::to_simple_string(p)));
  if (mcop->getMetricType() != 1 && mcop->getMetricType() != 2) {
    mlistObject->getMetric().push_back(met);
  }


  sigar_cpu_info_list_t cpuinfo_list;
  sigar_loadavg_t loadavg;
  sigar_cpu_info_list_get(sigar, &cpuinfo_list);
  sigar_loadavg_get(sigar, &loadavg);
  // Set in percentage
  cpu = 100 * (1 - loadavg.loadavg[0] / cpuinfo_list.number);
  met = ecoreFactory->createMetric();
  met->setType(1);
  met->setValue(static_cast<int>(cpu));
  met->setTime(string_to_time_t(boost::posix_time::to_simple_string(p)));
  if (mcop->getMetricType() != 3 && mcop->getMetricType() != 2) {
    mlistObject->getMetric().push_back(met);
  }

  return mlistObject;
}

IMS_Data::ListMetric*
MetricServer::getHistMet(string machineId){
  std::string request = "select cpuload, state.diskspace, state.memory, time from state, machine where machine.machineid='"+machineId+"' AND machine.nummachineid=state.machine_nummachineid ";
  vector<string>::iterator iter;
  vector<string> results = vector<string>();

  IMS_Data::MetricType type = mhop->getType();

  std::string reqnmid = "SELECT nummachineid from machine where  machineid='"+machineId+ "'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqnmid.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown machine id");
  }

  if (mhop->getStartTime()>0) {
    time_t start = static_cast<time_t>(mhop->getStartTime());
    if (start != -1) {
      start = convertUTCtimeINLocaltime(start);
      std::string startStr =
        boost::posix_time::to_simple_string(boost::posix_time::from_time_t(start));
      std::ostringstream osValue;
      osValue << startStr;
      request.append(" and time >= ");
      request.append("'" + osValue.str() + "'");
    }
  }
  if (mhop->getEndTime()>0) {
    time_t end = static_cast<time_t>(mhop->getEndTime());
    if (end != -1) {
      end = convertUTCtimeINLocaltime(end);
      std::string endStr = boost::posix_time::to_simple_string(
        boost::posix_time::from_time_t(end));
      std::ostringstream osValue;
      osValue << endStr;
      request.append(" and time <= ");
      request.append("'" + osValue.str() + "'");
    }
  }
  // To avoid having too much result if no date is given
  if (mhop->getEndTime()<=0 && mhop->getStartTime()<=0) {
    request.append(" order by time desc limit 1000");
  } else {
    request.append(" order by time asc");
  }

  IMS_Data::IMS_DataFactory_ptr ecoreFactory =
    IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject =
    ecoreFactory->createListMetric();
  try {
    boost::scoped_ptr<DatabaseResult> listOfMetric (mdatabase->getResult(request.c_str()));
    // Do not factorize the code to avoid repeating the 'if' tests in the for
    switch (type) {
    case 1 : // CASE CPU USE
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(1);
        met->setValue(convertToInt(*(iter)));
        met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+3))));
	mlistObject->getMetric().push_back(met);
      }
      break;
    case 2: // CASE FREE DISKSPACE
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(2);
        met->setValue(convertToInt(*(iter+1)));
        met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+3))));
	mlistObject->getMetric().push_back(met);
      }
      break;
    case 3: // CASE FREE MEMORY
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(3);
	met->setValue(convertToInt(*(iter+2)));
        met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+3))));
	mlistObject->getMetric().push_back(met);
      }
      break;
    default :
      for (size_t i = 0; i < listOfMetric->getNbTuples(); i++){
	results.clear();
	results = listOfMetric->get(i);
	iter = results.begin();
	IMS_Data::Metric_ptr met = ecoreFactory->createMetric();
	met->setType(3);
	met->setValue(convertToInt(*(iter+2)));
        met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+3))));
	mlistObject->getMetric().push_back(met);
	met = ecoreFactory->createMetric();
	met->setType(2);
        met->setValue(convertToInt(*(iter+1)));
        met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+3))));
	mlistObject->getMetric().push_back(met);
	met = ecoreFactory->createMetric();
	met->setType(1);
        met->setValue(convertToInt(*(iter)));
        met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+3))));
	mlistObject->getMetric().push_back(met);
      }
      break;
    }
  } catch (SystemException& e) {
    throw (e);
  }
  return mlistObject;
}


void
MetricServer::sendMail(int val, int threshold, int type,
                       std::string email, std::string uid,
                       std::string machine){
  std::ostringstream command;
  std::vector<std::string> tokens;
  std::string stype;
  int pid;

  // If no email script given, to not try to send, because in some occasions there may be a threshold reached but the object does not know about the script (a user call throught the api)
  if (msendmail.empty()) {
    return;
  }
  // Convert type from int to string for the email
  switch (type) {
  case 1:
    stype = "cpuload";
    break;
  case 2:
    stype = "free diskspace";
    break;
  case 3:
    stype = "free memory";
    break;
  default:
    throw SystemException(ERRCODE_SYSTEM, "Error during the creation of the process for sending mail to the user with the userId:" +uid);
  }
  std::string subject = "[VISHNU] Threshold reached";
  std::string content = "WARNING: The threshold of type " + stype +
    " and with the value of " + convertToString(threshold) +
    " has been reached with the value " + convertToString(val) +
    " on the machine " + machine;
  // To build the script command
  command << msendmail << " --to " << email << " -s ";

  std::istringstream is(command.str());
  std::copy(std::istream_iterator<std::string>(is),
  std::istream_iterator<std::string>(),
  std::back_inserter<std::vector<std::string> >(tokens));

  char* argv[tokens.size()+6];
  argv[tokens.size()+5]=NULL;
  //Use of tokens
  for (unsigned int i = 0; i < tokens.size(); ++i) {
    argv[i]=strdup(tokens[i].c_str());
  }
  //To avoid mutiple values by using tokens because of spaces
  argv[tokens.size()]=strdup(subject.c_str());
  argv[tokens.size()+1]=strdup(content.c_str());
  //To execute the script on background
  argv[tokens.size()+2]=strdup(std::string(" 1>/dev/null ").c_str());
  argv[tokens.size()+3]=strdup(std::string(" 2>/dev/null ").c_str());
  argv[tokens.size()+4]=strdup(std::string(" & ").c_str());

  pid = fork();
  if (pid == -1) {//if an error occurs during fork
    for (unsigned int i=0; i<tokens.size()+5; ++i) {
      free(argv[i]);
    }
    throw SystemException(ERRCODE_SYSTEM, "Error during the creation of the process for sending mail to the user with the userId:" +uid);
  }

  if (pid == 0) {//if the child process
    freopen("dev/null", "r", stdin);
    freopen("dev/null", "w", stdout);
    freopen("dev/null", "w", stderr);

    if (execv(argv[0], argv) == -1) {
      for (unsigned int i=0; i<tokens.size()+5; ++i) {
      free(argv[i]);
      }
      exit(1);
    }
  }
  for (unsigned int i=0; i<tokens.size()+5; ++i) {
    free(argv[i]);
  }
}
