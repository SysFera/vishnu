#include "CLICmd.hpp"


CLICmd::CLICmd(int argc, char** argv, boost::shared_ptr<Options> op, string dietConfig):margc(argc), margv(argv), mdietConfig(dietConfig), mop(op){
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
    return CLI_ERROR_MISSING_PARAMETER;
  } catch(po::error& e){ // catch all other bad parameter errors
    return CLI_ERROR_INVALID_PARAMETER;
  }
  return CLI_SUCCESS;
}

