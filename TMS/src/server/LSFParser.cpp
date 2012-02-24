#include <unistd.h>

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

 #define GETOPT_ARGS "J:q:o:e:"

 int c;
 while ((c = getopt(argc, argv, GETOPT_ARGS)) != EOF) {
   switch (c) {
     case 'J':
        req->options |=SUB_JOB_NAME;
        req->jobName = strdup(optarg);
        std::cout << "*********jobName-optarg=" << optarg << std::endl;
        break;
     case 'q':
        req->options |=SUB_QUEUE;
        req->queue = optarg;
        std::cout << "*********queue-optarg=" << optarg << std::endl;
        break;
     case 'o':
        req->options |=SUB_OUT_FILE;
        req->outFile = strdup(optarg);
        std::cout << "*********outFile-optarg=" << optarg << std::endl;
        break;
     case 'e':
        req->options |=SUB_ERR_FILE;
        req->errFile = optarg;
        std::cout << "*********errFile-optarg=" << optarg << std::endl;
        break;
     default: /* '?' */
      std::cerr << "error " << std::endl;
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

