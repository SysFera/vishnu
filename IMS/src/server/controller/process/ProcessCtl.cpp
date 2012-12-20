#include "ProcessCtl.hpp"
#include "IMSVishnuException.hpp"
#include "IMS_Data.hpp"
#include "IMS_Data_forward.hpp"

#include <cstdlib>
#include <string>
#include <iostream>
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

ProcessCtl::ProcessCtl(string mid, UserServer user): mp(user),
                                                     mmid(mid),
                                                     muser(user){
}

ProcessCtl::~ProcessCtl() {
}

bool
ProcessCtl::isIMSSeD(string Pname) {
  return mp.isIMSSeD(Pname);
}

void
ProcessCtl::restart(IMS_Data::SupervisorOp_ptr op, string machineTo, bool isAPI) {
  string type;
  mop = *op;
  if (isAPI) {
    if (!muser.isAdmin()) {
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "restart is an admin function. A user cannot call it");
    }
  }

// Don't care if unable to stop -> means already stopped
  try{
    stop(op);
  } catch (SystemException& e){
  }
  IMS_Data::ProcessOp processOp;
  processOp.setMachineId(mmid);
  ProcessServer procs(&processOp, muser);
  // Getting URI
  mop.setURI(procs.getURI());

  RPCCall("supervisor.startProcess");

  // Updating BDD (case of success no exception thrown)
  IMS_Data::Process proc;
  proc.setProcessName(mop.getName());
  proc.setMachineId(mmid);
  procs.setRestarted(&proc);
}

void
ProcessCtl::stop(IMS_Data::SupervisorOp_ptr op) {
  string name;
  mop = *op;
  if (!muser.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "stop is an admin function. A user cannot call it");
  }
  IMS_Data::ProcessOp processOp;
  processOp.setMachineId(mmid);
  ProcessServer procs(&processOp, muser);
  // Getting URI
  mop.setURI(procs.getURI());

  RPCCall("supervisor.stopProcessGroup");

  // Updating BDD (case of success no exception thrown)
  IMS_Data::Process proc;
  proc.setProcessName(mop.getName());
  proc.setMachineId(mmid);
  procs.stopProcess(&proc);
}

void
ProcessCtl::RPCCall(std::string methodName){
  string const serverUrl(mop.getURI()+"/RPC2");
  xmlrpc_c::clientSimple myClient;
  xmlrpc_c::value result;
  try{
    std::cout << "url =" << serverUrl << std::endl;
    std::cout << "name =" << methodName << std::endl;
    std::cout << "param =" << mop.getName() << std::endl;

    myClient.call(serverUrl, methodName, "s", &result, std::string(mop.getName()).c_str());
  } catch (girerr::error& e){
    throw SystemException(ERRCODE_SYSTEM, std::string("Error in RPC call: ")+std::string(e.what()));
  }
}


void
ProcessCtl::loadShed(int type) {
  // Bad call type, 1 = HARD => STOP ALL
  if (type != 1) {
    return;
  }
  // Physically stopping them
  stopAll();
}

void
ProcessCtl::stopAll() {
  vector<IMS_Data::Process_ptr> ims;
  if (mmid.compare("")==0) {
    throw SystemException(ERRCODE_SYSTEM, "Invalid empty machine id");
  }
  IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
  IMS_Data::SupervisorOp_ptr op = ecoreFactory->createSupervisorOp();
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

  std::cout << "Number of process " << res->getProcess().size() << std::endl;

  // Closing each process gotten
  for (unsigned int i = 0; i < res->getProcess().size(); i++) {
    IMS_Data::Process_ptr p = res->getProcess().get(i);
    // If ims, close at the end
    if (p->getProcessName().compare("imssed")==0) {
      std::cout << "Is IMS " << std::endl;
      ims.push_back(p);
    } else {
      try {
        op->setName(p->getProcessName());
        std::cout << "Not IMS, it is : " << op->getName() << std::endl;
	stop(op);
      }catch(VishnuException& e) {
	// Do nothing, keep on removing other sed
      }
    }
  }
  // Closing all ims sed
  for (unsigned int i = 0 ; i < ims.size() ; i++) {
    try {
      op->setName("imssed");
      stop(op);
    } catch (VishnuException &e) {
	// Do nothing, keep on removing other sed
    }
  }

}
