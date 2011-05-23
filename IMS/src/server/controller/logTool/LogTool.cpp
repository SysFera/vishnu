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
  boost::mt19937 rng;
  boost::uniform_int<> uni(1,1<<25); // Randon int between 1 and 1<<25
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
    die(rng, uni);
  char host[HOSTSIZE];
  if (gethostname(host, HOSTSIZE)){
    mname = "IMSLogT";
  }
  else{
    mname = "IMSLogT_";
    mname += string(host);
  }
  mname += "_"+convertToString(die());
}

LogTool::~LogTool(){
}
