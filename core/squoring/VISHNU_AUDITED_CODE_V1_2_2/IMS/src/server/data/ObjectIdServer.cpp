#include "ObjectIdServer.hpp"
#include "DbFactory.hpp"

ObjectIdServer::ObjectIdServer(const UserServer session):msession(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mvishnuId  = 1;
}

ObjectIdServer::~ObjectIdServer() {
}

void
ObjectIdServer::setUID(string fmt) {
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "define user id  format is an admin function. A user cannot call it");
  }
  if (!containCpt(fmt)) {
    throw UserException(10, "Invalid format, it does not contain any counter. ");
  }
  if (fmt.find_first_of('@')!=string::npos) {
    throw UserException(10, "Invalid format, it cannot contain the @ character. ");
  }
  string request = "update  vishnu set  formatiduser ='"+fmt+"' where  vishnuid ='";
  request += convertToString(mvishnuId);
  request += "'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp("Failed to set the user format to "+fmt);
    throw(e);
  }
}

void
ObjectIdServer::setFID(string fmt) {
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "define file transfer id  format is an admin function. A user cannot call it");
  }
  if (!containCpt(fmt)) {
    throw UserException(10, "Invalid format, it does not contain any counter. ");
  }
  if (fmt.find_first_of('@')!=string::npos) {
    throw UserException(10, "Invalid format, it cannot contain the @ character. ");
  }
  string request = "update  vishnu set  formatidfiletransfer ='"+fmt+"' where  vishnuid ='";
  request += convertToString(mvishnuId);
  request += "'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp("Failed to set the file format to "+fmt);
    throw(e);
  }
}

void
ObjectIdServer::setTID(string fmt) {
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "define task id  format is an admin function. A user cannot call it");
  }
  if (!containCpt(fmt)) {
    throw UserException(10, "Invalid format, it does not contain any counter. ");
  }
  if (fmt.find_first_of('@')!=string::npos) {
    throw UserException(10, "Invalid format, it cannot contain the @ character. ");
  }
  string request = "update  vishnu set  formatidjob ='"+fmt+"' where  vishnuid ='";
  request += convertToString(mvishnuId);
  request += "'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp("Failed to set the task format to "+fmt);
    throw(e);
  }
}

void
ObjectIdServer::setMID(string fmt) {
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "define machine id  format is an admin function. A user cannot call it");
  }
  if (!containCpt(fmt)) {
    throw UserException(10, "Invalid format, it does not contain any counter. ");
  }
  if (fmt.find_first_of('@')!=string::npos) {
    throw UserException(10, "Invalid format, it cannot contain the @ character. ");
  }
  string request = "update  vishnu set  formatidmachine ='"+fmt+"' where  vishnuid='";
  request += convertToString(mvishnuId);
  request += "'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp("Failed to set the machine format to "+fmt);
    throw(e);
  }
}

bool
ObjectIdServer::containCpt(string fmt) {
  return (fmt.find("$CPT")!=string::npos);
}

