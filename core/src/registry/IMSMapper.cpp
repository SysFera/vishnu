/**
 * \file IMSMapper.ccp
 * \brief Implementation of the IMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "IMSMapper.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;

IMSMapper::IMSMapper(){
};

IMSMapper::IMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(VISHNU_EXPORT, "exportCommands"));
  mmap.insert (pair<int, string>(VISHNU_GET_CUR, "getMetricCurrentValue"));
  mmap.insert (pair<int, string>(VISHNU_GET_HIST, "getMetricHistory"));
  mmap.insert (pair<int, string>(VISHNU_GET_PROC, "getProcesses"));
  mmap.insert (pair<int, string>(VISHNU_SET_SYSINF, "setSystemInfo"));
  mmap.insert (pair<int, string>(VISHNU_SET_THRESH, "setSystemThreshold"));
  mmap.insert (pair<int, string>(VISHNU_GET_THRESH, "getSystemThreshold"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_UID, "defineUserIdentifier"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_MID, "defineMachineIdentifier"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_TID, "defineJobIdentifier"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_FID, "defineTransferIdentifier"));
  mmap.insert (pair<int, string>(VISHNU_LOADSHED, "loadShed"));
  mmap.insert (pair<int, string>(VISHNU_SET_FREQ, "setUpdateFrequency"));
  mmap.insert (pair<int, string>(VISHNU_GET_FREQ, "getUpdateFrequency"));
  mmap.insert (pair<int, string>(VISHNU_STOP, "stop"));
  mmap.insert (pair<int, string>(VISHNU_RESTART, "restart"));
  mmap.insert (pair<int, string>(VISHNU_GET_SYSINF, "getSystemInfo"));
};

int
IMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
IMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
IMSMapper::getCommand(const int& key,string& command){
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
IMSMapper::getKey(const string& command, int& key){
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
IMSMapper::code(const string& cmd, unsigned int code){
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
IMSMapper::decode (const string& msg){
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
  case VISHNU_EXPORT           	    :
    res = decodeExp(separatorPos, msg);
    break;
  case VISHNU_GET_CUR 	  	:
    res = decodeCur(separatorPos, msg);
    break;
  case VISHNU_GET_HIST	          	:
    res = decodeHist(separatorPos, msg);
    break;
  case VISHNU_GET_PROC :
    res = decodeProc(separatorPos, msg);
    break;
  case VISHNU_SET_SYSINF	:
    res = decodeSetSys(separatorPos, msg);
    break;
  case VISHNU_SET_THRESH	:
    res = decodeSetThre(separatorPos, msg);
    break;
  case VISHNU_GET_THRESH   	:
    res = decodeGetThre(separatorPos, msg);
    break;
  case VISHNU_DEFINE_FID    	:
    res = decodeFid(separatorPos, msg);
    break;
  case VISHNU_DEFINE_TID 	:
    res = decodeTid(separatorPos, msg);
    break;
  case VISHNU_DEFINE_MID 	:
    res = decodeMid(separatorPos, msg);
    break;
  case VISHNU_DEFINE_UID 	:
    res = decodeUid(separatorPos, msg);
    break;
  case VISHNU_LOADSHED 	:
    res = decodeLoad(separatorPos, msg);
    break;
  case VISHNU_GET_FREQ 	:
    res = decodeGetF(separatorPos, msg);
    break;
  case VISHNU_SET_FREQ 	:
    res = decodeSetF(separatorPos, msg);
    break;
  case VISHNU_STOP 	:
    res = decodeStop(separatorPos, msg);
    break;
  case VISHNU_RESTART 	:
    res = decodeRestart(separatorPos, msg);
    break;
  case VISHNU_GET_SYSINF 	:
    res = decodeGetSys(separatorPos, msg);
    break;
  }
  return res;
}


string
IMSMapper::decodeExp(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeCur(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeHist(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeProc(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeSetSys(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeGetThre(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeSetThre(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeFid(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeMid(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeUid(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeTid(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeLoad(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeGetF(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeSetF(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeStop(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeRestart(vector<int> separator, const string& msg) {
  return "";
}
string
IMSMapper::decodeGetSys(vector<int> separator, const string& msg) {
  return "";
}
