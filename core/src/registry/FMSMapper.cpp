/**
 * \file FMSMapper.ccp
 * \brief Implementation of the FMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * and Daouda Traore (daouda.traore@sysfera.com)
 * \date May 2011
 */

#include "FMSMapper.hpp"
#include "FMS_Data.hpp"
#include "FMS_Data_forward.hpp"
#include "utilVishnu.hpp"
#include "utilServer.hpp"
#include "FMSVishnuException.hpp"

using namespace vishnu;

FMSMapper::FMSMapper(){
};

FMSMapper::FMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  mname = na;
  mmap.insert (pair<int, string>(VISHNU_CREATE_FILE,  "vishnu_create_file"));
  mmap.insert (pair<int, string>(VISHNU_CREATE_DIR,  "vishnu_create_dir"));
  mmap.insert (pair<int, string>(VISHNU_REMOVE_FILE,  "vishnu_remove_file"));
  mmap.insert (pair<int, string>(VISHNU_REMOVE_DIR,  "vishnu_remove_dir"));
  mmap.insert (pair<int, string>(VISHNU_CH_GRP,  "vishnu_ch_grp"));
  mmap.insert (pair<int, string>(VISHNU_CH_MOD,  "vishnu_ch_mod"));
  mmap.insert (pair<int, string>(VISHNU_HEAD_OF_FILE,  "vishnu_head_of_file"));
  mmap.insert (pair<int, string>(VISHNU_TAIL_OF_FILE,  "vishnu_tail_of_file"));
  mmap.insert (pair<int, string>(VISHNU_CONTENT_OF_FILE,  "vishnu_content_of_file"));
  mmap.insert (pair<int, string>(VISHNU_LIST_DIR, "vishnu_list_dir"));
  mmap.insert (pair<int, string>(VISHNU_COPY_FILE, "vishnu_copy_file"));
  mmap.insert (pair<int, string>(VISHNU_COPY_ASYNC_FILE, "vishnu_copy_async_file"));
  mmap.insert (pair<int, string>(VISHNU_MOVE_FILE, "vishnu_move_file"));
  mmap.insert (pair<int, string>(VISHNU_MOVE_ASYNC_FILE, "vishnu_move_async_file"));
  mmap.insert (pair<int, string>(VISHNU_STOP_FILE_TRANSFER, "vishnu_stop_file_transfer"));
  mmap.insert (pair<int, string>(VISHNU_LIST_FILE_TRANSFERS, "vishnu_list_file_transfers"));
  mmap.insert (pair<int, string>(VISHNU_GET_FILE_INFO, "vishnu_get_file_info"));
};

int
FMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
FMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
FMSMapper::getCommand(const int& key,string& command){
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
FMSMapper::getKey(const string& command, int& key){
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
FMSMapper::code(const string& cmd, unsigned int code){
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
FMSMapper::decode (const string& msg){

  vector<unsigned int> separatorPos;
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
    case VISHNU_CREATE_FILE:
      res = decodeCreateFile(separatorPos, msg);
      break;
    case VISHNU_CREATE_DIR:
      res = decodeCreateDir(separatorPos, msg);
      break;
    case VISHNU_REMOVE_FILE:
      res = decodeRemoveFile(separatorPos, msg);
      break;
    case VISHNU_REMOVE_DIR:
      res = decodeRemoveDir(separatorPos, msg);
      break;
    case VISHNU_CH_GRP:
      res = decodeChGrp(separatorPos, msg);
      break;
    case VISHNU_CH_MOD:
      res = decodeChMod(separatorPos, msg);
      break;
    case VISHNU_HEAD_OF_FILE:
      res = decodeHeadOfFile(separatorPos, msg);
      break;
    case VISHNU_TAIL_OF_FILE:
      res = decodeTailOfFile(separatorPos, msg);
      break;
    case VISHNU_CONTENT_OF_FILE:
      res = decodeContentOfFile(separatorPos, msg);
      break;
    case VISHNU_LIST_DIR:
      res = decodeListDir(separatorPos, msg);
      break;
    case VISHNU_COPY_FILE:
      res = decodeCopyFile(separatorPos, msg);
      break;
    case VISHNU_COPY_ASYNC_FILE:
      res = decodeCopyAsyncFile(separatorPos, msg);
      break;
    case VISHNU_MOVE_FILE:
      res = decodeMoveFile(separatorPos, msg);
      break;
    case VISHNU_MOVE_ASYNC_FILE:
      res = decodeMoveAsyncFile(separatorPos, msg);
      break;
    case VISHNU_STOP_FILE_TRANSFER:
      res = decodeStopFileTransfer(separatorPos, msg);
      break;
    case VISHNU_LIST_FILE_TRANSFERS:
      res = decodeListFileTransfers(separatorPos, msg);
      break;
    case VISHNU_GET_FILE_INFO:
      res = decodeGetFileInfo(separatorPos, msg);
      break;
    default:
      res = "";
      break;
  }
  return res;
}

string
FMSMapper::decodeCreateFile(vector<unsigned int> separator, const string& msg) {
  string res = "";
  string u;
  res += (mmap.find(VISHNU_CREATE_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1);
  res += u;

  return res;
}

string
FMSMapper::decodeCreateDir(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_CREATE_DIR))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";

  u  = msg.substr(separator.at(1)+1);
  FMS_Data::CreateDirOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->isIsRecursive()) {
    res += " -p ";
  }

  return res;
}

string
FMSMapper::decodeRemoveFile(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_REMOVE_FILE))->second;
  res+= " ";
  u   = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";

  u  = msg.substr(separator.at(1)+1);
  FMS_Data::RmFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->isIsRecursive()) {
    res += " -r ";
  }

  return res;
}


string
FMSMapper::decodeRemoveDir(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_REMOVE_DIR))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1);
  res += u;

  return res;
}

string
FMSMapper::decodeChGrp(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_CH_GRP))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";
  u    = msg.substr(separator.at(1)+1);
  res+= u;

  return res;
}

string
FMSMapper::decodeChMod(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_CH_MOD))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";
  u    = msg.substr(separator.at(1)+1);
  res +=u;

  return res;
}

string
FMSMapper::decodeHeadOfFile(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_HEAD_OF_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(1)+1);
  FMS_Data::HeadOfFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  res += " -n ";
  res += vishnu::convertToString(ac->getNline());

  return res;
}

string
FMSMapper::decodeTailOfFile(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_TAIL_OF_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(1)+1);
  FMS_Data::TailOfFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  res += " -n ";
  res += vishnu::convertToString(ac->getNline());

  return res;
}

string
FMSMapper::decodeContentOfFile(vector<unsigned int> separator, const string& msg){
  string res = "";
  string u;
  res += (mmap.find(VISHNU_CONTENT_OF_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1);
  res += u;

  return res;
}

string
FMSMapper::decodeListDir(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_LIST_DIR))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(1)+1);
  FMS_Data::LsDirOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->isLongFormat()) {
    res += " -l ";
  }

  if(ac->isAllFiles()) {
    res += " -a ";
  }
  return res;

}

string
FMSMapper::decodeCopyFile(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_COPY_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";
  u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(2)+1);
  FMS_Data::CpFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->isIsRecursive()) {
    res += " -r ";
  }

  if(ac->getTrCommand()!=-1){
    res += " -t "+vishnu::convertToString(ac->getTrCommand());
  }

  return res;
}

string
FMSMapper::decodeCopyAsyncFile(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_COPY_ASYNC_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";
  u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(2)+1);
  FMS_Data::CpFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->isIsRecursive()) {
    res += " -r ";
  }

  if(ac->getTrCommand()!=-1){
    res += " -t "+vishnu::convertToString(ac->getTrCommand());
  }

  return res;
}

string
FMSMapper::decodeMoveFile(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_MOVE_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";
  u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(2)+1);
  FMS_Data::CpFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->getTrCommand()!=-1){
    res += " -t "+vishnu::convertToString(ac->getTrCommand());
  }

  return res;
}

string
FMSMapper::decodeMoveAsyncFile(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_MOVE_ASYNC_FILE))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1, separator.at(1)-separator.at(0)-1);
  res += u;
  res+= " ";
  u    = msg.substr(separator.at(1)+1, separator.at(2)-separator.at(1)-1);
  res += u;
  res+= " ";

  u    = msg.substr(separator.at(2)+1);
  FMS_Data::CpFileOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->getTrCommand()!=-1){
    res += " -t "+vishnu::convertToString(ac->getTrCommand());
  }

  return res;
}

string
FMSMapper::decodeStopFileTransfer(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_STOP_FILE_TRANSFER))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1);
  FMS_Data::StopTransferOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->getTransferId().size()!=0) {
     res += " -i "+ac->getTransferId();
  }

  if(ac->getFromMachineId().size()!=0) {
     res += " -m "+ac->getFromMachineId();
  }

  if(ac->getUserId().size()!=0) {
     res += " -u "+ac->getUserId();
  }

  return res;
}

string
FMSMapper::decodeListFileTransfers(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_LIST_FILE_TRANSFERS))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1);
  FMS_Data::LsTransferOptions_ptr ac = NULL;

  //To parse the object serialized
  if(!vishnu::parseEmfObject(u, ac)) {
    throw SystemException(ERRCODE_INVMAPPER, "option: "+u);
  }

  if(ac->getTransferId().size()!=0) {
     res += " -t "+ac->getTransferId();
  }

  if(ac->getFromMachineId().size()!=0) {
     res += " -m "+ac->getFromMachineId();
  }

  if(ac->getUserId().size()!=0) {
     res += " -u "+ac->getUserId();
  }

  if(ac->getStatus()!=-1) {
     res += " -s "+convertToString((ac->getStatus()));
  }

  return res;
}

string
FMSMapper::decodeGetFileInfo(vector<unsigned int> separator, const string& msg){

  string res = "";
  string u;
  res += (mmap.find(VISHNU_GET_FILE_INFO))->second;
  res+= " ";
  u    = msg.substr(separator.at(0)+1);
  res += u;

  return res;
}
