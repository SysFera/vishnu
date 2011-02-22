/**
* \file utilServer.cpp
* \brief This file implements the utils functions of server objects
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 15/02/2011
*/

#include "utilServer.hpp"

/**
* \brief Function to convert a string to int
* \fn    std::string convertToString(const T& val);
* \param  val a value to convert to string
* \return int value of the corresponding string
*/
int utilServer::convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val);
  str >> intValue;
  return intValue;
}
/**
* \brief Function to get the string associated to SystemException
* \fn    buildExceptionString(SystemException& e)
* \param  e the exception caught
* \return int value of the corresponding string
*/
std::string
utilServer::buildExceptionString(VishnuException& e) {
  std::string errorInfo;

  //To get the error code associated to the exception follows by #
  errorInfo = utilServer::convertToString(e.getMsgI())+"#";

  //To get exception information
  errorInfo.append(e.getMsg());
  errorInfo.append(" ");
  errorInfo.append(e.what());

  return errorInfo;
}
/**
* \brief Function to get a random number
* \fn    int generate_numbers()
* \return the number generated
*/
int
utilServer::generate_numbers() {

  boost::mt19937 gen;
  gen.seed(static_cast<boost::uint32_t>(std::time(0)));
  boost::uniform_int<> dist(1, 100000);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > die(gen, dist);
  return die();

}

/**
* \brief Function to get a random number
* \fn    int isMonth (const char * s)
* \return
*/
int
utilServer::isMonth (const char * s) {
  return (s[0]=='M' && s[1]=='O' && s[2]=='N' && s[3]=='T' && s[4]=='H');
}
int
utilServer::isYear (const char * s) {
  return (s[0]=='Y' && s[1]=='E' && s[2]=='A' && s[3]=='R');
}
int
utilServer::isSite (const char * s) {
  return (s[0]=='S' && s[1]=='I' && s[2]=='T' && s[3]=='E');
}
int
utilServer::isType (const char * s) {
  return (s[0]=='T' && s[1]=='Y' && s[2]=='P' && s[3]=='E');
}
int
utilServer::isUName (const char * s) {
  return (s[0]=='U' && s[1]=='N' && s[2]=='A' && s[3]=='M' && s[4]=='E');
}
int
utilServer::isMaName (const char * s) {
  return (s[0]=='M' && s[1]=='A' && s[2]=='N' && s[3]=='A' && s[4]=='M' && s[5]=='E');
}
int
utilServer::isDay (const char * s) {
  return (s[0]=='D' && s[1]=='A' && s[2]=='Y');
}
int
utilServer::isCpt (const char * s) {
  return (s[0]=='C' && s[1]=='P' && s[2]=='T');
}

// Function to get the variables from the format and set their value
void
utilServer::getKeywords (int* size, Format_t* array, const char* format, int cpt, IdType type,
      std::string name, std::string site) {
  unsigned int i;
  *size = 0;

  char day[4];
  char month[4];
  char year[4];

  char buf[500];
  memset (buf, 0, 500);

  //the current time
  ptime now = microsec_clock::local_time();

  struct tm tmtime = to_tm(now);
  strftime (year, 3, "%y", &tmtime);
  strftime (month, 3, "%m", &tmtime);
  strftime (day, 3, "%d", &tmtime);

  /*struct tm = to_tm(ptime);
  year = std::string(now.date().year());
  month = std::string(now.date().month());
  day = std::string(now.date().day());
*/
  #ifdef DEBUG
  fprintf (stderr, "Temps : an ->%s<- \n", year.c_str());
  fprintf (stderr, "Temps : mois ->%s<- \n", month.c_str());
  fprintf (stderr, "Temps : jour ->%s<- \n", day.c_str());
#endif
  // Loop parsing for the variables, setting their position and their value
  for (i=0;i<strlen (format);i++){
    if (format[i]=='$'){
      if (isDay (format+i+1)){
  array[*size].start = i;
  array[*size].end = i+3;
  array[*size].value.append (std::string (day));
  (*size) ++;
      }else if (isMonth (format+i+1)){
  array[*size].value = std::string (month);
  array[*size].start = i;
  array[*size].end = i+5;
  (*size) ++;
      }else if (isYear (format+i+1)){
  array[*size].start = i;
  array[*size].end = i+4;
  array[*size].value = std::string (year);
  (*size) ++;
      }else if (isCpt (format+i+1)){
  char tmp[10];
  sprintf (tmp, "%d", cpt);
  array[*size].value = std::string (tmp);
  array[*size].start = i;
  array[*size].end = i+3;
  (*size) ++;
      }else if (isSite (format+i+1)){
  array[*size].value = site;
  array[*size].start = i;
  array[*size].end = i+4;
  (*size) ++;
      }else if (isMaName (format+i+1)){
  array[*size].value = name;
  array[*size].start = i;
  array[*size].end = i+6;
  (*size) ++;
      }else if (isUName (format+i+1)){
  array[*size].value = name;
  array[*size].start = i;
  array[*size].end = i+5;
  (*size) ++;
      }else if (isType (format+i+1)){
  switch (type){
    case 0 :
      array[*size].value = "M";
      break;
    case 1 :
      array[*size].value = "U";
      break;
    case 2 :
      array[*size].value = "J";
      break;
    case 3 :
      array[*size].value = "F";
      break;
  }
  array[*size].start = i;
  array[*size].end = i+4;
  (*size) ++;
      } else {
  SystemException e (ERRCODE_SYSTEM, "It is not possible to parse "+std::string(format));
  throw e;
      }
    }
  }
}

// Function to get the generated name
std::string
utilServer::getGeneratedName (const char* format, int cpt, IdType type,
      std::string name , std::string site ) {
  std::string res;
  res.clear ();
  res = std::string ("");
  int  i;
  int  size;
  Format_t *keywords;
  keywords = new Format_t[strlen(format)+1];
  getKeywords (&size, keywords, format, cpt, type, name, site); // Getting var and their value

  // Building the id using the format and the values of the var
  if (size>0)
    res.append (format, keywords[0].start);
  else
    res = std::string (format);
  for (i=0;i<size;i++){
    res.append (keywords[i].value);
    // If other variables
    if (*(format+keywords[i].end+1) != '\0' && i!=size-1)
      res.append (format+keywords[i].end+1, keywords[i+1].start-keywords[i].end-1);
    // If text after the variable
    else if (*(format+keywords[i].end+1) != '\0' ){
      res.append (format+keywords[i].end+1, strlen (format)-keywords[i].end-1);
    }
  }
  return res;
}
