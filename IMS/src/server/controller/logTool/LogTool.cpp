#include "LogTool.hpp"
#include <unistd.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include "utilVishnu.hpp"

using namespace vishnu;

LogTool::LogTool(){
  srand(time(NULL));

  char host[HOSTSIZE];
  if (gethostname(host, HOSTSIZE)){
    mname = "IMSLogT";
  }
  else{
    mname = "IMSLogT_";
    mname += string(host);
  }
  mname += "_"+convertToString(rand());
}

LogTool::~LogTool(){
}
