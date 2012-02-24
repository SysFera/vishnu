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
         {"ul", required_argument, 0, LONG_OPT_USER_SHELL_LIMITS}
};

/**
 * \brief Constructor
 */
LSFParser::LSFParser(){
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

 #define GETOPT_ARGS "J:q:m:i:o:e:xNBG:k:rw:R:E:L:P:u:K:Wg:H"
 
 int option_index = 0;
 int c;
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
        //req->options |=SUB_HOST;
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
        //req->options |=SUB_CHKPNT_PERIOD;
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
         //req->options |=SUB_PROJECT_NAME;
        //std::cout << "*********-optarg=" << optarg << std::endl;
        break;
     case 'u':
        req->options |=SUB_MAIL_USER;
        req->mailUser = strdup(optarg);
        std::cout << "*********mail-user-optarg=" << optarg << std::endl;
        break;
     case 'K':
         //req->options2 |=?;
        break;
     case 'W':
         //req->options2 |=?;
        //std::cout << "*********-optarg=" << optarg << std::endl;
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
     case LONG_OPT_APP:
       req->options3 |= SUB3_APP;
       req->app= strdup(optarg);
       std::cout << "*********app-optarg=" << optarg << std::endl;
     case LONG_OPT_CWD:
       req->options3 |= SUB3_CWD;
       req->cwd= strdup(optarg);
       std::cout << "*********cwd-optarg=" << optarg << std::endl;
       break;
     case LONG_OPT_USER_SHELL_LIMITS:
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

