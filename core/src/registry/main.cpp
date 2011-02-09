#include <iostream>
#include <sstream>

#include "MapperRegistry.hpp"
#include "Mapper_impl.hpp"

using namespace std;

int main(int argc, char** argv){
  string name = string("toto");
  int i, j;
  string command, key;
  string titi;
  if (argc>1)
    name = string(argv[1]);
  MapperRegistry* mr = MapperRegistry::getInstance();
  Mapper_impl mapper[6];
  for (i=0;i<6;i++){
    ostringstream tmp;
    tmp << (i%4);
    titi = name;
    titi += tmp.str();
    if (!(i%2)){
      istringstream tmp2(command);
      tmp2 >> j;
      tmp << (j%3);
      key = tmp.str();
    }
    mapper[i] = Mapper_impl(mr, titi);
    cout << "Adding mapper for module : " << titi << endl;
    mapper[i].registerMapper();
    if (i%2)
      mapper[i].getCommand(key, command);
  }

}
