/**
 * \file TMSMapper.ccp
 * \brief Implementation of the TMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "TMSMapper.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;

TMSMapper::TMSMapper(){
};

TMSMapper::~TMSMapper(){
};

TMSMapper::TMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(VISHNU_SUBMITJOB, "submitJob"));
  mmap.insert (pair<int, string>(VISHNU_GETJOBINFO, "getJobInfo"));
  mmap.insert (pair<int, string>(VISHNU_GETJOBPROG, "getJobProgress"));
  mmap.insert (pair<int, string>(VISHNU_LISTQUEUES, "listQueues"));
  mmap.insert (pair<int, string>(VISHNU_LISTJOBS, "listJobs"));
  mmap.insert (pair<int, string>(VISHNU_GETJOBOUTPUT, "getJobOutPut"));
  mmap.insert (pair<int, string>(VISHNU_GETALLJOBOUTPUT, "getAllJobsOutPut"));
  mmap.insert (pair<int, string>(VISHNU_CANCEL, "cancelJob"));
  mmap.insert (pair<int, string>(VISHNU_GETMACHINEREFRESHPERIOD, "getMachineRefreshPeriod"));
  mmap.insert (pair<int, string>(VISHNU_SETMACHINEREFRESHPERIOD, "setMachineRefreshPeriod"));
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
  for (it = mmap.begin() ; it != mmap.end() ; it++){
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
  for (it = mmap.begin() ; it != mmap.end() ; it++){
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
  if(separatorPos.size()>0){
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
  case VISHNU_GETALLJOBOUTPUT:
    res = decodeAllOutput(separatorPos, msg);
    break;
  case VISHNU_CANCEL:
    res = decodeCancel(separatorPos, msg);
    break;
  case VISHNU_GETMACHINEREFRESHPERIOD:
    res = decodeGetPeriod(separatorPos, msg);
    break;
  case VISHNU_SETMACHINEREFRESHPERIOD:
    res = decodeSetPeriod(separatorPos, msg);
    break;
  case VISHNU_GETJOBINFO:
    res = decodeJobInfo(separatorPos, msg);
    break;
  default:
    res = "";
    break;
  }
  return res;
}


string
TMSMapper::decodeSubmit(vector<int> separator, const string& msg){
  return "";
}

string
TMSMapper::decodeProg(vector<int> separator, const string& msg){
  return "";
}

string
TMSMapper::decodeQueue(vector<int> separator, const string& msg){
  return "";
}

string
TMSMapper::decodeListJob(vector<int> separator, const string& msg){
  return "";
}
string
TMSMapper::decodeOutput(vector<int> separator, const string& msg){
  return "";
}
string
TMSMapper::decodeAllOutput(vector<int> separator, const string& msg){
  return "";
}
string
TMSMapper::decodeCancel(vector<int> separator, const string& msg){
  return "";
}
string
TMSMapper::decodeGetPeriod(vector<int> separator, const string& msg){
  return "";
}
string
TMSMapper::decodeSetPeriod(vector<int> separator, const string& msg){
  return "";
}
string
TMSMapper::decodeJobInfo(vector<int> separator, const string& msg){
  return "";
}







