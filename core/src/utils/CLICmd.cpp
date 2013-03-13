#include "CLICmd.hpp"

#include "VishnuException.hpp"          // for VISHNU_OK
#include "UserException.hpp"

namespace boost { namespace program_options { class error; } }
namespace boost { namespace program_options { class required_option; } }


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
