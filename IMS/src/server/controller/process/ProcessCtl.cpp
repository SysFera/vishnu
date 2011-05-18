#include "ProcessCtl.hpp"

ProcessCtl::ProcessCtl(string mid): mmid(mid),
				    mp(UserServer(SessionServer(""))) {
}

ProcessCtl::~ProcessCtl() {
}

void
ProcessCtl::restart() {
  string type;
  IMS_Data::Process_ptr proc;
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
  proc->setProcessName(type);
  proc->setMachineId(mmid);
  mp.fillContent(proc);
  // TODO : faire le SSH  pour executer la commande  

}

void
ProcessCtl::stop(IMS_Data::Process_ptr p) {
  string name;
  // If deleting a specific process
  if (p->getProcessName().compare("")!=0) {
    name = p->getProcessName();
    boost::to_lower(name);
    name += "sed";
    try {
      // Setting to off in DB
      mp.stopProcess(p);
    } catch (SystemException& e) {
      throw (e);
    }
  } 
  // Else, kill all sed on the machine
  else { 
    name = "-r .*sed";
    try {
      // Setting to off in DB
      mp.stopAllProcesses(p);
    } catch (SystemException& e) {
      throw (e);
    }
  }
  string cmd = "killall -9 "+name;
  // TODO executer la commande sur la machine distante (ou pas)

  // if no machineid : local, else on the distant machine
  if (p->getMachineId().compare("")==0) {

  } else { // Distant stop

  }
}

void
ProcessCtl::loadShed() {

}
