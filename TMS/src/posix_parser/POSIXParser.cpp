/**
 * \file POSIXParser.cpp
 * \brief This file contains the implementation for VISHNU of the posix parser class.
 * \author Olivier Mornard (olivier.mornard@sysfera.com)
 * \date January 2013
 */

#include "POSIXParser.hpp"

#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

#include "utilVishnu.hpp"


#include <stdarg.h>

using namespace std;

static void
Debug(const char *s, ...) {
	va_list va_alist;
	char buf[256];

  va_start(va_alist, s);
  vsnprintf(buf, sizeof(buf), s, va_alist);
  va_end(va_alist);

  write(1,buf,strlen(buf));
}


static bool
GetNextValeur(istream& file,string& valeur) {
  char nextchar;

  while (file.get(nextchar)) {
    if (nextchar=='\n') {
      return false;
    }
    if (!isspace(nextchar)) {
      break;
    }
  }

  if (file.eof()) {
    return false;
  }

  valeur.push_back(nextchar);

  while (file.get(nextchar)) {
    if (nextchar=='\n') {
      file.unget();
      break;
    }
    if (isspace(nextchar)) {
      break;
    }

    valeur.push_back(nextchar);
  }
  return true;
}

static bool
GetNextEqualSign(istream& file) {
  char nextchar;

  while (file.get(nextchar)) {
    if ((nextchar=='=') || (nextchar=='\n')) {
      break;
    }
  }
  if (nextchar=='\n') {
    return false;
  }
  if (file.eof()) {
    return false;
  }
  return true;
}

static bool
GetNextToken(istream& file,string& keyname) {
  char nextchar;

  // Look for the first char of directive name
  while (file.get(nextchar)) {
    if (isalnum(nextchar) || (nextchar=='_')) {
      keyname.push_back(nextchar);
      break;
    }
  }
  if (nextchar=='\n') {
    return true;
  }
  if (file.eof()) {
    return false;
  }

  // Scan the directive name
  while (file.get(nextchar)) {
    if (!isalnum(nextchar) && (nextchar!='_')) {
      break;
    }
    keyname.push_back(nextchar);
  }

  file.unget();

  if (file.eof()) {
    return false;
  }

  return true;
}

static bool
GetNextLine(istream& file, Definition& current,bool& valide) {
  char nextchar;

  valide=false;
  if (!file.get(nextchar)) {
    return false;
  }

  if (nextchar == '\n') {
    return true;
  }

  if (nextchar == '#')
  {
    if (!file.get(nextchar)) {
      return false;
    }
    if (nextchar == '%')
    {
      if (!GetNextToken(file,current.key)) {
        return false;
      }
      if (!GetNextEqualSign(file)) {
        return false;
      }
      if (!GetNextValeur(file,current.value)) {
        return false;
      }
      valide=true;
    }
    else {
      file.unget();
    }
  }

  while (file.get(nextchar)) {
    if (nextchar == '\n') {
      break;
    }
  }
/**
  if (file.eof()) {
    return false;
  }
**/
  return true;
}

static bool
GetNextDefinition(istream& file, Definition& def) {
  bool valide;

  def.key.clear();
  def.value.clear();

  while (GetNextLine(file,def,valide)){
    if (valide) {
      transform(def.key.begin(), def.key.end(), def.key.begin(), ::tolower);
      return true;
    }
  }
  return false;
}


void JobCtx::AddDefinition(Definition Current) {
  if (Current.key == "vishnu_working_dir") {
    vishnu_working_dir = Current.value;
    return;
  }
  if (Current.key == "vishnu_job_name") {
    vishnu_job_name = Current.value;
    return;
  }
  if (Current.key == "vishnu_output") {
    vishnu_output = Current.value;
    return;
  }
  if (Current.key == "vishnu_error") {
    vishnu_error = Current.value;
    return;
  }
  if (Current.key == "vishnu_wallclocklimit") {
    Debug("Current WALLCLOCKLIMIT:%s.\n",Current.value.c_str());
//    vishnu_wallclocklimit = vishnu::convertToTimeType(Current.value); // non Safe!
    vishnu_wallclocklimit = std::atoi(Current.value.c_str());
    Debug("Conversion reussie.\n");
    return;
  }
}

bool
ParseCommand(char* Command, JobCtx& Context) {
  ifstream file;
  Definition def;

  Debug("Parser:%s.\n",Command);
  file.open(Command);

  while (GetNextDefinition(file,def)) {
    Debug("Definition :%s.\n",def.key.c_str());
    Debug("Valeur:%s.\n",def.value.c_str());
    Context.AddDefinition(def);
  };

  file.close();
  return true;
}

bool
ParseString()
{
  string Test="Une ligne\nDeux ligne\n#@vishnu_workingdir    = /tmp/toto\n\n";
  stringstream FluxTest(Test);
  Definition def;

  while (GetNextDefinition(FluxTest,def)) {
    cout << "[" << def.key << "]:" << def.value <<  '#' << endl;
  };
  return true;
}
