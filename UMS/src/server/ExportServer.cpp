#include "ExportServer.hpp"
#include "DbFactory.hpp"

ExportServer::ExportServer(UserServer u):muser(u){
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
}

ExportServer::~ExportServer(){
}
