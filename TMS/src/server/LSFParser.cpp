#include <unistd.h>
#include <getopt.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <boost/algorithm/string.hpp>
#include "TMSVishnuException.hpp"
#include "UMSVishnuException.hpp"
#include "utilVishnu.hpp"

#include "LSFParser.hpp"
#include <lsf/lsbatch.h>

using namespace std;

static struct option long_options[] = {
         {"We", required_argument, 0, LONG_OPT_WE},
         {"rn", no_argument, 0, LONG_OPT_RN},
         {"app", required_argument, 0, LONG_OPT_APP},
         {"cwd", required_argument, 0, LONG_OPT_CWD},
         {"ul", required_argument, 0, LONG_OPT_UL},
         {"Jd", required_argument, 0, LONG_OPT_JD},
         {"is", required_argument, 0, LONG_OPT_IS},
         {"eo", required_argument, 0, LONG_OPT_EO},
         {"oo", required_argument, 0, LONG_OPT_OO},
         {"ar", no_argument, 0, LONG_OPT_AR},
         {"wa", required_argument, 0, LONG_OPT_WA},
         {"wt", required_argument, 0, LONG_OPT_WT},
         {"Zs", no_argument, 0, LONG_OPT_ZS},
         {"Ep", required_argument, 0, LONG_OPT_EP},
         {"sp", required_argument, 0, LONG_OPT_SP},
         {"mig", required_argument, 0, LONG_OPT_MIG},
         {"sla", required_argument, 0, LONG_OPT_SLA},
         {"ext", required_argument, 0, LONG_OPT_EXT},
         {"Lp", required_argument, 0, LONG_OPT_LP},
         {"jsdl", required_argument, 0, LONG_OPT_JSDL},
         {"jsdl_strict", required_argument, 0, LONG_OPT_JSDL_STRICT},
         {"rnc", required_argument, 0, LONG_OPT_RNC},
         {"XF", no_argument, 0, LONG_OPT_XF}
};



/**
 * \brief Constructor
 */
LSFParser::LSFParser(){
}

bool LSFParser::isNumerical(const std::string& value) {

  if(value.size()==0) {
   return false;
  }
  bool ret = (value.find_first_not_of("0123456789")==std::string::npos);
  if(!ret) {
   return false;
  }

  return true;
}

int 
LSFParser::parse_file(const char* pathTofile, struct submit* req) {

 std::string LSF_PREFIX="#BSUB";
 ifstream fileStream;
 string line;

  fileStream.open (pathTofile);

  static std::map<size_t, pair<string,string> > tab;
  std::string cmd;
  vector<string> tokens;
  if (fileStream.is_open()) {
    while (!fileStream.eof()) {
      getline(fileStream, line);
      size_t pos = line.find('#');
      if(pos==string::npos) {
        continue;
      }
      line = line.erase(0, pos);
      if(boost::algorithm::starts_with(line, LSF_PREFIX)){
         line = line.substr(std::string(LSF_PREFIX).size());
         cmd = cmd+" "+line;
       }  
    }
    fileStream.close();

    std::istringstream iss(cmd);
    std::copy(istream_iterator<string>(iss),
              istream_iterator<string>(),
              back_inserter<vector<string> >(tokens));

  }
  
  int argc = tokens.size()+1;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  std::cout << "**********cmd=" << argv[0] << " ";
  for(int i=0; i < tokens.size(); i++) {
    argv[i+1] = const_cast<char*>(tokens[i].c_str());
    std::cout << argv[i+1] << " ";
  }
  std::cout << std::endl;

 #define GETOPT_ARGS "J:q:m:n:i:o:e:xNBG:k:rw:R:E:L:P:u:U:K:Wg:Hc:F:p:M:D:S:v:T:b:t:f:Q"
 
 int option_index = 0;
 int c;
 std::string host_list;
 std::vector<std::string> host_tokens;
 std::istringstream stream_host_list;
 std::string procsStr;
 std::istringstream stream_procs;
 std::string min_proc_str;
 std::string max_proc_str;
 int min_proc;
 int max_proc;
 char separator;
 while ((c = getopt_long_only(argc, argv, GETOPT_ARGS, long_options, &option_index)) != EOF) {
   switch (c) {
     case 'J':
        req->options |=SUB_JOB_NAME;
        req->jobName = strdup(optarg);
        break;
     case 'q':
        req->options |=SUB_QUEUE;
        req->queue = strdup(optarg);
        break;
     case 'm':
        req->options |=SUB_HOST;
        host_list = strdup(optarg);
        stream_host_list.str(host_list);
        host_tokens.clear();
        std::copy(istream_iterator<string>(stream_host_list),
              istream_iterator<string>(),
              back_inserter<vector<string> >(host_tokens));
        req->numAskedHosts = host_tokens.size();
        req->askedHosts = new char*[host_tokens.size()];
        for(int i=0; i < tokens.size(); i++) {
              req->askedHosts[i] = strdup(tokens[i].c_str());
              std::cout << "***********host" << i << " is " << req->askedHosts[i] << std::endl;
        }
        break;
     case 'n':
         procsStr = strdup(optarg);
         if(procsStr.find(',')!=std::string::npos) {
            stream_procs.str(procsStr);
            stream_procs >> min_proc_str;
            stream_procs >> separator;
            stream_procs >> max_proc_str;
            if(!isNumerical(min_proc_str) || !isNumerical(max_proc_str)){
              std::cerr << optarg << " is an invalid value for -n option" << std::endl;
            }
            min_proc = vishnu::convertToInt(min_proc_str);
            max_proc = vishnu::convertToInt(max_proc_str);
          } else {
            if(isNumerical(procsStr)) {
             min_proc = vishnu::convertToInt(procsStr);
             max_proc = min_proc;
            } else {
             std::cerr << optarg << " is an invalid value for -n option" << std::endl;
             return -1;
            }   
          }
          req->numProcessors=min_proc;
          req->maxNumProcessors=max_proc;
          std::cout << "***********min_prc=" << min_proc  << " and max_proc=" << max_proc << std::endl;
        //TODO; -n min_processors[,max_processors]
        break;
     case 'i':
        req->options |=SUB_IN_FILE;
        req->inFile = strdup(optarg);
        break;
     case 'o':
        req->options |=SUB_OUT_FILE;
        req->outFile = strdup(optarg);
        break;
     case 'e':
        req->options |=SUB_ERR_FILE;
        req->errFile = strdup(optarg);
        break;
     case 'x':
        req->options |=SUB_EXCLUSIVE;
        break;
     case 'N':
        req->options |=SUB_NOTIFY_END;
        break;
     case 'B':
        req->options |=SUB_NOTIFY_BEGIN;
        break;
     case 'G':
        req->options |=SUB_USER_GROUP;
        req->userGroup = strdup(optarg);
        std::cout << "*********user-group-optarg=" << optarg << std::endl;
        break;
     case 'k':
        //req->options |=SUB_CHKPNT_DIR and req->options |=SUB_CHKPNT_PERIOD
        //TODO: -k chkpnt_dir [init=initial_chkpnt_period] [chkpnt_period] [method=method_name]
        //req->chkpntPeriod = ?;
        //std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'r':
        req->options |=SUB_RERUNNABLE;
        break;
     case 'w':
        /*req->options |=SUB_DEPEND_COND;
        std::cout << "*********user-group-optarg=" << optarg << std::endl;
        break;*/
     case 'R':
         //req->options |=SUB_RES_REQ;
         //TODO; -R res_req
        //std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'E':
         //req->options |=SUB_PRE_EXEX;
        //std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'L':
         //req->options |=SUB_LOGIN_SHELL;
        //std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'P':
         //TODO; -P project_name
         //req->options |=SUB_PROJECT_NAME;
        //std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'u':
        req->options |=SUB_MAIL_USER;
        req->mailUser = strdup(optarg);
        std::cout << "*********mail-user-optarg=" << optarg << std::endl;
        break;
     case 'U':
        //TODO; -U reservation_id
        break;
     case 'K':
         //req->options2 |=?; //A voir
        break;
     case 'W':
         //-W run_limit[/host_spec]
         //TODO; req->rlimits[LSF_RLIMIT_RUN]
         //req->options |= SUB_HOST_SPEC;
        std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'g':
       req->options2 |= SUB2_JOB_GROUP;
       req->jobGroup = strdup(optarg);
       std::cout << "*********jobGroup-optarg=" << optarg << std::endl;
       break;
     case 'H':
       req->options2 |= SUB2_HOLD;
       std::cout << "*********hold-optarg=" << optarg << std::endl;
       break;
     case 'c':
       //TODO; req->rlimits[LSF_RLIMIT_CPU]
       break;
     case 'F':
       //TODO; req->rlimits[LSF_RLIMIT_FSIZE]
       break;
     case 'p':
       //TODO; req->rlimits[LSF_RLIMIT_PROCESS]
       break;
     case 'M':
       //TODO; req->rlimits[LSF_RLIMIT_RSS]
       break;
     case 'D':
       //TODO; req->rlimits[LSF_RLIMIT_DATA]
       break;
     case 'S':
       //TODO; req->rlimits[LSF_RLIMIT_STACK]
       break;
     case 'v':
       //TODO; req->rlimits[LSF_RLIMIT_SWAP]
       break;
     case 'T':
       //TODO; req->rlimits[LSF_RLIMIT_THREAD]
       break;
     case 'b':
       //TODO; begin_time
       break;
     case 't':
       //TODO; end_time
       break;
     case 'f':
       //TODO; -f "lfile op [rfile]"
       break;
     case 'Q':
       //TODO; -Q requeue_exit_values
       break;
     case LONG_OPT_APP:
       req->options3 |= SUB3_APP;
       req->app= strdup(optarg);
       std::cout << "*********app-optarg=" << optarg << std::endl;
     case LONG_OPT_CWD:
       req->options3 |= SUB3_CWD;
       req->cwd= strdup(optarg);
       std::cout << "*********cwd-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_UL:
       //req->options3 |= SUB3_USER_SHELL_LIMITS;
       std::cout << "*********LONG_OPT_CWD-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_WE:
       //req->options2 |= SUB3_RUNTIME_ESTIMATION;
       std::cout << "---------------------IN LONG_OPT_WE" << std::endl;
       std::cout << "*********we-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_RN:
       req->options3 |= SUB3_NOT_RERUNNABLE;
       std::cout << "*********rn-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_JD:
       //TODO: -Jd job_description
       break;
     case LONG_OPT_IS:
       //TODO: -is in_file
       break;
     case LONG_OPT_EO:
       //TODO: -eo in_file
       break;
     case LONG_OPT_OO:
       //TODO: -oo in_file
       break;
     case LONG_OPT_AR:
       //TODO: ar (boolean)
       break;
     case LONG_OPT_WA:
       //TODO: -wa job_warning_action
       break;
     case LONG_OPT_WT:
       //TODO: -wt job_action_warning_time
       break;
     case LONG_OPT_ZS:
       //TODO: -Zs (boolean)
       break;
     case LONG_OPT_EP:
       //TODO: -Ep "post_exec_command [argument ...]"
       break;
     case LONG_OPT_SP:
       //TODO: -sp job_priority
       break;
     case LONG_OPT_MIG:
       //TODO: -mig migration_threshold
       break;
     case LONG_OPT_SLA:
       //TODO: -sla service_class
       break;
     case LONG_OPT_EXT:
       //TODO: -ext "external_scheduler_options"
       break;
     case LONG_OPT_LP:
       //TODO: -Lp "license_project"
       break;
     case LONG_OPT_JSDL:
       //TODO: -jsdl JSDL_file
       break;
     case LONG_OPT_JSDL_STRICT:
       //TODO: -jsdl_strict JSDL_file
       break;
     case LONG_OPT_RNC:
       //TODO: -rnc resize_notify_command
       break;
     case LONG_OPT_XF:
       //TODO: -XF (boolean)
       break;
    default: /* '?' */
      std::cerr << "invalid option " << optarg << std::endl;
     break;             
  }

 }

 return 0;
}

/**
 * \brief Destructor
 */
LSFParser::~LSFParser(){
}

