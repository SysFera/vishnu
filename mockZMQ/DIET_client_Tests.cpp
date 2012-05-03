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

void diet_profile_allocTest()
{
  diet_profile_t profileExpected;
 
  profileExpected.name = (char*)(malloc(sizeof(char) * strlen("connect")));
  memcpy(profileExpected.name, "connect", strlen("connect"));

  diet_profile_t* profile = diet_profile_alloc("connect",1,1,1);

  if(strcmp(profile->name,profileExpected.name) == 0) printf("Test diet_profile_allocTest OK\n");
  else printf ("diet_profile_allocTest ERREUR\n");
  

}

void diet_parameter_Test()
{
  int pos = 1;
  diet_profile_t prof;
 
  prof.name = (char*)(malloc(sizeof(char) * strlen("connect")));
  memcpy(prof.name, "connect", strlen("connect"));

  diet_arg_t* res = diet_parameter(&prof, pos);
  if(res->prof == &prof && res->pos == 1 )
    {
      printf("Test diet_parameter_Test OK\n");
    }
      else printf ("diet_parameter_Test ERREUR\n");

}

void diet_string_set_Test()
{
  int pos = 0;
  diet_profile_t prof;

  prof.name = (char*)(malloc(sizeof(char) * strlen("connect")));
  memcpy(prof.name, "connect", strlen("connect"));
  prof.IN = 1;
  prof.INOUT = 1;
  prof.OUT = 1;
  prof.param = (char **) malloc (sizeof(char*)*1);

  diet_arg_t* res = diet_parameter(&prof, pos);
  
  diet_string_set(res,"test",1);
  if(strcmp(prof.param[0],"test")==0)
    {
      printf("Test diet_string_set_Test OK\n");
    }
  else printf ("diet_string_set_Test ERREUR. Value : ->%s<- Expected : test\n",prof.param[0]);

  
}

int 
main(int argc, char** argv)
{
  testSerizalizeMethodConnect();
  testDeserializeMethodConnect();
  diet_profile_allocTest();
  diet_parameter_Test();
  diet_string_set_Test();
  return EXIT_SUCCESS;
}
