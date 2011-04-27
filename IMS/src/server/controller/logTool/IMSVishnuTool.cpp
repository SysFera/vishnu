#include "IMSVishnuTool.hpp"
#include "LogORBMgr.hh"
#include "SystemException.hpp"
#include "UserServer.hpp"
#include "IMSVishnuException.hpp"
// For gethostname
#include <unistd.h>

IMSVishnuTool::IMSVishnuTool(int argc, char** argv):mproc(UserServer(SessionServer(""))){
  try {
    LogORBMgr::init(argc, argv, false);
  } catch (...) {
    fprintf (stderr, "ORB initialization failed");
  }
  setFilter("");
  mfilename = mname+".log";
  LogORBMgr::getMgr()->activate((IMSVishnuTool*)this);
  // Getting the hostname
  if (gethostname(msyshName, HNAMESIZE-1)==-1){
    throw SystemException(ERRCODE_SYSTEM, "Cannot get hostname to check process");
  }
}

IMSVishnuTool::~IMSVishnuTool(){
}

void 
IMSVishnuTool::run(){
  // Connexion to the LCT to get messages
  mLCT = LogORBMgr::getMgr()->resolve<LogCentralTool, LogCentralTool_ptr>("LogServiceT", "LCT");
  if (CORBA::is_nil(mLCT)){
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
    IMS_Data::Process_ptr p;
    // componentName
    log.append(msg[i].componentName);
    log.append(":");
    // If a connexion message arrive
    if (string(msg[i].tag).compare("IN")==0){
      string hname = getHostnameFromLog(string(msg[i].msg));
      // If it is on the same machine as the sed
      if (hname.compare(string(msyshName))){
	log += "Connexion of the component with the name : " + string(msg[i].componentName) + " and the message ->" + string(msg[i].msg) + "<- ";
	p->setProcessName(string(msg[i].componentName));
	try{
	  p->setMachineId(mproc.getMidFromHost(msyshName));
	}catch(IMSVishnuException& e){
	  log.append("Machine " + string(msg[i].componentName) + " is not registered as a server");
	  ofstream dest(mfilename.c_str(),ios::app);
	  dest << log << endl;
	  dest.close();
	  continue;
	}
	try {
	  mproc.connectProcess(p);
	}catch ( SystemException& e){
	  throw (e);
	}
	ofstream dest(mfilename.c_str(),ios::app);
	dest << log << endl;
	dest.close();
      }
    }
    // If message of disconnexion
    if (string(msg[i].tag).compare("OUT")==0){
      string hname = getHostnameFromLog(string(msg[i].msg));
      // If it is on the same machine as the sed
      if (hname.compare(string(msyshName))){
	log = "Disconnexion of the component with the name : " + string(msg[i].componentName);
	p->setProcessName(string(msg[i].componentName));
	try{
	  mproc.disconnectProcess(p);
	} catch(SystemException& e){
	  throw (e);
	}// end catch
      }// end if same machine
      // If disconnexion of an IMS sed on an other machine, must set him as out
      else if (mproc.isIMSSeD(string(msg[i].componentName))){


	/*******************************************************************************/
	// TODO ELECT PROCESS

	// If not stopped voluntarily

	// SSH and reconnect
	/*******************************************************************************/

      }
    }// end if disconnexion
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
	log.append(string(msg[i].msg)+", with type UMS");
	p->setMachineId(mproc.getMidFromHost(msyshName));
	p->setDietId(string(msg[i].componentName));
	p->setProcessName("UMS");
	mproc.authentifyProcess(p);
	//	cout << "The SeD " << msg[i].componentName << "is of type UMS" << endl;
      }
      else if ((string(msg[i].msg)).find("jobSubmit")!=string::npos) {
	log.append(msg[i].tag);
	log.append(":");
	//msg
	log.append(string(msg[i].msg)+", with type TMS");
	p->setMachineId(mproc.getMidFromHost(msyshName));
	p->setDietId(string(msg[i].componentName));
	p->setProcessName("TMS");
	mproc.authentifyProcess(p);
	//	cout << "The SeD " << msg[i].componentName << " is of type TMS" << endl;
      }// End else if submit machine
      else if (string(msg[i].msg).compare("int_getProcesses")==0) {
	log.append(msg[i].tag);
	log.append(":");
	//msg
	log.append(string(msg[i].msg)+", with type TMS");
	p->setMachineId(mproc.getMidFromHost(msyshName));
	p->setDietId(string(msg[i].componentName));
	p->setProcessName("IMS");
	mproc.authentifyProcess(p);
	//	cout << "The SeD " << msg[i].componentName << " is of type IMS" << endl;
      } // End else if get processes
    }// End if not IN tag
    ofstream dest(mfilename.c_str(),ios::app);
    dest << log << endl;
    dest.close();
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

string
IMSVishnuTool::getHostnameFromLog(string msg){
  // Extracting hostname from msg
  int pos = msg.find_last_of(' ');
  return msg.substr(pos+1);
}

