#include "internalApiIMS.hpp"
#include <exception>
#include "VishnuException.hpp"
#include "SystemException.hpp"
#include "IMSMapper.hpp"
#include "MapperRegistry.hpp"
#include <iostream>
#include "SessionServer.hpp"
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils
#include "IMS_Data.hpp"
#include "DIET_server.h"
#include "data/ProcessServer.hpp"
#include "data/MetricServer.hpp"

using namespace std;
using namespace vishnu;

int 
solveExport(diet_profile_t* pb){
  return 0;
}

// TODO FAIRE CE SERVICE COMME NOMME
int 
solveCurMetric(diet_profile_t* pb){
  char *sessionKey   = NULL;
  char *curOpSer = NULL;
  char *mid = NULL;
  char *curSer   = NULL;
  string error;
  string retErr = "";
  int mapperkey;
  string cmd;

  diet_string_get(diet_parameter(pb,0), &sessionKey,NULL);
  diet_string_get(diet_parameter(pb,1), &mid,NULL);
  diet_string_get(diet_parameter(pb,2), &curOpSer,NULL);

  SessionServer sessionServer = SessionServer(string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  try {
    userServer.init();
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(IMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_current_metric");
    mapper->code(std::string(mid), mapperkey);
    mapper->code(std::string(curOpSer), mapperkey);
    cmd = mapper->finalize(mapperkey);

    // Getting options
    IMS_Data::CurMetricOp_ptr curOp = NULL;
    if(!parseEmfObject(std::string(curOpSer), curOp)) {
      throw UserException(ERRCODE_INVALID_PARAM, "solve_getProcesses: Process option object is not well built");
    }
    // Creating the process server with the options
    MetricServer met(userServer, curOp);

    // Listing the old metric
    IMS_Data::ListMetric* res;
    res = met.getCurMet();
    // Serializing the results
    const char* name = "solve_getCurMetric";
    ::ecorecpp::serializer::serializer _ser(name);
    curSer = strdup(_ser.serialize(const_cast<IMS_Data::ListMetric_ptr>(res)).c_str());

    // Setting out diet param
    diet_string_set(diet_parameter(pb,3), curSer, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(retErr.c_str()), DIET_VOLATILE);

    // Finishing the command as a success
    sessionServer.finish(cmd, IMS, CMDSUCCESS);
  } catch (VishnuException& e){
    try{
      // Finishing the command as an error
      sessionServer.finish(cmd, IMS, CMDFAILED);
    }catch(VishnuException& fe){
      error = fe.what();
    }
    e.appendMsgComp(error);
    retErr = e.buildExceptionString();
    // Setting diet output parameters
    diet_string_set(diet_parameter(pb,3), strdup(""), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(retErr.c_str()), DIET_VOLATILE);

  }
  return 0;
}

int 
solveOldMetric(diet_profile_t* pb){
  char *sessionKey   = NULL;
  char *histOpSer = NULL;
  char *mid = NULL;
  char *histSer   = NULL;
  string error;
  string retErr = "";
  int mapperkey;
  string cmd;

  diet_string_get(diet_parameter(pb,0), &sessionKey,NULL);
  diet_string_get(diet_parameter(pb,1), &mid,NULL);
  diet_string_get(diet_parameter(pb,2), &histOpSer,NULL);

  SessionServer sessionServer = SessionServer(string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  try {
    userServer.init();
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(IMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_history_metric");
    mapper->code(std::string(mid), mapperkey);
    mapper->code(std::string(histOpSer), mapperkey);
    cmd = mapper->finalize(mapperkey);

    // Getting options
    IMS_Data::MetricHistOp_ptr histOp = NULL;
    if(!parseEmfObject(std::string(histOpSer), histOp)) {
      throw UserException(ERRCODE_INVALID_PARAM, "solve_getProcesses: Process option object is not well built");
    }
    // Creating the process server with the options
    MetricServer met(userServer, histOp);

    // Listing the old metric
    IMS_Data::ListMetric* res;
    res = met.getHistMet(string(mid));

    // Serializing the results
    const char* name = "solve_getHistMetric";
    ::ecorecpp::serializer::serializer _ser(name);
    histSer = strdup(_ser.serialize(const_cast<IMS_Data::ListMetric_ptr>(res)).c_str());

    // Setting out diet param
    diet_string_set(diet_parameter(pb,3), histSer, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(retErr.c_str()), DIET_VOLATILE);

    // Finishing the command as a success
    sessionServer.finish(cmd, IMS, CMDSUCCESS);

  } catch (VishnuException& e){
    try{
      // Finishing the command as an error
      sessionServer.finish(cmd, IMS, CMDFAILED);
    }catch(VishnuException& fe){
      error = fe.what();
    }
    e.appendMsgComp(error);
    retErr = e.buildExceptionString();
    // Setting diet output parameters
    diet_string_set(diet_parameter(pb,3), strdup(""), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,4), strdup(retErr.c_str()), DIET_VOLATILE);

  }
  return 0;
}

int 
solvePS(diet_profile_t* pb){
  char *sessionKey   = NULL;
  char *processOpSer = NULL;
  char *processSer   = NULL;
  string error;
  string retErr = "";
  int mapperkey;
  string cmd;

  diet_string_get(diet_parameter(pb,0), &sessionKey,NULL);
  diet_string_get(diet_parameter(pb,1), &processOpSer,NULL);

  SessionServer sessionServer = SessionServer(string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  try {
    userServer.init();
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(IMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_processes");
    mapper->code(std::string(processOpSer), mapperkey);
    cmd = mapper->finalize(mapperkey);

    // Getting options
    IMS_Data::ProcessOp_ptr processOp = NULL;
    if(!parseEmfObject(std::string(processOpSer), processOp)) {
      throw UserException(ERRCODE_INVALID_PARAM, "solve_getProcesses: Process option object is not well built");
    }

    // Creating the process server with the options
    ProcessServer proc(processOp, userServer);

    // Listing the processes
    IMS_Data::ListProcesses* res;
    res = proc.list();

    // Serializing the results
    const char* name = "solve_getProcesses";
    ::ecorecpp::serializer::serializer _ser(name);
    processSer = strdup(_ser.serialize(const_cast<IMS_Data::ListProcesses_ptr>(res)).c_str());

    // Setting out diet param
    diet_string_set(diet_parameter(pb,2), processSer, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(retErr.c_str()), DIET_VOLATILE);

    // Finishing the command as a success
    sessionServer.finish(cmd, IMS, CMDSUCCESS);

  }catch(VishnuException& e){
    try{
      // Finishing the command as an error
      sessionServer.finish(cmd, IMS, CMDFAILED);
    }catch(VishnuException& fe){
      error = fe.what();
    }
    e.appendMsgComp(error);
    retErr = e.buildExceptionString();
    // Setting diet output parameters
    diet_string_set(diet_parameter(pb,2), strdup(""), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,3), strdup(retErr.c_str()), DIET_VOLATILE);
  }

  return 0;
}

int 
solveSetSysInfo(diet_profile_t* pb){
  return 0;
}

int 
solveSetThreshold(diet_profile_t* pb){
  return 0;
}

int 
solveGetThreshold(diet_profile_t* pb){
  return 0;
}

int 
solveSetUID(diet_profile_t* pb){
  return 0;
}

int 
solveSetJID(diet_profile_t* pb){
  return 0;
}

int 
solveSetTID(diet_profile_t* pb){
  return 0;
}

int 
solveSetMID(diet_profile_t* pb){
  return 0;
}

int 
solveLoadShed(diet_profile_t* pb){
  return 0;
}

int 
solveSetUpFreq(diet_profile_t* pb){
  char *sessionKey   = NULL;
  char* freq = NULL;
  string error;
  string retErr = "";
  int mapperkey;
  string cmd;
  char *freqSer   = NULL;
  

  diet_string_get(diet_parameter(pb,0), &sessionKey,NULL);
  diet_string_get(diet_parameter(pb,1), &freq,NULL);

  SessionServer sessionServer = SessionServer(string(sessionKey));
  UserServer userServer = UserServer(sessionServer);
  try {
    userServer.init();
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(IMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_set_update_frequency");
    mapper->code(string(freq), mapperkey);
    cmd = mapper->finalize(mapperkey);

    // Creating the process server with the options
    MetricServer met(userServer);

    // Listing the old metric
    met.setUpFreq(convertToInt(string(freq)));
    // Setting out diet param
    diet_string_set(diet_parameter(pb,2), strdup(retErr.c_str()), DIET_VOLATILE);

    // Finishing the command as a success
    sessionServer.finish(cmd, IMS, CMDSUCCESS);

  }catch(VishnuException& e){
    try{
      // Finishing the command as an error
      sessionServer.finish(cmd, IMS, CMDFAILED);
    }catch(VishnuException& fe){
      error = fe.what();
    }
    e.appendMsgComp(error);
    retErr = e.buildExceptionString();
    // Setting diet output parameters
    diet_string_set(diet_parameter(pb,2), strdup(retErr.c_str()), DIET_VOLATILE);
  }
  return 0;
}

int 
solveGetUpFreq(diet_profile_t* pb){
  char *sessionKey   = NULL;
  string error;
  string retErr = "";
  int mapperkey;
  string cmd;
  char *freqSer   = NULL;

  diet_string_get(diet_parameter(pb,0), &sessionKey,NULL);

  SessionServer sessionServer = SessionServer(string(sessionKey));
  UserServer userServer = UserServer(sessionServer);

  try {
    userServer.init();
    //MAPPER CREATION
    Mapper *mapper = MapperRegistry::getInstance()->getMapper(IMSMAPPERNAME);
    mapperkey = mapper->code("vishnu_get_update_frequency");
    cmd = mapper->finalize(mapperkey);

    // Creating the process server with the options
    MetricServer met(userServer);

    // Listing the old metric
    int res;
    res = met.checkUpFreq();

    freqSer = new char[convertToString(res).size()+1];
    memcpy(freqSer, convertToString(res).c_str(), convertToString(res).size());

    // Setting out diet param
    diet_string_set(diet_parameter(pb,1), freqSer, DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,2), strdup(retErr.c_str()), DIET_VOLATILE);

    // Finishing the command as a success
    sessionServer.finish(cmd, IMS, CMDSUCCESS);
  } catch (VishnuException& e){
    try{
      // Finishing the command as an error
      sessionServer.finish(cmd, IMS, CMDFAILED);
    }catch(VishnuException& fe){
      error = fe.what();
    }
    e.appendMsgComp(error);
    retErr = e.buildExceptionString();
    // Setting diet output parameters
    diet_string_set(diet_parameter(pb,1), strdup(""), DIET_VOLATILE);
    diet_string_set(diet_parameter(pb,2), strdup(retErr.c_str()), DIET_VOLATILE);
  }
  return 0;
}

int 
solveRestart(diet_profile_t* pb){
  return 0;
}

int 
solveStop(diet_profile_t* pb){
  return 0;
}

int 
solveGetSysInfo(diet_profile_t* pb){
  return 0;
}

