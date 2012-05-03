#include <iostream>
#include "ServerUMS.hpp"
#include "internalApi.hpp"

using namespace std;

void callbackTest()
{
  ServerUMS* umsserver = ServerUMS::getInstance();

  diet_profile_t profileToSerialize;
  profileToSerialize.IN = 0;
  profileToSerialize.INOUT = 0;
  profileToSerialize.OUT = 0;
  profileToSerialize.param = (char**)(malloc(sizeof(char *)*3));

  profileToSerialize.name = (char*)(malloc(sizeof(char) * strlen("sessionConnect")));
  memcpy(profileToSerialize.name, "sessionConnect", strlen("sessionConnect"));

  umsserver->init();
  umsserver->call(&profileToSerialize);
  //TODO find a way to do an assert
  //TODO Create and manage Exception
}


int main(){
  callbackTest();
}
