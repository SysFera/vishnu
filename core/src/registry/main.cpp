// Sample test to use the registry system with polymorphism
// Author : kevin coulomb (kevin.coulomb@sysfera.com)
// Date   : 09/02/2011

#include <iostream>
#include <sstream>

#include "MapperRegistry.hpp"
#include "Mapper.hpp"
#include "Mapper_impl.hpp"
#include "Mapper_impl2.hpp"

using namespace std;

int main(int argc, char** argv){
  string name = string("toto");
  int i, j;
  string command, key;
  string titi;
  if (argc>1)
    name = string(argv[1]);
  MapperRegistry* mr = MapperRegistry::getInstance();
  Mapper_impl  mapper ;
  Mapper_impl2 mapper2;
  Mapper* m = NULL;

  mapper  = Mapper_impl(mr, name+"0");
  mapper.registerMapper();
  mapper2 = Mapper_impl2(mr, name+"1");
  mapper2.registerMapper();

  for (i=0; i<6;i++){
    ostringstream tmp2;
    ostringstream tmp;
    tmp << (i%2);
    titi = name;
    titi += tmp.str();
    tmp2 << (i%5+1);
    key = tmp2.str();
    m = mr->getMapper(titi);
    m->getCommand(key, command);
    cout << " key = " << key << " and command = " << command << " for mapper " << titi << endl;
  }
}
