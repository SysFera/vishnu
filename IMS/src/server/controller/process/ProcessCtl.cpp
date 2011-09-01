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

bool
ProcessCtl::isIMSSeD(string Pname) {
  return mp.isIMSSeD(Pname);
}

void
ProcessCtl::restart(IMS_Data::RestartOp_ptr op, string machineTo, bool isAPI) {
  string type;
  string cmd ;
  mop = *op;
  IMS_Data::Process proc;
  string login;
  string hostname;

  // If call made by the api check admin
  if (isAPI) {
    if (!muser.isAdmin()) {
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "restart is an admin function. A user cannot call it");
    }
  }

  // Getting the distants parameters (host and login)
  try  {
    mp.getHost(machineTo, hostname, login);
  } catch (VishnuException &e) {
    cerr << e.what() << endl;
    return;
  }

  // Converting to the stringified type of sed
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

  // Creating the process
  proc.setProcessName(type);
  proc.setMachineId(machineTo);
  proc.setScript(mop.getVishnuConf());

  // If process not to be restarted (when stopped vith a vishnu_stop vall before)
  if (mp.checkStopped(machineTo, type)) {
    throw SystemException(ERRCODE_SYSTEM, "No sed of type "+type+" running or down on machine "+machineTo+", cannot restart it ");
  }


  // Keep blocks separated because no catch error when try to stop
  try {
    // Make sure the process is really not running on the machine
    stop(&proc);
  } catch (VishnuException &e) {
    // Do nothing, stop just to make sure the process is not running anymore    
  }

  boost::to_lower(type);
  type += "sed";

  // The sequence of commands :
  // * Create vishnu conf script locally
  // * Set the rights to 777
  // * Copy on the machine to restart
  // * Create the script to launch the sed
  // * Set the rights to 777
  // * Copy on the machine to restart
  // * SSH and exec the script launching the sed with the generated conf file
  
  // This complicated scheme is used because a fork duplicate the sockets, and a fork exec would kill the database connexions of the sed
  
  // This part of the code is dirty but functionnal, make it better when it will be possible
  // TODO clean the code using the diet syntax with the | ssh -q
  string tmp = "";
  tmp = " echo \" ";
  tmp += proc.getScript();
  tmp += "\" > /tmp/vishnu_restart ; ";
  tmp += " chmod 777 /tmp/vishnu_restart; ";
  tmp += " scp /tmp/vishnu_restart "+login+"@"+hostname+":/tmp ;";
  tmp += " echo \"#!/bin/sh \\nsource ~/.bashrc ; \\nnohup "+type+" /tmp/vishnu_restart 1>/dev/null 2>/dev/null </dev/null  & \" > /tmp/script.sh; ";
  tmp += " chmod 777 /tmp/script.sh; ";
  tmp += " scp /tmp/script.sh "+login+"@"+hostname+":/tmp ;";
  tmp += " ssh "+login+"@"+hostname+" \"/tmp/script.sh \";";

  string dcmd = tmp;
  system(dcmd.c_str());
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
  // Physically stopping them
  stopAll();
  mp.stopAllProcesses(p);
}

void
ProcessCtl::stopAll() {
  vector<IMS_Data::Process_ptr> ims;
  if (mmid.compare("")==0) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid empty machine id");
  }
  IMS_Data::ProcessOp processOp;
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

