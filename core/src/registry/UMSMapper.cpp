/**
 * \file UMSMapper.ccp
 * \brief Implementation of the UMS mapper
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 10/02/11
 */

#include "UMSMapper.hpp"
#include "UMS_Data.hpp"
#include "UMS_Data_forward.hpp"
#include "UserServer.hpp"
#include "SessionServer.hpp"
#include "MachineServer.hpp"
#include "LocalAccountServer.hpp"
#include "ConfigurationServer.hpp"
#include "QueryServer.hpp"
#include "CommandServer.hpp"

using namespace vishnu;

UMSMapper::UMSMapper(){
  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
};

UMSMapper::~UMSMapper(){
};

UMSMapper::UMSMapper(MapperRegistry* reg, string na):Mapper(reg){
  //CREATE DATA MODEL
  UMS_DataPackage_ptr ecorePackage = UMS_DataPackage::_instance();
  ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);
  mname = na;
  mmap.insert (pair<int, string>(VISHNU_CONNECT           	, "vishnu_connect"));
  mmap.insert (pair<int, string>(VISHNU_RECONNECT 	  	, "vishnu_reconnect"));
  mmap.insert (pair<int, string>(VISHNU_CLOSE 	          	, "vishnu_close"));
  mmap.insert (pair<int, string>(VISHNU_ADD_VISHNU_USER   	, "vishnu_add_user"));
  mmap.insert (pair<int, string>(VISHNU_UPDATE_VISHNU_USER	, "vishnu_update_user"));
  mmap.insert (pair<int, string>(VISHNU_DELETE_VISHNU_USER	, "vishnu_delete_user"));
  mmap.insert (pair<int, string>(VISHNU_CHANGE_PASSWORD   	, "vishnu_change_password"));
  mmap.insert (pair<int, string>(VISHNU_RESET_PASSWORD    	, "vishnu_reset_password"));
  mmap.insert (pair<int, string>(VISHNU_ADD_LOCAL_ACCOUNT 	, "vishnu_add_local_account"));
  mmap.insert (pair<int, string>(VISHNU_UPDATE_LOCAL_ACCOUNT    , "vishnu_update_local_account"));
  mmap.insert (pair<int, string>(VISHNU_DELETE_LOCAL_ACCOUNT    , "vishnu_delete_local_account"));
  mmap.insert (pair<int, string>(VISHNU_SAVE_CONFIGURATION      , "vishnu_save_configuration"));
  mmap.insert (pair<int, string>(VISHNU_RESTORE_CONFIGURATION   , "vishnu_restore_configuration"));
  mmap.insert (pair<int, string>(VISHNU_ADD_MACHINE 	        , "vishnu_add_machine"));
  mmap.insert (pair<int, string>(VISHNU_UPDATE_MACHINE 	   	, "vishnu_update_machine"));
  mmap.insert (pair<int, string>(VISHNU_DELETE_MACHINE 	   	, "vishnu_delete_machine"));
  mmap.insert (pair<int, string>(VISHNU_LIST_LOCAL_ACCOUNT      , "vishnu_list_local_account"));
  mmap.insert (pair<int, string>(VISHNU_LIST_MACHINE 	        , "vishnu_list_machine"));
  mmap.insert (pair<int, string>(VISHNU_LIST_HISTORY_CMD        , "vishnu_list_history_cmd"));
  mmap.insert (pair<int, string>(VISHNU_LIST_OPTIONS 	        , "vishnu_list_options"));
  mmap.insert (pair<int, string>(VISHNU_LIST_USERS 	        , "vishnu_list_users"));
  mmap.insert (pair<int, string>(VISHNU_LIST_SESSIONS 	        , "vishnu_list_sessions"));
  mmap.insert (pair<int, string>(VISHNU_CONFIGURE_DEFAULT_OPTION, "vishnu_configure_default_option"));
  mmap.insert (pair<int, string>(VISHNU_CONFIGURE_OPTION	, "vishnu_configure_option"));
};

int
UMSMapper::registerMapper(){
  mreg->addMapper(mname, this);
  return 0;
}

int
UMSMapper::unregisterMapper(){
  return mreg->removeMapper(mname);
}

int
UMSMapper::getCommand(const int& key,string& command){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (key == it->first){
      command = it->second;
      return 0;
    }
  }
}

int
UMSMapper::getKey(const string& command, int& key){
  map<int, string>::const_iterator it;
  for (it = mmap.begin() ; it != mmap.end() ; it++){
    if (command.compare(it->second)==0){
      key = it->first;
      return 0;
    }
  }
}

int
UMSMapper::code(const string& cmd, unsigned int code){
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
UMSMapper::finalize(int key){
  map<int, string>::iterator it;
  string res;
  pthread_mutex_lock(&mutex);
  it = mcmd.find(key);
  if (it==mcmd.end()){
    std::cout << "key not found " << key << std::endl;
    throw new SystemException(ERRCODE_SYSTEM, "Unknown key to finalize");
  }
  res = it->second;
  std::cout << "string gotten : " << res << std::endl;
  mcmd.erase(it->first);
  pthread_mutex_unlock(&mutex);
  return res;
}

string
UMSMapper::decode (const string& msg){
  vector<int> separatorPos;
  string      func;
  int         funcCode;

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
  case VISHNU_CONNECT           	    :
    break;
  case VISHNU_RECONNECT 	  	:
    break;
  case VISHNU_CLOSE 	          	:
    return decodeClose(separatorPos, msg);
    break;
  case VISHNU_ADD_VISHNU_USER   	:
    return decodeAddUser(separatorPos, msg);
    break;
  case VISHNU_UPDATE_VISHNU_USER	:
    return decodeUpUser(separatorPos, msg);
    break;
  case VISHNU_DELETE_VISHNU_USER	:
    return decodeDelUser(separatorPos, msg);
    break;
  case VISHNU_CHANGE_PASSWORD   	:
    return decodeChangePwd(separatorPos, msg);
    break;
  case VISHNU_RESET_PASSWORD    	:
    return decodeResetPwd(separatorPos, msg);
    break;
  case VISHNU_ADD_LOCAL_ACCOUNT 	:
    return decodeAddAcc(separatorPos, msg);
    break;
  case VISHNU_UPDATE_LOCAL_ACCOUNT    :
    return decodeUpAcc(separatorPos, msg);
    break;
  case VISHNU_DELETE_LOCAL_ACCOUNT    :
    return decodeDelAcc(separatorPos, msg);
    break;
  case VISHNU_SAVE_CONFIGURATION      :
    return decodeSaveConf(separatorPos, msg);
    break;
  case VISHNU_RESTORE_CONFIGURATION   :
    return decodeRestoreConf(separatorPos, msg);
    break;
  case VISHNU_ADD_MACHINE 	       : 
    return decodeAddM(separatorPos, msg);
    break;
  case VISHNU_UPDATE_MACHINE 	   	:
    return decodeUpM(separatorPos, msg);
    break;
  case VISHNU_DELETE_MACHINE 	   	:
    return decodeDelM(separatorPos, msg);
    break;
  case VISHNU_LIST_LOCAL_ACCOUNT      :
    return decodeListAcc(separatorPos, msg);
    break;
  case VISHNU_LIST_MACHINE 	       : 
    return decodeListM(separatorPos, msg);
    break;
  case VISHNU_LIST_HISTORY_CMD        :
    return decodeListH(separatorPos, msg);
    break;
  case VISHNU_LIST_OPTIONS 	       : 
    return decodeListOp(separatorPos, msg);
    break;
  case VISHNU_LIST_USERS 	        :
    return decodeListUser(separatorPos, msg);
    break;
  case VISHNU_LIST_SESSIONS 	        :
    return decodeListSession(separatorPos, msg);
    break;
  case VISHNU_CONFIGURE_DEFAULT_OPTION:
    return decodeConfDefaultOp(separatorPos, msg);
    break;
  case VISHNU_CONFIGURE_OPTION	:
    return decodeConfOp(separatorPos, msg);
    break;
  default :
    break;
 }
  
  // TODO
  return "";
}


void
UMSMapper::findSeparator(const string& s, vector<int>& vec){
  int cpt = 0;
  while ((cpt=s.find_first_of("#", cpt))!=string::npos){
    vec.push_back(cpt);
  }
}

// %RELAX<MISRA_0_1_3> Because no explicit parameter to close session, useless to parse, just return the function name
string
UMSMapper::decodeClose(vector<int> separator, const string& msg){ 
  return (mcmd.find(VISHNU_CLOSE))->second;
}

string
UMSMapper::decodeAddUser(vector<int> separator, const string& msg){ 
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_ADD_VISHNU_USER))->second;
  res += getU(u);
  u    = msg.substr(separator.at(0), msg.size()-separator.at(0));
  res += getU(u);
  return res;
}

string
UMSMapper::decodeUpUser(vector<int> separator, const string& msg){ 
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_UPDATE_VISHNU_USER))->second;
  res += getU(u);
  u    = msg.substr(separator.at(0), msg.size()-separator.at(0));
  res += getU(u);
  return res;
}

string
UMSMapper::decodeDelUser(vector<int> separator, const string& msg){
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_DELETE_VISHNU_USER))->second;
  u    = msg.substr(separator.at(0), msg.size()-separator.at(0));
  res += getU(u);
  return res;
}

string
UMSMapper::decodeChangePwd(vector<int> separator, const string& msg){ 
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_DELETE_VISHNU_USER))->second;
  u    = msg.substr(separator.at(0), separator.at(1));
  res += getU(u);
  res+= " ";
  u    = msg.substr(separator.at(1), msg.size()-separator.at(1));
  res += getU(u);
  return res;
}

string
UMSMapper::decodeResetPwd(vector<int> separator, const string& msg){
  string res = string("");
  res += (mcmd.find(VISHNU_RESET_PASSWORD))->second;
  res += msg.substr(separator.at(0), msg.size()-separator.at(0));
  return res;
}

string
UMSMapper::decodeAddAcc(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_ADD_LOCAL_ACCOUNT))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  LocalAccount_ptr ac = parser.load(std::string(a))->as< LocalAccount >();
  res+=" ";
  res += ac->getUserId();
  res+=" ";
  res += ac->getMachineId();
  res+=" ";
  res += ac->getAcLogin();
  res+=" ";
  res += ac->getSshKeyPath();
  res+=" ";
  res += ac->getHomeDirectory();
  return res;
}

string
UMSMapper::decodeUpAcc(vector<int> separator, const string& msg){
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_UPDATE_LOCAL_ACCOUNT))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  LocalAccount_ptr ac = parser.load(std::string(a))->as< LocalAccount >();

  a = ac->getAcLogin();
  if (a.compare("")){
    res+=" -l ";
    res+=a;
  }
  a = ac->getSshKeyPath();
  if (a.compare("")){
    res+=" -s ";
    res+=a;
  }
  a = ac->getHomeDirectory();
  if (a.compare("")){
    res+=" -d ";
    res+=a;
  }
  res+=" ";
  res += ac->getUserId();
  res+=" ";
  res += ac->getMachineId();
  return res;
}

string
UMSMapper::decodeDelAcc(vector<int> separator, const string& msg){  
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_DELETE_LOCAL_ACCOUNT))->second;
  u    = msg.substr(separator.at(0), separator.at(1));
  res += getU(u);
  res+= " ";
  u    = msg.substr(separator.at(1), msg.size()-separator.at(1));
  res += getU(u);
  return res;
}

// %RELAX<MISRA_0_1_3> Because no explicit parameter to close session, useless to parse, just return the function name
string
UMSMapper::decodeSaveConf(vector<int> separator, const string& msg){
  return (mcmd.find(VISHNU_SAVE_CONFIGURATION))->second;
}

string
UMSMapper::decodeRestoreConf(vector<int> separator, const string& msg){ 
  string res = string("");
  res += (mcmd.find(VISHNU_RESTORE_CONFIGURATION))->second;
  res += msg.substr(separator.at(0), msg.size()-separator.at(0));
  return res;
}

string
UMSMapper::decodeAddM(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_ADD_MACHINE))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  Machine_ptr ac = parser.load(std::string(a))->as< Machine >();
  res+=" ";
  res += ac->getName();
  res+=" ";
  res += ac->getSite();
  res+=" ";
  res += ac->getLanguage();
  res+=" ";
  res += ac->getSshPublicKey();
  return res;
}

string
UMSMapper::decodeUpM(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_UPDATE_MACHINE))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  Machine_ptr ac = parser.load(std::string(a))->as< Machine >();
  res+=" ";
  a = ac->getName();
  if (a.compare("")){
    res+=" -n ";
    res+=a;
  } 
  a = ac->getSite();
  if (a.compare("")){
    res+=" -n ";
    res+=a;
  } 
  a = ac->getMachineDescription();
  if (a.compare("")){
    res+=" -d ";
    res+=a;
  } 
  a = ac->getStatus();
  if (a.compare("")){
    res+=" -t ";
    res+=a;
  } 
  a = ac->getLanguage();
  if (a.compare("")){
    res+=" -t ";
    res+=a;
  } 
  a = ac->getSshPublicKey();
  if (a.compare("")){
    res+=" -k ";
    res+=a;
  } 
}

string
UMSMapper::decodeDelM(vector<int> separator, const string& msg){
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_DELETE_MACHINE))->second;
  u    = msg.substr(separator.at(0), msg.size()-separator.at(0));
  res += getU(u);
  return res;
}

string
UMSMapper::decodeListAcc(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_LIST_LOCAL_ACCOUNT))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  ListLocalAccOptions_ptr ac = parser.load(std::string(a))->as< ListLocalAccOptions >();
  if (ac->isAdminListOption()){
    res+=" -a ";
  }
  a = ac->getUserId();
  if (a.compare("")){
    res+=" -u ";
    res+=a;
  }
  a = ac->getMachineId();
  if (a.compare("")){
    res+=" -i ";
    res+=a;
  }
  return res;
}

string
UMSMapper::decodeListM(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_LIST_MACHINE))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  ListMachineOptions_ptr ac = parser.load(std::string(a))->as< ListMachineOptions >();
  a = ac->getUserId();
  if (a.compare("")){
    res+=" -u ";
    res+=a;
  }
  if (ac->isListAllmachine()){
    res+=" -a ";
  }
  a = ac->getMachineId();
  if (a.compare("")){
    res+=" -m ";
    res+=a;
  }
  return res;
}

string
UMSMapper::decodeListH(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_LIST_HISTORY_CMD))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  ListCmdOptions_ptr ac = parser.load(std::string(a))->as< ListCmdOptions >();
  if (ac->isAdminListOption()){
    res+=" -a ";
  }
  a = ac->getUserId();
  if (a.compare("")){
    res+=" -u ";
    res+=a;
  }
  a = ac->getSessionId();
  if (a.compare("")){
    res+=" -i ";
    res+=a;
  }
  a = ac->getStartDateOption();
  if (a.compare("")){
    res+=" -s ";
    res+=a;
  }
  a = ac->getEndDateOption();
  if (a.compare("")){
    res+=" -e ";
    res+=a;
  }
  return res;
}

string
UMSMapper::decodeListOp(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_LIST_OPTIONS))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  ListOptOptions_ptr ac = parser.load(std::string(a))->as< ListOptOptions >();
  if (ac->isListAllDeftValue()){
    res+=" -a ";
  }
  a = ac->getUserId();
  if (a.compare("")){
    res+=" -u ";
    res+=a;
  }
  a = ac->getOptionName();
  if (a.compare("")){
    res+=" -n ";
    res+=a;
  }
  return res;
}

string
UMSMapper::decodeListUser(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_LIST_USERS))->second;
  res += msg.substr(separator.at(0), msg.size()-separator.at(0));
  return res;
}

string
UMSMapper::decodeListSession(vector<int> separator, const string& msg){ 
  string res = string("");
  string a;
  res += (mcmd.find(VISHNU_LIST_SESSIONS))->second;
  a    = msg.substr(separator.at(0), msg.size()-separator.at(0));

  ecorecpp::parser::parser parser;
  ListSessionOptions_ptr ac = parser.load(std::string(a))->as< ListSessionOptions >();
  res+=" -t ";
  res += convertToString(ac->getStatus());
  res+=" -p ";
  res += convertToString(ac->getSessionClosePolicy());
  a = ac->getMachineId();
  res+=" -d ";
  res += ac->getSessionInactivityDelay();
  a = ac->getMachineId();
  if (a.compare("")){
    res+=" -m ";
    res+=a;
  }
  if (ac->isAdminListOption()){
    res+=" -a ";
  }
  a = ac->getUserId();
  if (a.compare("")){
    res+=" -u ";
    res+=a;
  }
  a = ac->getMachineId();
  if (a.compare("")){
    res+=" -m ";
    res+=a;
  }
  a = ac->getSessionId();
  if (a.compare("")){
    res+=" -i ";
    res+=a;
  }
  a = ac->getStartDateOption();
  if (a.compare("")){
    res+=" -s ";
    res+=a;
  }
  a = ac->getEndDateOption();
  if (a.compare("")){
    res+=" -e ";
    res+=a;
  }
  return res;
}

string
UMSMapper::decodeConfDefaultOp(vector<int> separator, const string& msg){ 
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_CONFIGURE_DEFAULT_OPTION))->second;
  u    = msg.substr(separator.at(0), separator.at(1));
  res += getU(u);
  res+= " ";
  u    = msg.substr(separator.at(1), msg.size()-separator.at(1));
  res += getU(u);
  return res;
}

string
UMSMapper::decodeConfOp(vector<int> separator, const string& msg){ 
  string res = string("");
  string u;
  res += (mcmd.find(VISHNU_CONFIGURE_OPTION))->second;
  u    = msg.substr(separator.at(0), separator.at(1));
  res += getU(u);
  res+= " ";
  u    = msg.substr(separator.at(1), msg.size()-separator.at(1));
  res += getU(u);
  return res;
}


string
UMSMapper::getU(string serial){
  string res = string("");

  ecorecpp::parser::parser parser;
  User_ptr user = parser.load(std::string(serial))->as< User >();

  res+=" ";
  res += user->getFirstname();
  res+=" ";
  res += user->getLastname();
  res+=" ";
  res += user->getPrivilege();
  res+=" ";
  res += user->getEmail();
  return res;

}

