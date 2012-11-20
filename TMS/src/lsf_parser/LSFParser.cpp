/**
 * \file LSFParser.cpp
 * \brief This file presents the implementation of the LSF Parser
 * \author daouda.traore (daouda.traore@sysfera.com)
 * \date February 2012
*/

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
//long option table
static struct option long_options[] = {
  {"We", required_argument, 0, LONG_OPT_WE},
  {"rn", no_argument, 0, LONG_OPT_RN},
  {"app", required_argument, 0, LONG_OPT_APP},
  {"cwd", required_argument, 0, LONG_OPT_CWD},
  {"ul", no_argument, 0, LONG_OPT_UL},
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
  {"XF", no_argument, 0, LONG_OPT_XF},
  {"I", no_argument, 0, LONG_OPT_I},
  {"Ip", no_argument, 0, LONG_OPT_Ip},
  {"Is", no_argument, 0, LONG_OPT_Is},
  {"IS", no_argument, 0, LONG_OPT_IS2},
  {"ISp", no_argument, 0, LONG_OPT_ISp},
  {"ISs", no_argument, 0, LONG_OPT_ISs},
  {"IX", no_argument, 0, LONG_OPT_IX}
};

struct IsEndByQuote {

  char mquote;
  IsEndByQuote(const char quote):mquote(quote){};

  bool operator()(const string& str) {
    return (str.size() > 1 && *(str.end()-1)==mquote);
  }
};

bool isStartByQuote(const string& str, const char quote) {
  return (str.size() >= 1 && *(str.begin())==quote);
}

bool cleanString(string& str, const char& quote) {

  bool begIsClean = false;
  bool endIsClean = false;
  if(!str.empty()){
    if(*(str.begin())==quote){
      str.replace(str.begin(), str.begin()+1, "");
      begIsClean = true;
    }
    if(*(str.end()-1)==quote){
      str.replace(str.end()-1, str.end(), "");
      endIsClean = true;
    }
    // erase all white space at a begining
    while(boost::algorithm::starts_with(str, " ")){
       str.erase(0,1);
    };
  }
  return (begIsClean && endIsClean);
}

void verifyQuotaCharacter(const string& str, const char& quote) {

  std::string quoteStr;
  quoteStr.push_back(quote);
  std::string errorMsg = "Error: invalid value option in option "+str;
  errorMsg +=". A text started by the quote character "+quoteStr+" must be closed by the quote character "+quoteStr;
  //First character quote
  size_t pos = str.find(quote);
  if(pos!=std::string::npos) {
    //Second character quote
    pos = str.find(quote, pos+1);
    if(pos==std::string::npos) {
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
    }

    //First character quote
    pos = str.find(quote, pos+1);
    while(pos!=std::string::npos) {
      if(pos!=std::string::npos) {
        //Second character quote
        pos = str.find(quote, pos+1);
        if(pos==std::string::npos) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
        }
      }
      //First character quote
      pos = str.find(quote, pos+1);
    }
  }

}

std::vector<std::string>
getStreamTokens(const std::string& str) {

  std::istringstream stream_str;
  std::vector<std::string> streamTokens;

  stream_str.str(str);
  std::copy(istream_iterator<string>(stream_str),
      istream_iterator<string>(),
      back_inserter<vector<string> >(streamTokens));

  return streamTokens;
}

std::vector<std::string>
getTimeToKens(const std::string& timeFormat, const char& separator=':') {

  size_t beginPosToken = timeFormat.rfind(separator);
  size_t endPosToken = timeFormat.size()-1;
  std::vector<std::string> timeTokens;
  if(beginPosToken==std::string::npos) {
    timeTokens.push_back(timeFormat);
  }
  while(beginPosToken!=std::string::npos) {
    timeTokens.push_back(timeFormat.substr(beginPosToken+1, endPosToken-beginPosToken));
    endPosToken = beginPosToken-1;
    beginPosToken = timeFormat.rfind(separator, endPosToken);
    //last token
    if(beginPosToken==std::string::npos){
      timeTokens.push_back(timeFormat.substr(0, endPosToken+1));
    }
  }

  return timeTokens;
}

/**
 * \brief Constructor
 */
LSFParser::LSFParser(){
}

time_t
LSFParser::convertDateToTime(const std::string& date, const std::string& compErrMsg) {

  time_t totalTime;
  time_t timeNow;
  struct tm totalTimeTm;
  struct tm *timeNowTm;
  int minute = -1;
  int hour   = -1;
  int day    = -1;
  int month  = -1;
  int year   = -1;
  bool hasDayField   = false;
  bool hasMonthField = false;
  bool hasYearField  = false;

  std::string errMsg = "illegal date option value "+date+":";
  vector<std::string> tokens = getTimeToKens(date);
  for(std::vector<std::string>::iterator iter = tokens.begin(); iter!=tokens.end(); ++iter) {
    if(!isNumerical(*iter)) {
      errMsg += " The fields values must be intergers.";
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
    }
  }

  if(tokens.size() < 2) {
    errMsg += " At least two fields must be specified.";
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
  } else {

    //minute
    minute = vishnu::convertToInt(tokens[0]);
    if(minute < 0 || minute > 59) {
      errMsg += " minute number range must be 0-59.";
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
    }
    //hour
    hour = vishnu::convertToInt(tokens[1]);
    if(hour < 0 || hour > 23) {
      errMsg += " hour number range must be 0-23.";
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
    }
    //day
    if(tokens.size() >= 3) {
      day = vishnu::convertToInt(tokens[2]);
      if(day < 1 || day > 31) {
        errMsg += " day number range must be 1-31.";
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
      }
      hasDayField = true;
    }
    //month
    if(tokens.size() >= 4) {
      month = vishnu::convertToInt(tokens[3]);
      if(month < 1 || month > 12) {
        errMsg += " month number range must be 1-31.";
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
      }
      hasMonthField = true;
    }
    //year
    if(tokens.size() >= 5) {
      year = vishnu::convertToInt(tokens[4]);
      if(year < 1970) {
        errMsg += " year must after 1970.";
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
      }
      hasYearField = true;
    }
  }

  timeNow = std::time(NULL);
  timeNowTm = std::localtime(&timeNow);

  totalTimeTm.tm_min = minute;
  totalTimeTm.tm_hour = hour;
  //day
  if(hasDayField) {
    totalTimeTm.tm_mday = day;
  } else {
    if((hour < timeNowTm->tm_hour)|| (hour==timeNowTm->tm_hour && minute < timeNowTm->tm_min))
    {
      totalTimeTm.tm_mday = timeNowTm->tm_mday+1;
    } else {
      totalTimeTm.tm_mday = timeNowTm->tm_mday;
    }
  }
  //month
  if(hasMonthField) {
    totalTimeTm.tm_mon = month-1;
  } else {
    if((day!=-1 && (day < timeNowTm->tm_mday))
        || (day==timeNowTm->tm_mday && hour < timeNowTm->tm_hour)
        || (day==timeNowTm->tm_mday && hour==timeNowTm->tm_hour && minute < timeNowTm->tm_min))
    {
      totalTimeTm.tm_mon = timeNowTm->tm_mon+1;
    } else {
      totalTimeTm.tm_mon = timeNowTm->tm_mon;
    }
  }
  //month
  if(hasYearField) {
    totalTimeTm.tm_year = year-1900;
  } else {
    if((month!=-1 && ((month-1) < timeNowTm->tm_mon))
        ||((month-1)==timeNowTm->tm_mon && day < timeNowTm->tm_mday)
        || ((month-1)==timeNowTm->tm_mon && day==timeNowTm->tm_mday && hour < timeNowTm->tm_hour)
        || ((month-1)==timeNowTm->tm_mon && day==timeNowTm->tm_mday
          && hour==timeNowTm->tm_hour && minute < timeNowTm->tm_min))
    {
      totalTimeTm.tm_year = timeNowTm->tm_year+1;
    } else {
      totalTimeTm.tm_year = timeNowTm->tm_year;
    }
  }

  totalTimeTm.tm_sec = 0;
  totalTimeTm.tm_isdst = -1;
  totalTime = std::mktime(&totalTimeTm);

  return totalTime;
}


int
LSFParser::convertWallTimeToTime(const std::string& date, const std::string& compErrMsg) {

  int wallTime;
  int minute = -1;
  int hour   = -1;

  std::string errMsg = "illegal time  option value "+date+":";
  vector<std::string> tokens = getTimeToKens(date);
  if(tokens.empty()) {
    errMsg += " Empty time value. At least one fields must be specified.";
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
  }
  for(std::vector<std::string>::iterator iter = tokens.begin(); iter!=tokens.end(); ++iter) {
    if(!isNumerical(*iter)) {
      errMsg += " The fields values must be intergers.";
      throw UMSVishnuException(ERRCODE_INVALID_PARAM, errMsg+" "+compErrMsg);
    }
  }

  //hour
  minute = vishnu::convertToInt(tokens[0]);
  wallTime = minute;
  //day
  if(tokens.size() >= 2) {
    hour = vishnu::convertToInt(tokens[1]);
    wallTime += hour*60;
  }

  return wallTime;
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

std::vector<std::string>
LSFParser::convertScriptIntoArgv(const char* pathTofile,
                                 const std::string& BATCH_PREFIX){

  ifstream fileStream;
  string line;

  fileStream.open (pathTofile);

  static std::map<size_t, pair<string,string> > tab;
  std::string cmd;
  vector<string> tokens;
  std::string tmpLine="";
  size_t escapePos;
  bool escapeFound = false;

  if (fileStream.is_open()) {
    while (!fileStream.eof()) {

      getline(fileStream, line);

      //Treating of the escape character int the script content
      if(boost::algorithm::ends_with(boost::algorithm::erase_all_copy(line, " "),"\\")){
        escapePos = line.rfind("\\");
        if(escapePos!=std::string::npos) {
          tmpLine += line.substr(0, escapePos);
          escapeFound = true;
          continue;
        }
      }

      if(escapeFound) {
        tmpLine +=line;
        line = tmpLine;
        escapeFound = false;
        tmpLine = "";
      }

      // erase all white space until # (excluded)
      while(boost::algorithm::starts_with(line, " ")){
        line.erase(0,1);
      };

      /*search # character*/
      size_t pos = line.find('#');
      if(pos == string::npos) {
        continue;
      }

      if(boost::algorithm::starts_with(boost::algorithm::erase_all_copy(line," "), BATCH_PREFIX)){

        pos = line.find(BATCH_PREFIX.substr(1));//skip the character # in  BATCH_PREFIX
        line = line.substr(pos+BATCH_PREFIX.size()-1);

        //To skip a comment on the line
        pos = line.find('#');
        if(pos!=std::string::npos) {
           if(pos-1 >=0) {
              std::string tmp = line.substr(pos-1);
              if(boost::algorithm::starts_with(boost::algorithm::erase_all_copy(tmp, " "), "#")) {
                line = line.substr(0, pos);
              }
           } else {
             line = line.substr(0, pos);
           }
        }
        //verify quote characters in line
        verifyQuotaCharacter(line, '\"');
        verifyQuotaCharacter(line, '\'');
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
  char quote = '\0';
  for(iter=tokens.begin(); iter!=end; ++iter) {
    argvStr = *iter;
    if(isStartByQuote(argvStr, '\"')) {
      quote = '\"';
    } else if(isStartByQuote(argvStr, '\'')){
      quote = '\'';
    }
    if(quote!='\0'){
      std::vector<std::string>::iterator found_iter;
      std::vector<std::string>::iterator beg_iter=iter;
      found_iter = std::find_if(beg_iter, end, IsEndByQuote(quote));
      if(found_iter!=end) {
        while(beg_iter!=found_iter) {
           ++beg_iter;
           argvStr = argvStr+" "+*beg_iter;
        }
        iter=beg_iter;
      } else {
        std::string errorMsg = "Error: invalid argument "+argvStr;
        errorMsg +=". It must be closed by the character quote character (\' or \")";
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errorMsg);
      }
    }
    quote='\0';
    if(!cleanString(argvStr, '\"')) {
       cleanString(argvStr, '\'');
    }
    tokensArgs.push_back(argvStr);
  }

 return tokensArgs;
}

int
LSFParser::parse_file(const char* pathTofile, struct submit* req) {

  std::vector<std::string>  tokensArgs=convertScriptIntoArgv(pathTofile);
  int argc = tokensArgs.size()+1;
  char* argv[argc];
  argv[0] = (char*) "vishnu_submit_job";
  for(int i=0; i < tokensArgs.size(); ++i) {
    argv[i+1] = strdup(tokensArgs[i].c_str());
  }

  #define GETOPT_ARGS "J:q:m:n:i:o:e:xNBG:k:rw:R:E:L:P:u:U:KW:g:Hc:F:p:M:D:S:v:T:b:t:f:Q:s:"

  int option_index = 0;
  optind=0;
  int c;
  std::string host_list;
  std::vector<std::string> host_tokens;
  std::string procsStr;
  std::istringstream stream_procs;
  std::string min_proc_str;
  std::string max_proc_str;
  int min_proc;
  int max_proc;
  char separator=',';
  std::string chkpnt;
  std::vector<std::string> chkpnt_tokens;
  std::string timeStr;
  std::string wHostSpec;
  struct xFile *tmpXfile;
  std::string xFileStr;
  std::vector<std::string> xFile_tokens;
  int oldNxf;

  std::string errHead = "Error in your script: ";
  //set default values for jobName and job output and error path
  req->options |=SUB_JOB_NAME;
  req->jobName = (char*) "NoName";
  req->options |=SUB_OUT_FILE;
  req->outFile = (char*) "lsf-%J.out";
  req->options |=SUB_ERR_FILE;
  req->errFile = (char*) "lsf-%J.err";

  //Parse options in script file
  while ((c = getopt_long_only(argc, argv, GETOPT_ARGS, long_options, &option_index)) != -1) {
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
        host_tokens=getStreamTokens(host_list);
        req->numAskedHosts = host_tokens.size();
        req->askedHosts = new char*[host_tokens.size()];
        for(int i=0; i < host_tokens.size(); i++) {
          req->askedHosts[i] = strdup(host_tokens[i].c_str());
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
        break;
      case 'i':
        if((req->options2 & SUB2_IN_FILE_SPOOL)==SUB2_IN_FILE_SPOOL) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" is|i options are exclusive");
        }
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
        break;
      case 'k':
        req->options |=SUB_CHKPNT_DIR;
        chkpnt = strdup(optarg);
        chkpnt_tokens=getStreamTokens(chkpnt);
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
        break;
      case 'r':
        req->options |=SUB_RERUNNABLE;
        if((req->options3 & SUB3_NOT_RERUNNABLE)==SUB3_NOT_RERUNNABLE) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" You cannot set a job to be rerunnable"
                                   "and not-rerunnable at the same time");
        }
        break;
      case 'w':
        req->options |=SUB_DEPEND_COND;
        req->dependCond = strdup(optarg);
        break;
      case 'R':
        req->options |=SUB_RES_REQ;
        req->resReq = strdup(optarg);
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
        break;
      case 'U':
        req->options2 |=SUB2_USE_RSV;
        req->rsvId = strdup(optarg);
        break;
      case 'K':
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" option -K is "
                  " not treated by VISHNU.");
        break;
      case 'W':
        //-W run_limit[/host_spec]
        timeStr = std::string(strdup(optarg));
        if(timeStr.find("/")!=std::string::npos) {
          if((timeStr.find("/")+1)!=std::string::npos) {
            req->options |= SUB_HOST_SPEC;
            req->rLimits[LSF_RLIMIT_RUN] = convertWallTimeToTime(timeStr.substr(0, timeStr.find("/")));
            wHostSpec = timeStr.substr(timeStr.find("/")+1);
            req->hostSpec = strdup(wHostSpec.c_str());
          }
        } else {
          req->rLimits[LSF_RLIMIT_RUN] = convertWallTimeToTime(timeStr);
        }
        break;
      case 'g':
        req->options2 |= SUB2_JOB_GROUP;
        req->jobGroup = strdup(optarg);
        break;
      case 'H':
        req->options2 |= SUB2_HOLD;
        break;
      case 'c':
        timeStr = std::string(strdup(optarg));
        if(timeStr.find("/")!=std::string::npos) {
          if((timeStr.find("/")+1)!=std::string::npos) {
            req->options |= SUB_HOST_SPEC;
            req->rLimits[LSF_RLIMIT_CPU] = convertWallTimeToTime(timeStr.substr(0, timeStr.find("/")));
            wHostSpec = timeStr.substr(timeStr.find("/")+1);
            req->hostSpec = strdup(wHostSpec.c_str());
          }
        } else {
          req->rLimits[LSF_RLIMIT_CPU] = convertWallTimeToTime(timeStr);
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
              " process_limit value for -p option.");
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
              " data_limit value for -D option.");
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
        req->beginTime = convertDateToTime(strdup(optarg), " See LSF manual for -b option.");
        break;
      case 't':
        req->termTime = convertDateToTime(strdup(optarg), " See LSF manual for -t option.");
        break;
      case 'f':
        //-f "lfile op [rfile]"
        xFileStr = strdup(optarg);
        xFile_tokens = getStreamTokens(xFileStr);

        if(xFile_tokens.size() < 2) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+xFileStr+" is an invalid"
              " file transfer value for -f option. This must be taken at least two arguments ");
        } else {

          if(req->nxf > 0) {
            oldNxf=req->nxf;
            tmpXfile = new xFile[req->nxf];
            for(int i=0; i < oldNxf; ++i) {
              tmpXfile[i]=req->xf[i];
            }
            delete [] req->xf;
            req->nxf +=1;
            req->xf = new xFile[req->nxf];
            for(int i=0; i < oldNxf; ++i) {
              req->xf[i]=tmpXfile[i];
            }
            delete [] tmpXfile;

          } else {
            oldNxf=0;
            req->nxf =1;
            req->xf = new xFile[req->nxf];
          }

          req->xf[oldNxf].subFn = strdup(xFile_tokens[0].c_str());
          if(xFile_tokens[1]==">") {
            req->xf[oldNxf].options = XF_OP_SUB2EXEC;
          }
          else if(xFile_tokens[1]=="<") {
            req->xf[oldNxf].options = XF_OP_EXEC2SUB;
          }
          else if(xFile_tokens[1]=="<<") {
            req->xf[oldNxf].options = XF_OP_EXEC2SUB_APPEND;
          }
          else if(xFile_tokens[1]=="><") {
            req->xf[oldNxf].options = (XF_OP_SUB2EXEC | XF_OP_EXEC2SUB);
          }
          else if(xFile_tokens[1]=="<>") {
            req->xf[oldNxf].options = (XF_OP_SUB2EXEC | XF_OP_EXEC2SUB);
          } else {
            throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+xFile_tokens[1]+" is an invalid"
                " file transfer operator for -f option. See LSF manual.");
          }

          if(xFile_tokens.size() >=3) {
            req->xf[oldNxf].execFn = strdup(xFile_tokens[2].c_str());
          }
        }
        break;
      case 'Q':
        req->options3 |= SUB3_JOB_REQUEUE;
        req->requeueEValues = strdup(optarg);
        break;
      case 's':
        if(isNumerical(strdup(optarg))) {
          req->sigValue = vishnu::convertToInt(strdup(optarg));
        } else {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+std::string(strdup(optarg))+" is an invalid"
              " job migration threshold value for -s option.");
        }
        break;
      case LONG_OPT_APP:
        req->options3 |= SUB3_APP;
        req->app= strdup(optarg);
        break;
      case LONG_OPT_CWD:
        req->options3 |= SUB3_CWD;
        req->cwd= strdup(optarg);
        break;
      case LONG_OPT_UL:
        req->options3 |= SUB3_USER_SHELL_LIMITS;
        break;
      case LONG_OPT_WE:
        req->options3 |= SUB3_RUNTIME_ESTIMATION;
        timeStr = strdup(optarg);
        if(timeStr.find("/")!=std::string::npos) {
          if((timeStr.find("/")+1)!=std::string::npos) {
            req->options |= SUB_HOST_SPEC;
            req->runtimeEstimation = convertWallTimeToTime(timeStr.substr(0, timeStr.find("/")));
            wHostSpec = timeStr.substr(timeStr.find("/")+1);
            req->hostSpec = strdup(wHostSpec.c_str());
          }
        } else {
          req->runtimeEstimation = convertWallTimeToTime(timeStr);
        }
        break;
      case LONG_OPT_RN:
        req->options3 |= SUB3_NOT_RERUNNABLE;
        if((req->options & SUB_RERUNNABLE)==SUB_RERUNNABLE) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" You cannot set a job to be rerunnable"
                                   "and not-rerunnable at the same time");
        }
        break;
      case LONG_OPT_JD:
        req->options3 |= SUB3_JOB_DESCRIPTION;
        req->jobDescription = strdup(optarg);
        break;
      case LONG_OPT_IS:
        if((req->options & SUB_IN_FILE)==SUB_IN_FILE) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" is|i options are exclusive");
        }
        req->options |=SUB_IN_FILE;
        req->options2 |=SUB2_IN_FILE_SPOOL;
        req->inFile = strdup(optarg);
        break;
      case LONG_OPT_EO:
        req->options |=SUB_ERR_FILE;
        req->options2 |= SUB2_OVERWRITE_ERR_FILE;
        req->errFile = strdup(optarg);
        break;
      case LONG_OPT_OO:
        req->options |=SUB_OUT_FILE;
        req->options2 |= SUB2_OVERWRITE_OUT_FILE;
        req->outFile = strdup(optarg);
        break;
      case LONG_OPT_AR:
        req->options3 |= SUB3_AUTO_RESIZE;
        break;
      case LONG_OPT_WA:
        req->options2 |=SUB2_WARNING_ACTION;
        req->warningAction = strdup(optarg);
        break;
      case LONG_OPT_WT:
        req->options2 |= SUB2_WARNING_TIME_PERIOD;
        req->warningTimePeriod = convertWallTimeToTime(strdup(optarg), "Is an invalid"
            " job action warning time  value for -wt option.");
        break;
      case LONG_OPT_ZS:
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" -Zs is not supported for VISHNU"
                                 " because LSF is unable to determine the first command"
                                 " to be spooled in an embedded job command"
                                 "(VISHNU job is considered by LSF as embedded job commands)");
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
              " job migration threshold value for -mig option.");
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
        if(req->jsdlFlag == 0) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" conflict: you cannot set jsdl"
                                   " and jsdl_strict at the same time");
        }
        req->jsdlFlag = 1;
        req->jsdlDoc = strdup(optarg);
        break;
      case LONG_OPT_JSDL_STRICT:
        if(req->jsdlFlag == 1) {
          throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" conflict: you cannot set jsdl"
                                   " and jsdl_strict at the same time");
        }
        req->jsdlFlag = 0;
        req->jsdlDoc = strdup(optarg);
        break;
      case LONG_OPT_RNC:
        req->options3 |=SUB3_RESIZE_NOTIFY_CMD;
        req->notifyCmd = strdup(optarg);
        break;
      case LONG_OPT_XF:
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" option -XF is "
                  " not treated by VISHNU.");
        break;
      case LONG_OPT_I: case LONG_OPT_Ip: case LONG_OPT_Is:
      case LONG_OPT_IS2: case LONG_OPT_ISp: case LONG_OPT_ISs: case LONG_OPT_IX:
        throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+" LSF batch interactive job "
                                 "option "+std::string(argv[optind-1])+" is not supported by VISHNU");
       break;
      default:
       throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+"Invalid option: "
                  +std::string(argv[optind-1]));
       break;
    }

  }

  if(optind < argc) {
    throw UMSVishnuException(ERRCODE_INVALID_PARAM, errHead+"Invalid argument: "
                             +std::string(argv[optind]));
  }
  //search vishnu generic syntaxes and convert them
  searchAndConvertVishnuScriptGenSyntax(pathTofile, &(*req));

  return 0;
}


void
LSFParser::searchAndConvertVishnuScriptGenSyntax(const char* pathTofile, struct submit* req) {

  std::vector<std::string>  tokensArgs=convertScriptIntoArgv(pathTofile, "#%");
  std::string nodesAndCpuPerNodeSyntax="-vishnuNbNodesAndCpuPerNode=";//Ref ScriptGenConvertor
  std::string cpuSyntax="-vishnuCpu=";//Ref ScriptGenConvertor
  std::string mailNotificationSyntax="-vishnuMailNofication="; //Ref ScriptGenConvertor
  std::string wallTimeSyntax="-vishnuWaillClockLimit=";//Ref ScriptGenConvertor
  size_t pos;

  std::vector<std::string>::iterator iter;
  for(iter=tokensArgs.begin(); iter!=tokensArgs.end(); ++iter) {
      //to treat #% -vishnuNbNodesAndCpuPerNode=x:y
      if((pos=(*iter).find(nodesAndCpuPerNodeSyntax))!=std::string::npos){
         std::string nbNodesAndCpuPerNode = (*iter).substr(pos+nodesAndCpuPerNodeSyntax.size());
         cleanString(nbNodesAndCpuPerNode,'\"');
         cleanString(nbNodesAndCpuPerNode,'\'');
         vishnu::checkJobNbNodesAndNbCpuPerNode(nbNodesAndCpuPerNode);

          size_t posNbNodes = nbNodesAndCpuPerNode.find(":");
          if(posNbNodes!=std::string::npos) {
              std::string nbNodesStr = nbNodesAndCpuPerNode.substr(0, posNbNodes);
              std::string cpuPerNode = nbNodesAndCpuPerNode.substr(posNbNodes+1);


              struct hostInfoEnt *hostInfo;
              char **hosts = NULL;
              int numhosts = 0;
              int nbNodes = vishnu::convertToInt(nbNodesStr);
              //select the candidates host
              hostInfo = lsb_hostinfo(hosts, &numhosts);
              if(nbNodes > numhosts) {
                 throw UserException(ERRCODE_BATCH_SCHEDULER_ERROR, "LSF ERRROR: "
                  "In your script: the number of nodes is greater than the number of total nodes.");
              }
              //set the number of processor
              req->numProcessors = vishnu::convertToInt(cpuPerNode);
              req->maxNumProcessors = req->numProcessors*nbNodes;
          }
      }
      //To treat cpu syntax
      if((pos=(*iter).find(cpuSyntax))!=std::string::npos){
         std::string cpuStr = (*iter).substr(pos+cpuSyntax.size());
         int cpu;
         if(isNumerical(cpuStr)) {
            cpu = vishnu::convertToInt(cpuStr);
         } else {
              throw UMSVishnuException(ERRCODE_INVALID_PARAM, "Illegal value in your script, the value of vishnu_nb_cpu"
                  " must be integer value");
         }
         req->numProcessors = cpu;
         req->maxNumProcessors= cpu;
         //compte the number of unique nodes
         std::vector<std::string> tmpHosts;
         for(int i=0; i < req->numAskedHosts; i++) {
           if(req->askedHosts[i]!=NULL) {
             tmpHosts.push_back(req->askedHosts[i]);
           }
         }
         std::vector<std::string>::iterator endTmp=std::unique(tmpHosts.begin(), tmpHosts.end());
         int node = endTmp-tmpHosts.begin();
         if(node <=0) {
            node = 1;
         }
         req->numProcessors = req->maxNumProcessors= req->numProcessors*node;

      }
      //To treat mail notification syntax
      if((pos=(*iter).find(mailNotificationSyntax))!=std::string::npos) {
           std::string notification = (*iter).substr(pos+mailNotificationSyntax.size());
           if(notification.compare("BEGIN")==0) {
               req->options |=SUB_NOTIFY_BEGIN;
           } else if(notification.compare("END")==0) {
               req->options |=SUB_NOTIFY_END;
           } else if(notification.compare("ERROR")==0) {//not exist in LSF
               req->options |=SUB_NOTIFY_END; //send mail after execution or failure of the job
           } else if(notification.compare("ALL")==0) {
               req->options |=SUB_NOTIFY_BEGIN;
               req->options |=SUB_NOTIFY_END;
           } else {
                 throw UserException(ERRCODE_INVALID_PARAM, "In your script: "+notification+" is an invalid notification"
                               " type:"+" consult the vishnu user manuel");
           }
      }
      //To treat waill clock time limit
      if((pos=(*iter).find(wallTimeSyntax))!=std::string::npos) {
         std::string waillTime = (*iter).substr(pos+wallTimeSyntax.size());
         req->rLimits[LSF_RLIMIT_RUN] = vishnu::convertStringToWallTime(waillTime)/60;
      }
  }

}


/**
 * \brief Destructor
 */
LSFParser::~LSFParser(){
}

