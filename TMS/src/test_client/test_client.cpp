#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <unistd.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

//EMF
#include <ecore.hpp> // Ecore metamodel
#include <ecorecpp.hpp> // EMF4CPP utils

#include "UMS_Data.hpp"
#include "TMS_Data.hpp"
#include "api_tms.hpp"

#include "DIET_client.h"

namespace bfs=boost::filesystem; // an alias for boost filesystem namespace
using namespace std;
using namespace vishnu;

std::ostream&
operator<<(std::ostream& os, const TMS_Data::SubmitOptions& options) {

  std::string name = options.getName();
  std::string queuename = options.getQueue();
  int wallTime = options.getWallTime();
  int nbCpu = options.getNbCpu();
  int nbNodesAndCpuPerNode = options.getNbNodesAndCpuPerNode();
  std::string outputPath = options.getOutputPath(); //ATTENTION a changer en getOutPutPath()
  std::string errorPath =  options.getErrorPath();
  int mem = options.getMemory();

  os << "==============SubmitOptions content ================" << endl;
  if(name.size()!=0) os << setw(25) << right << "name: " << name  << endl;
  if(queuename.size()!=0) os << setw(25) << right << "queuename: " << queuename  << endl;
  if(wallTime!=-1) os << setw(25) << right << "wallTime: " << wallTime  << endl;
  if(nbCpu!=-1) os << setw(25) << right << "nbCpu: " << nbCpu  << endl;
  if(nbNodesAndCpuPerNode!=-1) os << setw(25) << right << "nbNodesAndCpuPerNode: " << nbNodesAndCpuPerNode  << endl;
  if(mem!=-1) os << setw(25) << right << "memory: " << mem  << endl;
  if(outputPath.size()!=0) os << setw(25) << right << "outputPath: " << outputPath  << endl;
  if(errorPath.size()!=0) os << setw(25) << right << "errorPath: " << errorPath  << endl;
  os << endl;

 return os;
}

std::ostream&
operator<<(std::ostream& os, const TMS_Data::Job_ptr& job) {

  std::string jobId = job->getJobId();

  os << "==============Job content ================" << endl;
  if(jobId.size()!=0) os << setw(25) << right << "jobId: " << jobId  << endl;
  os << endl;

  return os;
}

std::string get_file_content(const std::string& filePath) {

  bfs::path file (filePath);
  // Check existence of the file
  if ((false==bfs::exists(file)) || (true==bfs::is_empty(file)) ){
    cerr << "can not read the file: " + filePath << endl;;
    exit(EXIT_FAILURE);
  }

  bfs::ifstream ifs (file);

  // Read the whole file into string
  std::stringstream ss;
  ss << ifs.rdbuf();

 return ss.str();

}

void usage(char* cmd) {

  cerr << endl;
  cerr << "Usage: " << cmd;
  cerr << " [-h] [-n name] [-q queue] [-t wallTime] [-m memory ]" << endl;
  cerr <<  "[-P nbCpu] [-N nbNodesAndCpuPerNode] [-o outPutPath] [-e errorPath]" << endl;
  cerr << " <vishnu_file_config> <sessionKey> <machineId> <scriptPath> " << endl;
  cerr << endl;
  exit(EXIT_FAILURE);
}
/*
 * MAIN
 *
 * @param argc
 * @param argv
 *   argv[0] :
 */

int
main(int argc, char* argv[], char* envp[])
{

  char* machineId  = NULL;
  char* sessionKey = NULL;
  char* scriptPath = NULL;
  char* vishnu_config_file = NULL;
  char* submitOptionsToString = NULL;
  char* jobToString = NULL;
  char* jobInString = NULL;
  char* errorInfo = NULL;
  diet_profile_t* profile = NULL;

  cout << "*******Begin test **********" << endl;
  TMS_Data::SubmitOptions submitOptions;
  int  opt;
  while ((opt = getopt(argc, argv, "hn:q:t:m:P:N:o:e:")) != -1) {

    switch (opt) {
      case 'n':
        submitOptions.setName(optarg);
        break;
      case 'q':
        submitOptions.setQueue(optarg);
        break;
      case 't':
        submitOptions.setWallTime(atoi(optarg));
        break;
      case 'P':
        submitOptions.setNbCpu(atoi(optarg));
        break;
      case 'N':
        submitOptions.setNbNodesAndCpuPerNode(atoi(optarg));
        break;
      case 'o':
        submitOptions.setOutputPath(optarg);
        break;
      case 'e':
        submitOptions.setErrorPath(optarg);
        break;
      case 'm':
        submitOptions.setMemory(atoi(optarg));
        break;
      default: /* 'h' '?' */
        usage(argv[0]);
    }

  }

  if(argc-optind < 4) {
    usage(argv[0]);
  } else {
    vishnu_config_file = argv[optind++];
    sessionKey =  argv[optind++];
    machineId = argv[optind++];
    scriptPath = argv[optind];
    std::cout << "arguments: " << argv[0] << " " << vishnu_config_file << " " << sessionKey << " " << machineId << " " << scriptPath << endl;
    std::cout << submitOptions;
    std::cout << "The service was performed successfully" << std::endl;
  }

  try {
   // initializing DIET
    if (diet_initialize(vishnu_config_file, argc, argv)) {
      cerr << "DIET initialization failed !" << endl;
      exit(EXIT_FAILURE);
    }

    TMS_Data::Job job;
    /*job.setJobPath(scriptPath);
    job.setSubmitMachineId(machineId);
    //To set the script content
    std::string script_content = get_file_content(scriptPath);
    job.setJobPath(script_content);

    const char* name = "jobSumit";
    ::ecorecpp::serializer::serializer _ser(name);
    jobToString =  strdup(_ser.serialize(const_cast<TMS_Data::Job_ptr>(&job)).c_str());
    ::ecorecpp::serializer::serializer _ser2(name);
    submitOptionsToString =  strdup(_ser2.serialize(const_cast<TMS_Data::SubmitOptions_ptr>(&submitOptions)).c_str());

    profile = diet_profile_alloc(("jobSubmit_"+std::string(machineId)).c_str(), 3, 3, 5);

    diet_string_set(diet_parameter(profile,0), sessionKey, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,1), machineId, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,2), submitOptionsToString, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,3), jobToString, DIET_VOLATILE);

    //OUT Parameters
    diet_string_set(diet_parameter(profile,4), NULL, DIET_VOLATILE);
    diet_string_set(diet_parameter(profile,5), NULL, DIET_VOLATILE);

    if(!diet_call(profile)) {
      diet_string_get(diet_parameter(profile,4), &jobInString, NULL);
      diet_string_get(diet_parameter(profile,5), &errorInfo, NULL);
    } else {
      cerr << "Error in diet_call...." << endl;
      exit(EXIT_FAILURE);
    }


    TMS_Data::TMS_DataPackage_ptr ecorePackage = TMS_Data::TMS_DataPackage::_instance();
    ecorecpp::MetaModelRepository::_instance()->load(ecorePackage);

    //Parse the model
    ecorecpp::parser::parser parser;
    job = *(parser.load(jobInString)->as<TMS_Data::Job>());
    */


    submitJob(sessionKey,
              machineId,
              std::string(scriptPath),
              job,
              submitOptions);
    std::cout << "***************job.getJobId()=" << job.getJobId() << std::endl;
  }
  catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

