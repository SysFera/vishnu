#include "DIET_client.h"
#include <string.h>
#include <stdio.h>
#include <string>

using namespace std;

void testDeserializeMethodConnect()
{
   diet_profile_t* profileDeserialied  = my_deserialize("connect#user1#password1#option1");
   
   if( strcmp(profileDeserialied->param[0],"user1") != 0)
     {
       cerr << "Echec user. Resultat : " << profileDeserialied->param[0] << " Expected : " << "user1"<< endl;
       
     }
   else if( strcmp(profileDeserialied->param[1],"password1") != 0)
     {
       cerr << "Echec user. Resultat : " << profileDeserialied->param[1] << " Expected : " << "password1"<< endl;
       
     }
   else if( strcmp(profileDeserialied->param[2],"option1") != 0)
     {
       cerr << "Echec user. Resultat : " << profileDeserialied->param[2] << " Expected : " << "option1"<< endl;
       
     }
   else 
     {
       cout << "testDeserializeMethodConnect PASS !" << endl;
     }

   
}

void testSerizalizeMethodConnect()
{
  std::string resultExpected = "connect#user1#password1#option1";

  diet_profile_t profileToSerialize;
  profileToSerialize.IN = 3;
  profileToSerialize.INOUT = 3;
  profileToSerialize.OUT = 3;
  profileToSerialize.param = (char**)(malloc(sizeof(char *)*3));

  profileToSerialize.name = (char*)(malloc(sizeof(char) * strlen("connect")));
  memcpy(profileToSerialize.name, "connect", strlen("connect"));
  profileToSerialize.param[0] = (char*)(malloc(sizeof(char) * strlen("user1")));
  memcpy(profileToSerialize.param[0], "user1", strlen("user1"));
  profileToSerialize.param[1] = (char*)(malloc(sizeof(char) * strlen("password1")));
  memcpy(profileToSerialize.param[1], "password1", strlen("password1"));
  profileToSerialize.param[2] = (char*)(malloc(sizeof(char) * strlen("option1")));
  memcpy(profileToSerialize.param[2], "option1", strlen("option1"));

  std::string result = my_serialize(&profileToSerialize);
  if(result.compare(resultExpected) != 0) printf("Erreur\n");
  else printf("PASS\n");

  //TODO free
 
}
int 
main(int argc, char** argv)
{
  testSerizalizeMethodConnect();
  testDeserializeMethodConnect();
  
  return EXIT_SUCCESS;
}
