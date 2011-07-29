#include "LogTool.hpp"
#include <unistd.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include "utilVishnu.hpp"
#include "utilVishnu.hpp"

using namespace vishnu;

LogTool::LogTool(string mid):mmid(mid){
  mname = "IMSLogT_";
  mname += mid;
  mname += "_"+convertToString(generateNumbers());
}

LogTool::~LogTool(){
}
