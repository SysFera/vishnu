#include "ShellExporter.hpp"

#include <string>
#include <vector>

#include "utilServer.hpp"
#include "IMSVishnuException.hpp"
#include "Mapper.hpp"
#include "UMSShellMapper.hpp"
#include "FMSShellMapper.hpp"
#include "TMSShellMapper.hpp"
#include "IMSShellMapper.hpp"

ShellExporter::ShellExporter(UserServer u, IMS_Data::ExportOp_ptr op):ExportServer(u, *op) {
  UMSShellMapper *mmapperUMS;
  TMSShellMapper *mmapperTMS;
  FMSShellMapper *mmapperFMS;
  IMSShellMapper *mmapperIMS;
  /*Mapper registration*/
  mmapperTMS = new TMSShellMapper(MapperRegistry::getInstance(), TMSSHELLMAPPERNAME);
  mmapperTMS->registerMapper();
  mmapperIMS = new IMSShellMapper(MapperRegistry::getInstance(), IMSSHELLMAPPERNAME);
  mmapperIMS->registerMapper();
  mmapperFMS = new FMSShellMapper(MapperRegistry::getInstance(), FMSSHELLMAPPERNAME);
  mmapperFMS->registerMapper();
  mmapperUMS = new UMSShellMapper(MapperRegistry::getInstance(), UMSSHELLMAPPERNAME);
  mmapperUMS->registerMapper();
}

ShellExporter::~ShellExporter() {
}

int
ShellExporter::exporte(std::string oldSession, std::string &content){
  std::vector<std::string>::iterator iter;
  std::vector<std::string> line;

  // Init the script
  content = "#!/bin/sh";
  content += " \n";

  // Check the user is alloed to export
  if (!muser.isAdmin() && !isAllowed(oldSession, muser)){
    throw IMSVishnuException(ERRCODE_INVEXPORT, "The user is not allowed to export this session");
  }

  // Check the session is closed
  if (!isClosed(oldSession)) {
    throw IMSVishnuException(ERRCODE_INVEXPORT, "The session id is invalid");
  }

  // The request, ordered by starttime (=submission)
  std::string req = "SELECT command.ctype, command.description, command.starttime from "
    " command, vsession where vsession.numsessionid=command.vsession_numsessionid and "
    " vsession.vsessionid='"+mdatabase->escapeData(oldSession)+"' order by starttime asc";

  boost::scoped_ptr<DatabaseResult> result (mdatabase->getResult(req.c_str()));

  // Adding all the results to the content
  for (size_t i = 0 ; i<result->getNbTuples(); i++) {
    line.clear();
    line = result->get(i);
    iter = line.begin();
    //MAPPER CREATION
    try {
      vishnu::CmdType type = static_cast<vishnu::CmdType>(vishnu::convertToInt(*iter));
      Mapper* mapper = MapperRegistry::getInstance()->getMapper(getMapperName(type));
    content += mapper->decode(*(++iter));
    } catch (SystemException &e) {
      throw (e);
    }
    content += " \n";
  }

  return 0;
}

bool
ShellExporter::isClosed(std::string sid) {
  bool res = false;
  if (sid.size() < 1) {
    return res;
  }
  std::string req = "select * from vsession where vsessionid ='"+mdatabase->escapeData(sid)+"' and state='0'";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    res = (result->getNbTuples()>0);
  } catch(SystemException &e) {
    return res;
  }
  return res;
}

bool
ShellExporter::isAllowed(std::string oldSession, UserServer muser) {
  bool res = false;
  std::string req = "select * from vsession, users where vsession.vsessionid='"+mdatabase->escapeData(oldSession)+"' and vsession.users_numuserid=users.numuserid and users.userid='"+mdatabase->escapeData(muser.getData().getUserId())+"'";
  try {
    boost::scoped_ptr<DatabaseResult> result(mdatabase->getResult(req.c_str()));
    res = (result->getNbTuples()>0);
  } catch(SystemException &e) {
    return res;
  }
  return res;
}

std::string
ShellExporter::getMapperName(int type) {
  std::string shellMapperName;
  switch (type) {
  case vishnu::UMS:
     shellMapperName = UMSSHELLMAPPERNAME;
     break;
  case vishnu::TMS:
     shellMapperName = TMSSHELLMAPPERNAME;
     break;
  case vishnu::FMS:
     shellMapperName = FMSSHELLMAPPERNAME;
     break;
  case vishnu::IMS:
     shellMapperName = IMSSHELLMAPPERNAME;
     break;
  default:
     shellMapperName = "UNKNOWNMAPPER";
     break;
  }

  if(shellMapperName.compare("UNKNOWNMAPPER")!=0) {
    return shellMapperName;
  } else {
    throw IMSVishnuException(ERRCODE_INVEXPORT, "Cannot find mapper");
  }
}
