/**
 * \file IMSMapper.ccp
 * \brief Implementation of the IMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "IMSMapper.hpp"
#include "utilVishnu.hpp"
#include "IMS_Data.hpp"
#include "IMS_Data_forward.hpp"
#include "utilServer.hpp"
#include "IMSVishnuException.hpp"

using namespace vishnu;

IMSMapper::IMSMapper(){
};

IMSMapper::IMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(VISHNU_EXPORT, "vishnu_export_commands"));
  mmap.insert (pair<int, string>(VISHNU_GET_CUR, "vishnu_get_metric_current_value"));
  mmap.insert (pair<int, string>(VISHNU_GET_HIST, "vishnu_get_metric_history"));
  mmap.insert (pair<int, string>(VISHNU_GET_PROC, "vishnu_get_processes"));
  mmap.insert (pair<int, string>(VISHNU_SET_SYSINF, "vishnu_set_system_info"));
  mmap.insert (pair<int, string>(VISHNU_SET_THRESH, "vishnu_set_system_threshold"));
  mmap.insert (pair<int, string>(VISHNU_GET_THRESH, "vishnu_get_system_threshold"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_UID, "vishnu_define_user_identifier"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_MID, "vishnu_define_machine_identifier"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_TID, "vishnu_define_job_identifier"));
  mmap.insert (pair<int, string>(VISHNU_DEFINE_FID, "vishnu_define_transfer_identifier"));
  mmap.insert (pair<int, string>(VISHNU_LOADSHED, "vishnu_load_shed"));
  mmap.insert (pair<int, string>(VISHNU_SET_FREQ, "vishnu_set_update_frequency"));
  mmap.insert (pair<int, string>(VISHNU_GET_FREQ, "vishnu_get_update_frequency"));
  mmap.insert (pair<int, string>(VISHNU_STOP, "vishnu_stop"));
  mmap.insert (pair<int, string>(VISHNU_RESTART, "vishnu_restart"));
  mmap.insert (pair<int, string>(VISHNU_GET_SYSINF, "vishnu_get_system_info"));
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
  default :
    break;
  }
  return res;
}


string
IMSMapper::decodeExp(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_EXPORT))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
  res += u;
  res += " ";
  u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-1);
  res += u;
  res += " ";
  u    = msg.substr(separator.at(2)+1, msg.size()-separator.at(2));
  IMS_Data::ExportOp_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = convertToString(ac->getExportType());
  if (u.compare("")) {
    res+=" -t ";
    res += u;
  }
  return res;
}

string
IMSMapper::decodeCur(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_GET_CUR))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
  res += u;
  res += " ";
  u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
  IMS_Data::CurMetricOp_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = convertToString(ac->getMetricType());
  if (u.compare("")) {
    res+=" -t ";
    res += u;
  }
  return res;
}
string
IMSMapper::decodeHist(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  long l;
  boost::posix_time::ptime pt;
  res += (mmap.find(VISHNU_GET_HIST))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
  res += u;
  res += " ";
  u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
  IMS_Data::MetricHistOp_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = convertToString(ac->getType());
  if (u.compare("")) {
    res+=" -t ";
    res += u;
  }
  l = ac->getStartTime();
  if (l > 0) {
    pt = boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(l));
    u = boost::posix_time::to_simple_string(pt);
    res+=" -s \"";
    res += u;
    res += "\"";
  }
  l = ac->getEndTime();
  if (l > 0) {
    pt = boost::posix_time::from_time_t(vishnu::convertUTCtimeINLocaltime(l));
    u = boost::posix_time::to_simple_string(pt);
    res+=" -e \"";
    res += u;
    res += "\"";
  }
  return res;
}

string
IMSMapper::decodeProc(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_GET_PROC))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  IMS_Data::ProcessOp_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = ac->getMachineId();
  if (u.compare("")) {
    res+=" -p ";
    res += u;
  }
  return res;
}
string
IMSMapper::decodeSetSys(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_SET_SYSINF))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  IMS_Data::SystemInfo_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = ac->getMachineId();
  if (u.compare("")) {
    res += u;
  }
  u = convertToString(ac->getMemory());
  if (u.compare("")) {
    res+=" -m ";
    res += u;
  }
  u = convertToString(ac->getDiskSpace());
  if (u.compare("")) {
    res+=" -d ";
    res += u;
  }
  return res;
}

string
IMSMapper::decodeGetThre(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_GET_THRESH))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  IMS_Data::ThresholdOp_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = ac->getMachineId();
  if (u.compare("")) {
    res+=" -m ";
    res += u;
  }
  u = convertToString(ac->getMetricType());
  if (u.compare("")) {
    res+=" -t ";
    res += u;
  }
  return res;
}
string
IMSMapper::decodeSetThre(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_SET_THRESH))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  IMS_Data::Threshold_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }

  res += convertToString(ac->getValue());
  res += " ";
  res += ac->getMachineId();
  res += " ";
  res += convertToString(ac->getType());
  res += " ";
  res += ac->getHandler();
  return res;
}
string
IMSMapper::decodeFid(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_DEFINE_FID))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  res += u;
  return res;
}
string
IMSMapper::decodeMid(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_DEFINE_MID))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  res += u;
  return res;
}

string
IMSMapper::decodeUid(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_DEFINE_UID))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  res += u;
  return res;
}
string
IMSMapper::decodeTid(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_DEFINE_TID))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  res += u;
  return res;
}

string
IMSMapper::decodeLoad(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_LOADSHED))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-2);
  res += u;
  res += " ";
  u    = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
  res += convertToString(u);
  return res;
}

string
IMSMapper::decodeGetF(vector<int> separator, const string& msg) {
  string res = string("");
  res += (mmap.find(VISHNU_GET_FREQ))->second;
  return res;
}
string
IMSMapper::decodeSetF(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_SET_FREQ))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  res += convertToString(u);
  return res;
}

string
IMSMapper::decodeStop(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_STOP))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  IMS_Data::Process_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  res += ac->getProcessName();
  res += " ";
  res += ac->getMachineId();
  return res;
}
string
IMSMapper::decodeRestart(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_RESTART))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-3);
  res += u;
  res += " ";
  u = msg.substr(separator.at(1)+1, msg.size()-separator.at(1));
  IMS_Data::RestartOp_ptr ac = NULL;
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  res += ac->getVishnuConf();
  res += " ";
  res += convertToString(ac->getSedType());
  return res;
}
string
IMSMapper::decodeGetSys(vector<int> separator, const string& msg) {
  string res = string("");
  string u;
  res += (mmap.find(VISHNU_GET_SYSINF))->second;
  res += " ";
  u    = msg.substr(separator.at(0)+1, msg.size()-separator.at(0));
  IMS_Data::SysInfoOp_ptr ac = NULL;
  //To parse the object serialized
  if(!parseEmfObject(u, ac)) {
    throw IMSVishnuException(ERRCODE_INVALID_PARAM);
  }
  u = ac->getMachineId();
  if (u.compare("")) {
    res+=" -m ";
    res += u;
  }
  return res;
}
