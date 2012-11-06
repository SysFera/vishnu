#include "ProcessCtl.hpp"
#include "IMSVishnuException.hpp"

#include <cstdlib>
#include <string>
#include <iostream>
#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

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
  } catch (SystemException e){
  }
  RPCCall(op, "supervisor.startProcess");
}

void
ProcessCtl::stop(IMS_Data::SupervisorOp_ptr op) {
  int res;
  string name;
  mop = *op;
  if (!muser.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "stop is an admin function. A user cannot call it");
  }
  RPCCall(op, "supervisor.stopProcessGroup");
}

void
ProcessCtl::RPCCall( IMS_Data::SupervisorOp_ptr op, std::string methodName){
  string const serverUrl(op->getURI()+"/RPC2");
  xmlrpc_c::clientSimple myClient;
  xmlrpc_c::value result;
  try{
    myClient.call(serverUrl, methodName, "s", &result, std::string(mop.getName()).c_str());
  } catch (girerr::error e){
    throw SystemException(ERRCODE_SYSTEM, std::string("Error in RPC call: ")+std::string(e.what()));
  }
}


void
ProcessCtl::loadShed(int type, IMS_Data::SupervisorOp_ptr op) {
  // Bad call type, 1 = HARD => STOP ALL
  if (type != 1) {
    return;
  }
  // Physically stopping them
  stopAll(op);
}

void
ProcessCtl::stopAll(IMS_Data::SupervisorOp_ptr op) {
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
    op->setName(p->getProcessName());
    // If ims, close at the end
    if (p->getProcessName().compare("imssed")==0) {
      ims.push_back(p);
    } else {
      try {
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

//void
//ProcessCtl::list() {
//  zmq::context_t ctx(1);
//  LazyPirateClient lpc(ctx, uri);
//
//  if (!lpc.send("")) {
//    std::cerr << "E: request failed, exiting ...\n";
//    exit(-1);
//  }
//
//  std::string response = lpc.recv();
//  std::cout << "response received: ->" << response << "<- ," << response.length() <<  "\n";
//  if (0 == response.length()) {
//      throw SystemException(ERRCODE_SYSTEM, "No corresponding server found");
//  }
//  int precDol = response.find("$");
//  std::string server;
//  int tmp;
//  while (precDol != std::string::npos) {
//    tmp = response.find("$", precDol+1);
//    if(tmp != std::string::npos){
//      server = response.substr(precDol+1, tmp-precDol-1);
//    } else {
//      server = response.substr(precDol+1, std::string::npos);
//    }
//    precDol = tmp;
//
//    std::string nameServ;
//    std::string addr;
//    int prec;
//    std::vector< std::string> vec;
//
//    tmp = server.find("#", 0);
//    prec = tmp;
//    nameServ = server.substr(0, tmp);
//    tmp = server.find("#", prec+1);
//    addr = server.substr(prec+1, tmp-prec-1);
//
//    boost::shared_ptr<Server> s =boost::shared_ptr<Server>(new Server(nameServ, vec, addr));
//    serv.push_back(s);
//  }
//}
