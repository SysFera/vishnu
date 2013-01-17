
#include "POSIXParser.hpp"

#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "utilsVishnu.hpp"

using namespace std;


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
  
  while (GetNextLine(file,def,valide)){
    if (valide) {
      transform(def.key.begin(), def.key.end(), def.key.begin(), ::tolower);
      return true;
    }
  }
  return false;
}


void JobCtx::AddDefinition(Definion Current) {
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
    vishnu_wallclocklimit = convertToTimeType(Current.value);
    return;
  }
  if (Current.key == "vishnu_nbnodesandcpupernode") {
    vishnu_nbNodesAndCpuPerNode = convertToInt(Current.value);
    return;
  }
  if (Current.key == "vishnu_memory") {
    vishnu_memory = convertToInt(Current.value);
    return;
  }
}

bool
ParseCommand(char* Command, JobCtx& Context) {
  ifstream file;
  Definition def;
  
  file.open(Command);

  while (GetNextDefinition(file,def)) {
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
