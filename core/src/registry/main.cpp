// Sample test to use the registry system with polymorphism
// Author : kevin coulomb (kevin.coulomb@sysfera.com)
// Date   : 09/02/2011

#include <iostream>
#include <sstream>

#include "MapperRegistry.hpp"
#include "Mapper.hpp"
#include "UMSMapper.hpp"

using namespace std;

int main(int argc, char** argv){
  string name = string("toto");
  int i, j;
  string command, key;
  string titi;
  if (argc>1)
    name = string(argv[1]);
  MapperRegistry* mr = MapperRegistry::getInstance();
  UMSMapper mapper = UMSMapper(mr, "UMS");
//   Mapper* m = NULL;

//   mapper  = Mapper_impl(mr, "UMS");
   mapper.registerMapper();
  string s;
  cout << " XXXXXXXXXXXXXXXXXXX" << endl;
  for (i=0; i<6;i++){
//     ostringstream tmp2;
//     ostringstream tmp;
//     tmp << (i%2);
//     titi = name;
//     titi += tmp.str();
//     tmp2 << (i%5+1);
//     key = tmp2.str();
//    m = mr.getMapper("UMS");
    //    m.getCommand(key, command);
    //    cout << " key = " << key << " and command = " << command << " for mapper " << titi << endl;
    try{
      cout << "code value2 " << VISHNU_CLOSE << endl;
      j = mapper.code("vishnu_close")  ;
      s = mapper.finalize(j)         ;
      cout << "Res coded :" << s << endl;
      cout << "Res decoded :" <<  mapper.decode(s) << endl;
    }catch (SystemException *e){
      cout << "msg : " << e->what() << endl;
    }
  }
}
