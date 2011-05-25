#include "ProcessCtl.hpp"
#include <omniORB4/CORBA.h>
#include "DIET_admin.h"

ProcessCtl::ProcessCtl(string mid, UserServer user): mmid(mid),
						     mp(user) {
}

ProcessCtl::~ProcessCtl() {
}

void
ProcessCtl::restart(IMS_Data::RestartOp_ptr op) {
  string type;
  string cmd ;
  string dest;
  mop = *op;
  IMS_Data::Process proc;
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
    throw SystemException(ERRCODE_SYSTEM, "Unknown component to restart type");
    break;
  }
  proc.setProcessName(type);
  proc.setMachineId(mmid);
  proc.setScript(mop.getVishnuConf());
  //  mp.fillContent(&proc);

  char hname[200];
  gethostname(hname, 199);
  
  // If have to stop a local process 
  if (proc.getMachineId().compare(getMidFromHost(string(hname)))==0) {
    proc.setMachineId("");
  }

  // Make sure the process is really not running on the machine
  stop(&proc);

  //  createFile (dest, &proc);
  dest = proc.getScript();

  boost::to_lower(type);
  type += "sed";
  cmd = type + " " + dest;
  // If local
  if (proc.getMachineId().compare("")==0) {
    int ret = system(cmd.c_str());
    if (ret == -1) {
      throw SystemException(ERRCODE_SYSTEM, "Failed to restart process "+type);
    }
  } else {
    // TODO : faire le SSH  pour executer la commande  
  }
}

// TODO Fix dirty construction
void
ProcessCtl::stop(IMS_Data::Process_ptr p) {
  int res;
  string name;
  // If deleting a specific process
  if (p->getProcessName().compare("")!=0) {
    name = p->getProcessName();
    try {
      // Setting to off in DB
      mp.stopProcess(p);
    } catch (SystemException& e) {
      throw (e);
    }
  } 
  cout << "process name: " << p->getProcessName() << endl;
  cout << "process name: " << p->getDietId() << endl;
  mp.fillContent(p);

  res = diet_remove_from_hierarchy(SED, p->getDietId().c_str(), false);
  if (res != DIET_NO_ERROR) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid "+convertToString(res));
  }
}

void
ProcessCtl::loadShed(int type) {

}


void 
ProcessCtl::createFile(string& dest, IMS_Data::Process_ptr p) {
  dest = "/tmp/vishnu_restart";
  string cmd = "echo \""+p->getScript()+"\" > "+dest;
  int res = system(cmd.c_str());
  if (res == -1) {
    throw SystemException(ERRCODE_SYSTEM, "Error creating restart file");
  }
}

