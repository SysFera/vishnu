#include "ProcessCtl.hpp"

ProcessCtl::ProcessCtl(string mid): mmid(mid),
				    mp(UserServer(SessionServer(""))) {
}

ProcessCtl::~ProcessCtl() {
}

void
ProcessCtl::restart() {
  string type;
//  switch(op.getSeDType()) {
//  case 1 : // UMS
//    type = "UMS";
//    break;
//  case 2 : // TMS
//    type = "TMS";
//    break;
//  case 3 : // FMS
//    type = "FMS";
//    break;
//  case 4 : // IMS
//    type = "IMS";
//    break;
//  default:
//    throw SystemException(ERRCODE_SYSTEM, "Unknown component to restart type");
//    break;
//  }
  // TODO : faire le SSH  pour executer la commande  

}

void
ProcessCtl::stop() {
}

void
ProcessCtl::loadShed() {

}
