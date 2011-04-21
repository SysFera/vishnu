#include "IMSVishnuTool.hpp"
#include "LogORBMgr.hh"
#include "SystemException.hpp"

IMSVishnuTool::IMSVishnuTool(int argc, char** argv){
  try {
    LogORBMgr::init(argc, argv, false);
  } catch (...) {
    fprintf (stderr, "ORB initialization failed");
  }
  setFilter("");
  mfilename = mname+".log";
  LogORBMgr::getMgr()->activate((IMSVishnuTool*)this);
}

IMSVishnuTool::~IMSVishnuTool(){
}

void 
IMSVishnuTool::run(){
  // Connexion to the LCT to get messages
  mLCT = LogORBMgr::getMgr()->resolve<LogCentralTool, LogCentralTool_ptr>("LogServiceT", "LCT");
  if (CORBA::is_nil(mLCT)){
    cout << "Failed to narrow " << endl;
    throw SystemException(ERRCODE_SYSTEM, "Failed to narrow the LCT ! \n");
  }

  // Binding the tool
  try{
    LogORBMgr::getMgr()->bind("LogServiceT", mname, _this(), true);
    LogORBMgr::getMgr()->fwdsBind("LogServiceT", mname,
				  LogORBMgr::getMgr()->getIOR(_this()));
  }
  catch (...){
    throw SystemException(ERRCODE_SYSTEM, "Failed to bind ! \n");
  }

  // Connecting the tool
  try{
    char* name = CORBA::string_dup(mname.c_str());
    mLCT->connectTool (name, mname.c_str());
    mLCT->addFilter(mname.c_str(), mfilter);
  }
  catch (...){
    throw SystemException(ERRCODE_SYSTEM, "Failed to connect the tool ! \n");
  }
}

int
IMSVishnuTool::disconnect(){
  return mLCT->disconnectTool (mname.c_str());
}

// TODO IMPLEMENT THE FUNCTION TO PLAY ITS ROLES WITH THE DATABASE
void
IMSVishnuTool::sendMsg(const log_msg_buf_t& msg){
  for (CORBA::ULong i=0; i<msg.length(); i++){
    string log;
    // componentName
    log.append(msg[i].componentName);
    log.append(":");
    if (string(msg[i].tag).compare("IN")==0 || string(msg[i].msg).compare("sessionConnect")==0){
      cout << "Connexion of the component with the name : " << msg[i].componentName << " and the message ->" << msg[i].msg << "<- " << endl;
    }
    if (string(msg[i].tag).compare("OUT")==0){
      cout << "Disconnexion of the component with the name : " << msg[i].componentName << endl;
    }
    //tagtype
    if (string(msg[i].tag).compare("IN")==0){
      log.append(msg[i].tag);
      log.append(":");
      //msg
      log.append(msg[i].msg);    
    }
    else {
      if (string(msg[i].msg).compare("sessionConnect")==0){
	log.append(msg[i].tag);
	log.append(":");
	//msg
	log.append(msg[i].msg);    
	cout << "The SeD " << msg[i].componentName << "is of type UMS" << endl;
      }
      else if (string(msg[i].msg).compare("jobSubmit_machine_1")==0) {
	log.append(msg[i].tag);
	log.append(":");
	//msg
	log.append(msg[i].msg);
	cout << "The SeD " << msg[i].componentName << " is of type TMS" << endl;
      }// End else if submit machine  
    }// End if not IN tag
  }// End for
}

void
IMSVishnuTool::setFilter(string description_file){
  mfilter.filterName = CORBA::string_dup("allFilter");
  mfilter.tagList.length(2);
  mfilter.tagList[0] = CORBA::string_dup("IN");
  mfilter.tagList[1] = CORBA::string_dup("ADD_SERVICE");
  mfilter.componentList.length(1);
  mfilter.componentList[0] = CORBA::string_dup("*");
}

void
IMSVishnuTool::setFilename(string name){
  mfilename = name;
}
