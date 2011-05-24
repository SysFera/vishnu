#include "ExportServer.hpp"
#include "DbFactory.hpp"

ExportServer::ExportServer(UserServer u, IMS_Data::ExportOp op):muser(u), mop(op){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

ExportServer::~ExportServer(){
}
