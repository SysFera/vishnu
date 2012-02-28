#include <unistd.h>
#include <getopt.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

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

bool isEndByQuote(const string& str) {
  return (*(str.end()-1)=='\"');
}

bool isStartByQuote(const string& str) {
  return (*(str.begin())=='\"');
}

void cleanString(string& str) {
   if(!str.empty()){
      if(*(str.begin())=='\"'){
         str.replace(str.begin(), str.begin()+1, "");
      }
      if(*(str.end()-1)=='\"'){
          str.replace(str.end()-1, str.end(), "");
      }
   }
}

void verifyQuotaCharacter(const string& str) {

 std::string errorMsg = "Error: invalid value option in option "+str;
 errorMsg +=". A text started by the quote character \" must be closed by the quote character \"";
 //First character quote
 size_t pos = str.find('\"');
 if(pos!=std::string::npos) {
   //Second character quote
   pos = str.find('\"', pos+1);
   if(pos==std::string::npos) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
   }
  
   //First character quote 
   pos = str.find('\"', pos+1);
   while(pos!=std::string::npos) {
     if(pos!=std::string::npos) {
        //Second character quote
        pos = str.find('\"', pos+1);
        if(pos==std::string::npos) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
        }
     }
     //First character quote
     pos = str.find('\"', pos+1);
   }
 }

}

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
         
         size_t pos = line.find(LSF_PREFIX);
         size_t vbeg = 0;
         //virify quote characters in line
         while(pos!=std::string::npos){
           verifyQuotaCharacter(line.substr(vbeg, pos-vbeg));
           vbeg = pos+LSF_PREFIX.size();
           pos = line.find(LSF_PREFIX, pos+vbeg);
         }
         //search LSF_PREFIX in the line and replace by empty string
         pos = line.find(LSF_PREFIX);
         while(pos!=std::string::npos){
           line.replace(pos, LSF_PREFIX.size(), " ");
           pos = line.find(LSF_PREFIX);
         }
         //virify quote characters
         verifyQuotaCharacter(line);
         //add line to cmd
         cmd = cmd+" "+line;
       }  
    }
    fileStream.close();

    std::istringstream iss(cmd);
    std::copy(istream_iterator<string>(iss),
              istream_iterator<string>(),
              back_inserter<vector<string> >(tokens));

  }
  
  std::string argvStr;
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = tokens.end();
  std::vector<std::string> tokensArgs;
  for(iter=tokens.begin(); iter!=tokens.end(); ++iter) {
     argvStr = *iter;
     if(isStartByQuote(argvStr)){
        std::vector<std::string>::iterator found_iter;
        found_iter = std::find_if(iter, end, isEndByQuote);
        if(found_iter!=end) {
          while(iter!=found_iter) {
            iter++;
            argvStr = argvStr+" "+*iter;
          }
        } else {
           std::string errorMsg = "Error: invalid argument "+argvStr;
           errorMsg +=". It must be closed by the character \"";
           throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
        }
     }
     tokensArgs.push_back((argvStr.c_str()));
  }

  int argc = tokensArgs.size()+1; 
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  std::cout << "**********argc=" << argc << std::endl;
  std::cout << "**********cmd=" << argv[0] << " ";
  for(int i=0; i < tokensArgs.size(); ++i) {
    argv[i+1] = strdup(tokensArgs[i].c_str());
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
 char separator=',';
 std::string chkpnt;
 std::vector<std::string> chkpnt_tokens;
 std::istringstream stream_chkpnt;
 std::string timeStr;
 std::string wHostSpec;
 std::string errHead = "Error in your script: "; 
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
        std::cout << "***********optarg=" << optarg << std::endl;
        host_list = strdup(optarg);
        if(!host_list.empty()){
          if(*(host_list.begin())=='\"'){
             host_list.replace(host_list.begin(), host_list.begin()+1, "");
          }
          if(*(host_list.end()-1)=='\"'){
             host_list.replace(host_list.end()-1, host_list.end(), "");
          }
        }
        std::cout << "***********host_list=" << host_list << std::endl;
        stream_host_list.str(host_list);
        host_tokens.clear();
        std::copy(istream_iterator<string>(stream_host_list),
              istream_iterator<string>(),
              back_inserter<vector<string> >(host_tokens));
        req->numAskedHosts = host_tokens.size();
        req->askedHosts = new char*[host_tokens.size()];
        std::cout << "***********req->numAskedHosts=" << req->numAskedHosts << std::endl;
        for(int i=0; i < host_tokens.size(); i++) {
              req->askedHosts[i] = strdup(host_tokens[i].c_str());
              std::cout << "***********host" << i << " is " << req->askedHosts[i] << std::endl;
        }
        break;
     case 'n':
         procsStr = strdup(optarg);
         if(procsStr.find(',')!=std::string::npos) {
            stream_procs.str(procsStr);
            stream_procs >> min_proc_str;
            min_proc_str = procsStr.substr(0,procsStr.find(separator));
            if(procsStr.find(separator)+1!=std::string::npos){
              max_proc_str = procsStr.substr(procsStr.find(separator)+1);
            }
            std::cout << "***********min_proc_str=" << min_proc_str  << " and max_proc_str=" << max_proc_str << std::endl;
            if(!isNumerical(min_proc_str) || !isNumerical(max_proc_str)){
              throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(optarg)+"is an invalid"
               " value for -n option. Correct format is -n min_processors[,max_processors]");
            }
            min_proc = vishnu::convertToInt(min_proc_str);
            max_proc = vishnu::convertToInt(max_proc_str);
          } else {
            if(isNumerical(procsStr)) {
             min_proc = vishnu::convertToInt(procsStr);
             max_proc = min_proc;
            } else {
             throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(optarg)+" is an invalid"
                    " value for -n option. Correct format is -n min_processors[,max_processors]");
            }   
          }
          req->numProcessors=min_proc;
          req->maxNumProcessors=max_proc;
          std::cout << "***********min_prc=" << min_proc  << " and max_proc=" << max_proc << std::endl;
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
        req->options |=SUB_CHKPNT_DIR;
        chkpnt = strdup(optarg);
        cleanString(chkpnt);
        std::cout << "*****************chkpnt=" << chkpnt << std::endl;
        stream_chkpnt.str(chkpnt);
        chkpnt_tokens.clear();
        std::copy(istream_iterator<string>(stream_chkpnt),
              istream_iterator<string>(),
              back_inserter<vector<string> >(chkpnt_tokens));
       
        for(vector<std::string>::iterator iter=chkpnt_tokens.begin(); iter!=chkpnt_tokens.end(); ++iter){
          std::cout << "*iter=" << *iter << std::endl;
        }
        req->chkpntDir = strdup(chkpnt_tokens[0].c_str());
        if(chkpnt_tokens.size() >=2) {
           if(boost::algorithm::starts_with(chkpnt_tokens[1], "init=")) {
            chkpnt_tokens[1] = chkpnt_tokens[1].substr(std::string("init=").size());
            if(isNumerical(chkpnt_tokens[1])) {
              req->initChkpntPeriod = vishnu::convertToInt(chkpnt_tokens[1]);
            } else {
               throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+chkpnt_tokens[1]+" is an invalid"
                    " initial checkpoint period value for -k option.");
            }
           } else {
             if(isNumerical(chkpnt_tokens[1])) {
               req->chkpntPeriod= vishnu::convertToInt(chkpnt_tokens[1]);
             } else {
                 throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+chkpnt_tokens[1]+" is an invalid"
                    " checkpoint period value for -k option.");
               }
           }
        }
        if(chkpnt_tokens.size() >=3) {
           if(!boost::algorithm::starts_with(chkpnt_tokens[2], "method=")) {
             req->options |=SUB_CHKPNT_PERIOD;
             if(isNumerical(chkpnt_tokens[2])) {
               req->chkpntPeriod= vishnu::convertToInt(chkpnt_tokens[2]);
             }  else {
                throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+chkpnt_tokens[2]+" is an invalid"
                    " checkpoint period value for -k option.");
             }
           } 
        }
         std::cout << "*****************req->initChkpntPeriod=" << req->initChkpntPeriod << std::endl;
         std::cout << "*****************req->chkpntPeriod=" << req->chkpntPeriod << std::endl;
        //TODO: -k chkpnt_dir [init=initial_chkpnt_period] [chkpnt_period] [method=method_name]
        break;
     case 'r':
        req->options |=SUB_RERUNNABLE;
        break;
     case 'w':
        req->options |=SUB_DEPEND_COND;
        req->dependCond = strdup(optarg);
        break;
     case 'R':
         req->options |=SUB_RES_REQ;
         req->resReq=strdup(optarg);
        break;
     case 'E':
         req->options |=SUB_PRE_EXEC;
         req->preExecCmd = strdup(optarg);
        break;
     case 'L':
         req->options |=SUB_LOGIN_SHELL;
         req->loginShell = strdup(optarg);
        break;
     case 'P':
         req->options |=SUB_PROJECT_NAME;
         req->projectName = strdup(optarg);
        break;
     case 'u':
        req->options |=SUB_MAIL_USER;
        req->mailUser = strdup(optarg);
        std::cout << "*********mail-user-optarg=" << optarg << std::endl;
        break;
     case 'U':
        req->options |=SUB2_USE_RSV;
        req->rsvId = strdup(optarg);
        break;
     case 'K':
         req->options2 |=SUB2_BSUB_BLOCK; //A voir
        break;
     case 'W':
         //-W run_limit[/host_spec]
         timeStr = strdup(optarg);
         if(timeStr.find("\\")!=std::string::npos) {
            if((timeStr.find("\\")+1)!=std::string::npos) {
              if(req->options && SUB_HOST_SPEC !=SUB_HOST_SPEC){
                req->options |= SUB_HOST_SPEC;
              }
              req->rLimits[LSF_RLIMIT_RUN] = vishnu::convertToInt(timeStr.substr(0, timeStr.find("\\")));
              wHostSpec = timeStr.substr(timeStr.find("\\")+1);
              req->hostSpec = strdup(wHostSpec.c_str());
            }
         } else {
            req->rLimits[LSF_RLIMIT_RUN] = vishnu::convertToInt(timeStr);//TODO: replace by convertToDate()
         }
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
        timeStr = strdup(optarg);
        if(timeStr.find("\\")!=std::string::npos) {
           if((timeStr.find("\\")+1)!=std::string::npos) {
             if(req->options && SUB_HOST_SPEC !=SUB_HOST_SPEC){
               req->options |= SUB_HOST_SPEC;
             }
             req->rLimits[LSF_RLIMIT_CPU] = vishnu::convertToInt(timeStr.substr(0, timeStr.find("\\")));
             wHostSpec = timeStr.substr(timeStr.find("\\")+1);
             req->hostSpec = strdup(wHostSpec.c_str());
           }
        } else {
         req->rLimits[LSF_RLIMIT_CPU] = vishnu::convertToInt(timeStr);//TODO: replace by convertToDate()
        }
        break;
     case 'F':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_FSIZE]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " file_limit value for -F option.");
        }
       break;
     case 'p':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_PROCESS]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " process_limit value for -F option.");
        }
       break;
     case 'M':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_RSS]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " mem_limit value for -M option.");
        }
       break;
     case 'D':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_DATA]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " data_limit value for -s option.");
        }
       break;
     case 'S':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_STACK]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " stack_limit value for -S option.");
        }
       break;
     case 'v':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_SWAP]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " swap_limit value for -v option.");
        }
       break;
     case 'T':
        if(isNumerical(strdup(optarg))) {
          req->rLimits[LSF_RLIMIT_THREAD]=vishnu::convertToInt(strdup(optarg));
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " thread_limit value for -T option.");
        }
       break;
     case 'b':
       //TODO; begin_time
       //req->beginTime = LSFParser::convertToTime(strdup(optarg));
       break;
     case 't':
       //TODO; end_time
       //req->termTime = LSFParser::convertToTime(strdup(optarg));
       break;
     case 'f':
       //TODO; -f "lfile op [rfile]"
       // struct xFile *xf
       //nxf = xf_tokens.size();
       //(req->xf)->subFn = lfile 
       //(req->xf)->execFn = rfile
       /*{
           //#define  XF_OP_SUB2EXEC         0x1     ( Transfer files from submit peer to 
                                         * execution peer )
             //#define XF_OP_EXEC2SUB         0x2     ( Transfer files from execution peer to 
                //                         * submit peer )
            //#define  XF_OP_SUB2EXEC_APPEND  0x4     ( Transfer files from submit peer to 
              //                           * execution peer with appending mode )
           //#define  XF_OP_EXEC2SUB_APPEND  0x8     ( Transfer files from execution peer to 
             //                            * submit peer with appending mode )
           //#define  XF_OP_URL_SOURCE       0x10 
        }*/ 
       break;
     case 'Q':
       //TODO; -Q requeue_exit_values
       //req->requeueEValues = strdup(optarg);
       break;
     case LONG_OPT_APP:
       req->options3 |= SUB3_APP;
       req->app= strdup(optarg);
     case LONG_OPT_CWD:
       req->options3 |= SUB3_CWD;
       req->cwd= strdup(optarg);
       break;
     case LONG_OPT_UL:
       req->options3 |= SUB3_USER_SHELL_LIMITS;
       break;
     case LONG_OPT_WE:
       req->options2 |= SUB3_RUNTIME_ESTIMATION;
       req->runtimeEstimation = vishnu::convertToInt(strdup(optarg)); //TODO:convertToDate(); 
       std::cout << "---------------------IN LONG_OPT_WE" << std::endl;
       std::cout << "*********we-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_RN:
       req->options3 |= SUB3_NOT_RERUNNABLE;
       std::cout << "*********rn-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_JD:
       req->options3 |= SUB3_JOB_DESCRIPTION;
       req->jobDescription = strdup(optarg);
       break;
     case LONG_OPT_IS:
       if(req->options && SUB_IN_FILE!=SUB_IN_FILE){
         req->options |=SUB_IN_FILE;//TODO: to complete
       } 
       req->inFile = strdup(optarg);
       break;
     case LONG_OPT_EO:
       if(req->options && SUB_ERR_FILE!=SUB_ERR_FILE){
         req->options |=SUB_ERR_FILE;//TODO: to complete
       } 
       req->errFile = strdup(optarg);
       break;
     case LONG_OPT_OO:
       if(req->options && SUB_OUT_FILE!=SUB_OUT_FILE){
         req->options |=SUB_OUT_FILE;//TODO: to complete
       } 
       req->outFile = strdup(optarg);
       break;
     case LONG_OPT_AR:
       //TODO: ar (boolean)
       break;
     case LONG_OPT_WA:
       req->options2 |=SUB2_WARNING_ACTION;
       req->warningAction = strdup(optarg);
       break;
     case LONG_OPT_WT:
       req->options2 |= SUB2_WARNING_TIME_PERIOD;
       if(isNumerical(strdup(optarg))) {
          req->warningTimePeriod = vishnu::convertToInt(optarg);
        }  else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
                 " job action warning time  value for --wt option.");
        }
       break;
     case LONG_OPT_ZS:
       //TODO: -Zs (boolean)
       break;
     case LONG_OPT_EP:
       req->options3 |= SUB3_POST_EXEC;
       req->postExecCmd = strdup(optarg);
       break;
     case LONG_OPT_SP:
       req->options2 |= SUB2_JOB_PRIORITY;
       if(isNumerical(strdup(optarg))) {
          req->userPriority =  vishnu::convertToInt(strdup(optarg));
       } else {
           throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
               " job priority value for -sp option.");
       }
       break;
     case LONG_OPT_MIG:
       req->options3 |=SUB3_MIG_THRESHOLD;
       if(isNumerical(strdup(optarg))) {
         req->migThreshold = vishnu::convertToInt(strdup(optarg));
       } else {
           throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
               " job migration threshold value for -sp option.");
       }
       break;
     case LONG_OPT_SLA:
       req->options2 |=SUB2_SLA;
       req->sla = strdup(optarg);
       break;
     case LONG_OPT_EXT:
       req->options2 |=SUB2_EXTSCHED;
       req->extsched = strdup(optarg);
       break;
     case LONG_OPT_LP:
       req->options2 |=SUB2_LICENSE_PROJECT;
       req->licenseProject = strdup(optarg);
       break;
     case LONG_OPT_JSDL:
       req->jsdlFlag = 1;
       break;
     case LONG_OPT_JSDL_STRICT:
       req->jsdlFlag = 0;
       break;
     case LONG_OPT_RNC:
       //-rnc resize_notify_command
       req->options3 |=SUB3_RESIZE_NOTIFY_CMD;
       req->notifyCmd = strdup(optarg); 
       break;
     case LONG_OPT_XF:
       //-XF (boolean): To see
       //req->options3 |=SUB3_XFJOB;
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

