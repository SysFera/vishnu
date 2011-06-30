#include "MetricServer.hpp"
#include "IMSVishnuException.hpp"
#include "DbFactory.hpp"
#include <vector>
#include "DIET_data.h"
#include "DIET_server.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "QueryServer.hpp"

using namespace vishnu;

MetricServer::MetricServer(const UserServer session, string mail):msession(session), msendmail(mail) {
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mvishnuId = 1;
  mfreq = 0;
  mcop = ecoreFactory->createCurMetricOp();
  mhop = ecoreFactory->createMetricHistOp();
}
MetricServer::MetricServer(const UserServer session, IMS_Data::MetricHistOp_ptr op, string mail):msession(session), msendmail(mail) {
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mhop = op;
  mvishnuId = 1;
  mfreq = 0;
  mcop = ecoreFactory->createCurMetricOp();
}
MetricServer::MetricServer(const UserServer session, IMS_Data::CurMetricOp_ptr op, string mail):msession(session), msendmail(mail) {
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mcop = op;
  mvishnuId = 1;
  mfreq = 0;
  mhop = ecoreFactory->createMetricHistOp();
}
MetricServer::~MetricServer(){
}

void
MetricServer::setUpFreq(unsigned int freq){
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "set update frequency is an admin function. A user cannot call it");
  }

  if (freq < 1) {
    throw UserException(ERRCODE_INVALID_PARAM, "Error invalid frequency.");
  }

  string request = "update vishnu set updatefreq ='"+convertToString(freq)+"' where  vishnuid ='";
  request += convertToString(mvishnuId);
  request += "'";
  try {
    mdatabase->process(request.c_str());
  }catch(SystemException& e) {
    e.appendMsgComp("Failed to set frequency to "+convertToString(freq));
    throw(e);
  }
}


void
MetricServer::addMetricSet(IMS_Data::ListMetric* set, string mid){
  string nmid;
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

  vector<string> results = vector<string>();
  vector<string>::iterator iter;

  // Getting the num machine id to insert
  string reqThre = "SELECT * from threshold, users, machine where machine.machineid='"+mid+"' and threshold.machine_nummachineid=machine.nummachineid and users.numuserid=threshold.users_numuserid";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqThre.c_str()));
    for (size_t i = 0; i < result->getNbTuples(); i++){
      results.clear();
      results = result->get(i);
      iter = results.begin();
      switch (convertToInt(*(iter+3))) {
      case 1://cpu
	cpu_thre.setType(convertToInt(*(iter+3)));
	cpu_thre.setValue(convertToInt(*(iter+4)));
	cpu_user.setUserId(*(iter+7));
	cpu_user.setEmail(*(iter+12));
	break;
      case 2://disk
	disk_thre.setType(convertToInt(*(iter+3)));
	disk_thre.setValue(convertToInt(*(iter+4)));
	disk_user.setUserId(*(iter+7));
	disk_user.setEmail(*(iter+12));
	break;
      case 3://memory
	mem_thre.setType(convertToInt(*(iter+3)));
	mem_thre.setValue(convertToInt(*(iter+4)));
	mem_user.setUserId(*(iter+7));
	mem_user.setEmail(*(iter+12));
	break;
      }	
    }
  } catch (SystemException &e) {
    throw (e);
  }
  

  // Getting the num machine id to insert
  string reqnmid = "SELECT * from machine where  machineid ='"+mid+"'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqnmid.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown machine id");
  }
  // numerical index always in position 0 in tables
  nmid = result->get(0).at(0);
  // Filling values (If various in list, the latest is kept
  for (unsigned int i = 0 ; i < set->getMetric().size() ; i++){
    switch (set->getMetric().get(i)->getType()) {
    case 1 : //cpu
      cpu = set->getMetric().get(i)->getValue();
      if (static_cast<int>(cpu)<cpu_thre.getValue()) {
	try {
	  sendMail(static_cast<int>(cpu), cpu_thre.getValue(), 1, cpu_user.getEmail(), cpu_user.getUserId(), mid);
	} catch (SystemException& e) {
	  cerr << e.what() << endl;
	}
      }
      break;
    case 2 : //disk
      disk = set->getMetric().get(i)->getValue();
      if (static_cast<int>(disk)<disk_thre.getValue()) {
	try {
	  sendMail(static_cast<int>(disk), disk_thre.getValue(), 2, disk_user.getEmail(), disk_user.getUserId(), mid);
	} catch (SystemException& e) {
	  cerr << e.what() << endl;
	}
      }
      break;
    case 3: //mem
      mem = set->getMetric().get(i)->getValue();
      if (static_cast<int>(mem)<mem_thre.getValue()) {
	try {
	  sendMail(static_cast<int>(mem), mem_thre.getValue(), 3, mem_user.getEmail(), mem_user.getUserId(), mid);
	} catch (SystemException& e) {
	  cerr << e.what() << endl;
	}
      }
      break;
    default:
      throw SystemException (ERRCODE_SYSTEM, "Unknown metric type");
      break;
    }
  }

  // Inserting the value
  string req = "insert into state(machine_nummachineid, memory, diskspace, cpuload, time) values('"+nmid+"', '"+convertToString(static_cast<int>(mem))+"', '"+convertToString(static_cast<int>(disk))+"', '"+convertToString(static_cast<int>(cpu))+"', CURRENT_TIMESTAMP) ";
  
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
  string request = "select * from vishnu where vishnuid='";
  request += convertToString(mvishnuId);
  request += "'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(request.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVVISHNU, "Unknown VISHNU id");
  }
  vector<string> res;
  res = result->get(0);
  // Updating the frequency value
  mfreq = convertToInt(res.at(FREQPOS));
  // Returning the new frequency value
  return mfreq;
}


IMS_Data::ListMetric*
MetricServer::getCurMet(){
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject = ecoreFactory->createListMetric();
  estVector_t vec = diet_new_estVect();
  IMS_Data::Metric_ptr met;
  double disk = 0.0;
  double cpu  = 0.0;
  double mem  = 0.0;
  ptime p =  second_clock::local_time();

  diet_estimate_cori_add_collector(EST_COLL_EASY,NULL);

  diet_estimate_cori (vec, EST_FREESIZEDISK, EST_COLL_EASY, "./");
  disk = diet_est_get_system(vec, EST_FREESIZEDISK, -1); 
  met = ecoreFactory->createMetric();
  met->setType(2);
  met->setValue(static_cast<int>(disk));
  met->setTime(string_to_time_t(boost::posix_time::to_simple_string(p)));
  if (mcop->getMetricType() != 1 && mcop->getMetricType() != 3) {
    mlistObject->getMetric().push_back(met);
  }

  diet_estimate_cori (vec, EST_FREEMEM, EST_COLL_EASY, NULL);
  mem = diet_est_get_system(vec, EST_FREEMEM, -1); 
  met = ecoreFactory->createMetric();
  met->setType(3);
  met->setValue(static_cast<int>(mem));
  met->setTime(string_to_time_t(boost::posix_time::to_simple_string(p)));
  if (mcop->getMetricType() != 1 && mcop->getMetricType() != 2) {
    mlistObject->getMetric().push_back(met);
  }

  diet_estimate_cori (vec, EST_FREECPU, EST_COLL_EASY, NULL);
  cpu = diet_est_get_system(vec, EST_FREECPU, -1); 
  cpu *= 100; // Set in percentage
  met = ecoreFactory->createMetric();
  met->setType(1);
  met->setValue(static_cast<int>(cpu));
  met->setTime(string_to_time_t(boost::posix_time::to_simple_string(p)));
  if (mcop->getMetricType() != 3 && mcop->getMetricType() != 2) {
    mlistObject->getMetric().push_back(met);
  }

  diet_destroy_estVect(vec) ;

  return mlistObject;
}

IMS_Data::ListMetric*
MetricServer::getHistMet(string machineId){
  string request = "select * from state, machine where machine.machineid='"+machineId+"' AND machine.nummachineid=state.machine_nummachineid ";
  vector<string>::iterator iter;
  vector<string> results = vector<string>();

  IMS_Data::MetricType type = mhop->getType();

  string reqnmid = "SELECT * from machine where  machineid='"+machineId+"'";
  boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(reqnmid.c_str()));
  if(result->getNbTuples() == 0) {
    throw IMSVishnuException(ERRCODE_INVPROCESS, "Unknown machine id");
  }

  if (mhop->getStartTime()>0) {
    time_t start = static_cast<time_t>(mhop->getStartTime());
    if (start != -1) {
      start = convertUTCtimeINLocaltime(start);
      string startStr = boost::posix_time::to_simple_string(boost::posix_time::from_time_t(start));
      std::ostringstream osValue;
      osValue << startStr;
      request.append(" and time >= ");
      request.append("'"+osValue.str()+"'");
    }
  }
  if (mhop->getEndTime()>0) {
    time_t end = static_cast<time_t>(mhop->getStartTime());
    if (end != -1) {
      end = convertUTCtimeINLocaltime(end);
      string endStr = boost::posix_time::to_simple_string(boost::posix_time::from_time_t(end));
      std::ostringstream osValue;
      osValue << endStr;
      request.append(" and time <= ");
      request.append("'"+osValue.str()+"'");
    }
  }
  // To avoid having too much result if no date is given
  if (mhop->getEndTime()<=0 && mhop->getStartTime()<=0) {
    request.append(" order by time desc limit 1000");
  } else {
    request.append(" order by time asc");
  }

  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::ListMetric_ptr mlistObject = ecoreFactory->createListMetric();
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
	met->setValue(convertToInt(*(iter+4)));
	met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+5))));
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
	met->setValue(convertToInt(*(iter+3)));
	met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+5))));
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
	met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+5))));
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
	met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+5))));
	mlistObject->getMetric().push_back(met);
	met = ecoreFactory->createMetric();
	met->setType(2);
	met->setValue(convertToInt(*(iter+3)));
	met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+5))));
	mlistObject->getMetric().push_back(met);
	met = ecoreFactory->createMetric();
	met->setType(1);
	met->setValue(convertToInt(*(iter+4)));
	met->setTime(convertLocaltimeINUTCtime(convertToTimeType(*(iter+5))));
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
MetricServer::sendMail(int val, int threshold, int type, string email, string uid, string machine){
  std::ostringstream command;
  std::vector<std::string> tokens;
  string stype;
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
  string subject = "[VISHNU] Threshold reached";
  string content = "WARNING: The threshold of type "+stype+" and with the value of "+convertToString(threshold)+" has been reached with the value "+convertToString(val)+" on the machine "+machine;
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
