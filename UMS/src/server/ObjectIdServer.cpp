#include "ObjectIdServer.hpp"

#include <string>
#include <boost/algorithm/string.hpp>

#include "DbFactory.hpp"

ObjectIdServer::ObjectIdServer(const UserServer session):msession(session) {
  DbFactory factory;
  mdatabase = factory.getDatabaseInstance();
  mvishnuId  = 1;
}

ObjectIdServer::~ObjectIdServer() {
}


/**
 * \brief To set the format of an entry
 */
void
ObjectIdServer::setformat(std::string fmt, std::string entry){
  if (!msession.isAdmin()){
    throw UMSVishnuException(ERRCODE_NO_ADMIN, "define "+entry+ "id  format is an admin function. A user cannot call it");
  }
  boost::trim(entry);
  if (fmt.find_first_of('@')!=std::string::npos) {
    throw UserException(10, "Invalid format, it cannot contain the @ character. ");
  }
  std::string request = "update  vishnu set  formatid"+entry+" ='"+mdatabase->escapeData(fmt)+"' where  vishnuid ='";
  request += vishnu::convertToString(mvishnuId);
  request += "'";
  try{
    mdatabase->process(request.c_str());
  }catch(SystemException& e){
    e.appendMsgComp("Failed to set the "+entry+" format to "+fmt);
    throw(e);
  }
}



bool
ObjectIdServer::containCpt(std::string fmt) {
  return (fmt.find("$CPT")!=std::string::npos);
}
