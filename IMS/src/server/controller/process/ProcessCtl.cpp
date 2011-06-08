#include "ProcessCtl.hpp"
#include <omniORB4/CORBA.h>
#include "DIET_admin.h"
#include "IMSVishnuException.hpp"

ProcessCtl::ProcessCtl(string mid, UserServer user): mmid(mid),
						     mp(user),
                                                     muser(user){
}

ProcessCtl::~ProcessCtl() {
}

void
ProcessCtl::restart(IMS_Data::RestartOp_ptr op, bool isAPI) {
  string type;
  string cmd ;
  mop = *op;
  IMS_Data::Process proc;
  string keyPath;
  string login;
  string hostname;

  // If call made by the api check admin
  if (isAPI) {
    if (!muser.isAdmin()) {
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "restart is an admin function. A user cannot call it");
    }
    // Currently too much but kept
    mp.getSshKeyAndAcc(keyPath, login, mmid, muser.getData().getUserId(), hostname);
  } else {
    // Currently too much but kept
    mp.getAnAdmin(keyPath, login, mmid, hostname);
  }

  switch(mop.getSedType()) {
  case 1 : // UMS
    type = "UMS";
    break;
  case 2 : // TMS
    type = "TMS";
    break;
  case 3 : // FMS
    type = "FMS";
    break;
  case 4 : // IMS
    type = "IMS";
    break;
  default:
    throw SystemException(ERRCODE_SYSTEM, "Unknown component to restart, type "+string(convertToString(mop.getSedType()))+" is unknown");
    break;
  }
  proc.setProcessName(type);
  proc.setMachineId(mmid);
  proc.setScript(mop.getVishnuConf());

  // Keep blocks separated because no catch error when try to stop
  try {
    // Make sure the process is really not running on the machine
    stop(&proc);
  } catch (VishnuException &e) {
    // Do nothing, stop just to make sure the process is not running anymore    
  }

  char hname[200];
  gethostname(hname, 199);
  
  // If have to restart a local process, ssh will not be used
  if (proc.getMachineId().compare(getMidFromHost(string(hname)))==0) {
    proc.setMachineId("");
  }

  createFile (cmd, &proc);
  

  boost::to_lower(type);
  type += "sed";
  cmd += type + " /tmp/vishnu_restart";
  // If local
  if (proc.getMachineId().compare("")==0) {
    int ret = system(cmd.c_str());
    if (ret == -1) {
      throw SystemException(ERRCODE_SYSTEM, "Failed to restart process "+type);
    }
  } else {
    // TODO : faire le SSH  pour executer la commande
    string cmd = "ssh vishnu@"+hostname+" \""+cmd+"\"";
    int ret = system(cmd.c_str());
    if (ret == -1) {
      throw SystemException(ERRCODE_SYSTEM, "Failed to restart process "+type);
    }
  }
}

void
ProcessCtl::stop(IMS_Data::Process_ptr p) {
  int res;
  string name;
  if (!muser.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "stop is an admin function. A user cannot call it");
  }
  // If deleting a specific process
  if (p->getProcessName().compare("")!=0) {
    name = p->getProcessName();
    try {
      // Setting to off in DB
      mp.stopProcess(p);
      mp.fillContent(p);
    } catch (SystemException& e) {
      throw (e);
    }
  } 
  // Diet admin api, remove a sed from hierarchy and stop it
  res = diet_remove_from_hierarchy(SED, p->getDietId().c_str(), false);
  if (res != DIET_NO_ERROR) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid remove with error code: "+convertToString(res));
  }
}

void
ProcessCtl::loadShed(int type) {
  // Bad call type, 1 = HARD => STOP ALL
  if (type != 1) {
    return;
  }
  // Setting processes as stopped in the database
  IMS_Data::Process_ptr p = new IMS_Data::Process();
  p->setMachineId(mmid);
  mp.stopAllProcesses(p);
  // Physically stopping them
  stopAll();
}

void
ProcessCtl::stopAll() {
  vector<IMS_Data::Process_ptr> ims;
  if (mmid.compare("")==0) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid empty machine id");
  }
  IMS_Data::ProcessOp processOp;// = ecoreFactory->createProcessOp();
  processOp.setMachineId(mmid);
  // Creating the process server with the options
  ProcessServer proc(&processOp, muser);

  // Listing all the processes on the machine to stop
  IMS_Data::ListProcesses* res;
  res = proc.list();

  if (res->getProcess().size() == 0) {
    throw SystemException(ERRCODE_SYSTEM, "No process found on machine: "+mmid+". ");
  }

  // Closing each process gotten
  for (unsigned int i = 0; i < res->getProcess().size(); i++) {
    IMS_Data::Process_ptr p = res->getProcess().get(i);
    // If ims, close at the end
    if (p->getProcessName().compare("IMS")==0) {
      ims.push_back(p);
    } else {
      try {
	stop(p);
      }catch(VishnuException& e) {
	// Do nothing, keep on removing other sed
      }
    }
  }
  // Closing all ims sed
  for (unsigned int i = 0 ; i < ims.size() ; i++) {
    try {
      stop(ims.at(i));
    } catch (VishnuException &e) {
	// Do nothing, keep on removing other sed
    }
  }

}


void 
ProcessCtl::createFile(string& cmd, IMS_Data::Process_ptr p) {
  cmd = "echo \""+p->getScript()+"\" > /tmp/vishnu_restart; ";
}

