/**
 * \file TMSMapper.ccp
 * \brief Implementation of the TMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "TMSMapper.hpp"
#include "TMS_Data.hpp"
#include "TMS_Data_forward.hpp"
#include "utilVishnu.hpp"
#include "TMSVishnuException.hpp"
#include "utilServer.hpp"

using namespace vishnu;

TMSMapper::TMSMapper(){
};

TMSMapper::~TMSMapper(){
};

TMSMapper::TMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(VISHNU_SUBMITJOB, "vishnu_submit_job"));
  mmap.insert (pair<int, string>(VISHNU_GETJOBINFO, "vishnu_get_job_info"));
  mmap.insert (pair<int, string>(VISHNU_GETJOBPROG, "vishnu_get_job_progress"));
  mmap.insert (pair<int, string>(VISHNU_LISTQUEUES, "vishnu_list_queues"));
  mmap.insert (pair<int, string>(VISHNU_LISTJOBS, "vishnu_list_jobs"));
  mmap.insert (pair<int, string>(VISHNU_GETJOBOUTPUT, "vishnu_get_job_output"));
  mmap.insert (pair<int, string>(VISHNU_GETCOMPLETEDJOB, "vishnu_get_completed_jobs_output"));
  mmap.insert (pair<int, string>(VISHNU_CANCEL, "vishnu_cancel_job"));
  mmap.insert (pair<int, string>(VISHNU_ADD_WORK, "vishnu_add_work"));
};

int
TMSMapper::registerMapper(){
	mreg->addMapper(mname, this);
	return 0;
}

int
TMSMapper::unregisterMapper(){
	return mreg->removeMapper(mname);
}

int
TMSMapper::getCommand(const int& key,string& command){
	map<int, string>::const_iterator it;
	for (it = mmap.begin() ; it != mmap.end() ; ++it){
		if (key==it->first){
			command = it->second;
			return 0;
		}
	}
	return 0;
}

int
TMSMapper::getKey(const string& command, int& key){
	map<int, string>::const_iterator it;
	for (it = mmap.begin() ; it != mmap.end() ; ++it){
		if (command.compare(it->second)==0){
			key = it->first;
			return 0;
		}
	}
	return 0;
}


int
TMSMapper::code(const string& cmd, unsigned int code){
	map<int, string>::iterator it;
	int size;
	string key;
	int keycode;
	// If existing code -> add to the existing entry
	if(code){
		it = mcmd.find(code);
		if (it==mcmd.end()){
			throw new SystemException(ERRCODE_SYSTEM, "Error wrong code to build command: "+cmd);
		}
		it->second += "#";
		// If empty param value
		if (cmd.compare("")==0){
			it->second += " ";
		}else {
			it->second += cmd;
		}
		return 0;
	}

	// Else creating a new unique key and insert in the map
	pthread_mutex_lock(&mutex);
	size = mcmd.size() + 1;
	while (true){
		it = mcmd.find(size);
		if (it==mcmd.end()){
			break;
		}
		size++;
	}
	getKey(cmd, keycode);
	key = convertToString(keycode);
	mcmd.insert(pair<int, string>(size, key));
	pthread_mutex_unlock(&mutex);
	return size;
}

string
TMSMapper::decode (const string& msg){
  vector<int> separatorPos;
  string      func;
  int         funcCode;
  string res;

  // Getting separator position
  findSeparator(msg, separatorPos);

  // Getting function code
  if(!separatorPos.empty()){
    func = msg.substr(0, separatorPos.at(0));
  }else{
    func = msg;
  }

  // Convert code to int
  funcCode = convertToInt(func);

  switch(funcCode){
  case VISHNU_SUBMITJOB:
    res = decodeSubmit(separatorPos, msg);
    break;
  case VISHNU_GETJOBPROG:
    res = decodeProg(separatorPos, msg);
    break;
  case VISHNU_LISTQUEUES:
    res = decodeQueue(separatorPos, msg);
    break;
  case VISHNU_LISTJOBS:
    res = decodeListJob(separatorPos, msg);
    break;
  case VISHNU_GETJOBOUTPUT:
    res = decodeOutput(separatorPos, msg);
    break;
  case VISHNU_GETCOMPLETEDJOB:
    res = decodeCompletedJob(separatorPos, msg);
    break;
  case VISHNU_CANCEL:
    res = decodeCancel(separatorPos, msg);
    break;
  case VISHNU_GETJOBINFO:
    res = decodeJobInfo(separatorPos, msg);
    break;
  case VISHNU_ADD_WORK:
    res = decodeAddWork(separatorPos, msg);
    break;
  default:
    res = "";
    break;
  }
  return res;
}


string
TMSMapper::decodeSubmit(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	long l;
	res += (mmap.find(VISHNU_SUBMITJOB))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-2);
	TMS_Data::SubmitOptions_ptr ac = NULL;

	//To parse the object serialized
	if(!vishnu::parseEmfObject(u, ac)) {
		throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
	}

	u = ac->getName();
	if (u.compare("")){
		res += " -n ";
		res += u;
	}
	u = ac->getQueue();
	if (u.compare("")){
		res += " -q ";
		res += u;
	}
	l = ac->getWallTime();
	if (l>0){
		res += " -t ";
		res += convertWallTimeToString(l);
	}
	l = ac->getMemory();
	if (l>0){
		res += " -m ";
		res += convertToString(l);
	}
	l = ac->getNbCpu();
	if (l>0){
		res += " -P ";
		res += convertToString(l);
	}
	u = ac->getNbNodesAndCpuPerNode();
	if (u.compare("")){
		res += " -N ";
		res += u;
	}
	u = ac->getOutputPath();
	if (u.compare("")){
		res += " -o ";
		res += u;
	}
	u = ac->getErrorPath();
	if (u.compare("")){
		res += " -e ";
		res += u;
	}
	u = ac->getMailNotification();
	if (u.compare("")){
		res += " -M ";
		res += u;
	}
	u = ac->getMailNotifyUser();
	if (u.compare("")){
		res += " -u ";
		res += u;
	}
	u = ac->getGroup();
	if (u.compare("")){
		res += " -g ";
		res += u;
	}
	u = ac->getWorkingDir();
	if (u.compare("")){
		res += " -D ";
		res += u;
	}
	if (ac->isSelectQueueAutom()){
		res += " -Q  ";
	}
	u = ac->getWorkingDir();
	if (u.compare("")){
		res += " -D ";
		res += u;
	}
	if(ac->getCriterion()!=NULL) {
		res += " -L ";
		res += convertToString((ac->getCriterion())->getLoadType());
	}

	u    = msg.substr(separator.at(2)+1, msg.size()-separator.at(2));
	TMS_Data::Job_ptr j = NULL;

	//To parse the object serialized
	if(!parseEmfObject(u, j)) {
		throw SystemException(ERRCODE_INVMAPPER, "job: "+u);
	}
	u = j->getJobPath();
	if (u.compare("")){
		res += " ";
		res += u;
	}
	u = ac->getTextParams();
	if (u.compare("")){
		size_t lastPos = 0;
		size_t pos = u.find(" ", lastPos) ;
		while(pos != std::string::npos) {
			res += " -v ";
			res += u.substr(lastPos, pos - lastPos);
			lastPos = ++pos ;
			pos = u.find(" ", lastPos) ;
		}
		res += " -v ";
		res += u.substr(lastPos, pos - lastPos);
	}
	u = ac->getFileParams();
	if (u.compare("")){
		size_t lastPos = 0;
		size_t pos = u.find(" ", lastPos) ;
		while(pos != std::string::npos) {
			res += " -f ";
			res += u.substr(lastPos, pos - lastPos);
			lastPos = ++pos ;
			pos = u.find(" ", lastPos) ;
		}
		res += " -f ";
		res += u.substr(lastPos, pos - lastPos);
	}

	u = ac->getWorkId();
	if (u.compare("")){
		res += " -w ";
		res += u;
	}

	return res;
}

string
TMSMapper::decodeProg(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	res += (mmap.find(VISHNU_GETJOBPROG))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
	TMS_Data::ProgressOptions_ptr j = NULL;

	//To parse the object serialized
	if(!vishnu::parseEmfObject(u, j)) {
		throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
	}
	u = j->getJobOwner();
	if (u.compare("")){
		res += " -u ";
		res += u;
	}
	u = j->getJobId();
	if (u.compare("")){
		res += " -i ";
		res += u;
	}
	return res;
}

string
TMSMapper::decodeQueue(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	res += (mmap.find(VISHNU_LISTQUEUES))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	// size-2 because # after, and numerotation starts at 0
	if (separator.at(1)!=(msg.size()-2)) {
		u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
		res += " -q ";
		res += u;
	}
	return res;
}

string
TMSMapper::decodeListJob(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	long l;
	boost::posix_time::ptime pt;
	res += (mmap.find(VISHNU_LISTJOBS))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));

	TMS_Data::ListJobsOptions_ptr j = NULL;

	//To parse the object serialized
	if(!vishnu::parseEmfObject(u, j)) {
		throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
	}
	u = j->getJobId();
	if (u.compare("")){
		res += " -i ";
		res += u;
	}
	l = j->getNbCpu();
	if (l>0){
		res += " -P ";
		res += convertToString(l);
	}
	l = j->getFromSubmitDate();
	if (l>0){
		pt = boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(l));
		u = boost::posix_time::to_simple_string(pt);
		res += " -d '";
		res += u;
		res += "'";
	}
	l = j->getToSubmitDate();
	if (l>0){
		pt = boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(l));
		u = boost::posix_time::to_simple_string(pt);
		res += " -D '";
		res += u;
		res += "'";
	}
	u = j->getOwner();
	if (u.compare("")){
		res += " -u ";
		res += u;
	}
	l = j->getStatus();
	if (l>0){
		res += " -s ";
		res += convertToString(l);
	}
	l = j->getPriority();
	if (l>0){
		res += " -p ";
		res += convertToString(l);
	}
	u = j->getQueue();
	if (u.compare("")){
		res += " -q ";
		res += u;
	}
	u = j->getMultipleStatus();
	if (u.compare("")){
		res += " -S ";
		res += u;
	}
	if (j->isBatchJob()){
		res += " -b ";
	}

	if (j->isListAll()) {
    res += " -l ";
  }

	return res;
}
string
TMSMapper::decodeOutput(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	res += (mmap.find(VISHNU_GETJOBOUTPUT))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	res+= " ";
	u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-1);

	TMS_Data::JobResult_ptr j = NULL;

	//To parse the object serialized
	if(!vishnu::parseEmfObject(u, j)) {
		throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
	}
	res+= " ";
	if (j->getJobId().compare("")) {
		res += j->getJobId();
		res+= " ";
	}
	if (separator.at(2)!=(msg.size()-2)) {
		u    = msg.substr(separator.at(2)+1, msg.size()-separator.at(2));
		res += " -o ";
		res += u;
	}
	return res;
}
string
TMSMapper::decodeCompletedJob(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	res += (mmap.find(VISHNU_GETCOMPLETEDJOB))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	res+= " ";
	u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
	if (u.compare(" ")){
		res += " -o ";
		res += u;
	}
	return res;
}

string
TMSMapper::decodeCancel(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	res += (mmap.find(VISHNU_CANCEL))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	res+= " ";
	u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));

	TMS_Data::Job_ptr j = NULL;

	//To parse the object serialized
	if(!vishnu::parseEmfObject(u, j)) {
		throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
	}
	if (j->getJobId().compare("")) {
		res += j->getJobId();
		res+= " ";
	}

	return res;
}
string
TMSMapper::decodeJobInfo(vector<int> separator, const string& msg){
	string res = string("");
	string u;
	res += (mmap.find(VISHNU_GETJOBINFO))->second;
	res+= " ";
	u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
	res += u;
	res+= " ";
	u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));

	TMS_Data::Job_ptr j = NULL;

	//To parse the object serialized
	if(!vishnu::parseEmfObject(u, j)) {
		throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
	}
	if (j->getJobId().compare("")) {
		res += j->getJobId();
		res+= " ";
	}
	return res;
}


string
TMSMapper::decodeAddWork(vector<int> separator, const string& msg){
  string res = string("");
// TODO FIX WHEN FUNCTIONNAL
//  string a;
//  int tmp;
//  res += (mmap.find(VISHNU_ADD_WORK))->second;
//  a    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
//
//  TMS_Data::Work_ptr ac = NULL;
//
//  //To parse the object serialized
//  if(!parseEmfObject(std::string(std::string(a)), ac)) {
//    throw SystemException(ERRCODE_INVMAPPER, "option: "+a);
//  }
//
//  res+=" ";
//  a = ac->getApplicationId();
//  if (a.compare("")){
//    res += " -a ";
//    res += a;
//  }
//  a = ac->getSubject();
//  if (a.compare("")){
//    res += " -s ";
//    res += a;
//  }
//  tmp = ac->getPriority();
//  if (tmp!=0){
//    res += " -l ";
//    res += convertToString(tmp);
//  }
//  a = ac->getOwner();
//  if (a.compare("")){
//    res += " -u ";
//    res += a;
//  }
//  tmp = ac->getEstimatedHour();
//  if (tmp!=0){
//    res += " -p ";
//    res += convertToString(tmp);
//  }
//  a = ac->getDescription();
//  if (a.compare("")){
//    res += " -d ";
//    res += a;
//  }
//  a = ac->getProjectId();
//  if (a.compare("")){
//    res += " -p ";
//    res += a;
//  }
//  a = ac->getMachineId();
//  if (a.compare("")){
//    res += " -m ";
//    res += a;
//  }
//  tmp = ac->getNbCPU();
//  if (tmp!=0){
//    res += " -n ";
//    res += convertToString(tmp);
//  }
//
//  if (ac != NULL) {
//    delete ac;
//  }

  return res;
}
