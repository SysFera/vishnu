#include "CLICmd.hpp"
#include "UserException.hpp" 

CLICmd::CLICmd(int argc, char** argv, boost::shared_ptr<Options> op):margc(argc), margv(argv), mop(op){
}

CLICmd::~CLICmd(){
}

int
CLICmd::parse(const func1 & s){
  try{
    mop->parse_cli(margc,margv);
    mop->parse_env(s);
    mop->notify();
  } catch(po::required_option& e){//  a required parameter is missing
    return ERRCODE_CLI_ERROR_MISSING_PARAMETER;
  } catch(po::error& e){ // catch all other bad parameter errors
    return ERRCODE_INVALID_PARAM;
  }
  return VISHNU_OK;
}

