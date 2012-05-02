//
// DIET_client.c
// Implémentation du mock de la couche DIET par ZMQ dans VISHNU pour UMS
// Le 02/05/2012
// Auteur K. COULOMB 
//

#include "DIET_client.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

diet_profile_t*
diet_profile_alloc(char* name, int IN, int INOUT, int OUT) {
  diet_profile_t* res = (diet_profile_t*) malloc(sizeof(diet_profile_t)*1);
  res->IN = IN;
  res->INOUT = INOUT;
  res->OUT = OUT;
  res->param = (char **)malloc(sizeof (char *)*(IN+INOUT+OUT));
  res->name = (char *)malloc (sizeof(char) * (strlen(name)+1));
  memcpy(res->name, name, strlen(name));
  res->name[strlen(name)]='\0';
  return res;
}

int
diet_string_set(diet_arg_t* arg, char* value, int pers){
  arg->prof->param[arg->pos] = (char *)malloc(sizeof(char)*(strlen(value)+1));
  memcpy(arg->prof->param[arg->pos], value, strlen(value));
  (arg->prof->param[arg->pos])[strlen(value)] = '\0';
  return SUCCESS;
}

int 
diet_call(diet_profile_t* prof){
  mdcli session ("tcp://localhost:5555", 0);
  zmsg * request = new zmsg(my_serialize(prof).c_str());
  zmsg * reply   = session.send(prof->name,request);
  if (reply) {
    delete reply;
  }
  return SUCCESS;
}

int
diet_string_get(diet_arg_t* arg, char** value, void* ptr){
  *value = (char *)malloc((strlen(arg->prof->param[arg->pos])+1)*sizeof (char));
  memcpy(*value, arg->prof->param[arg->pos], strlen(arg->prof->param[arg->pos]));
  (*value)[strlen(arg->prof->param[arg->pos])]='\0';
  return SUCCESS;
}

int 
diet_profile_free(diet_profile_t* prof){
  
  return SUCCESS;
}

diet_arg_t*
diet_parameter(diet_profile_t* prof, int pos){
  diet_arg_t* res = (diet_arg_t*) malloc(sizeof(diet_arg_t)*1);
  res->prof = prof;
  res->pos = pos;
  return res;
}


std::string
my_serialize(diet_profile_t* prof){
  std::string res = "";
  res += std::string(prof->name);
  res +=  "#";
  for (int i = 0; i<(prof->OUT)-1; ++i) {
    res += std::string(prof->param[i]);
    res += "#";
  }
  res += std::string(prof->param[(prof->OUT)-1]);
  return res;
}

diet_profile_t*
my_deserialize(std::string prof){
  diet_profile_t* res = (diet_profile_t *) malloc(sizeof(diet_profile_t)*1);
  std::vector<int> vec;

  int size, prec, next;

  // Getting all the separators
  size_t cpt = 0;
  do{
    cpt=prof.find_first_of("#", cpt);
    if (cpt ==std::string::npos){
      break;
    }
    vec.push_back(cpt);
    cpt++;
  }
  while ((true));

  // All param
  res->param = (char **)malloc(sizeof(char *)*vec.size());
  prec = 0;
  
  // Setting first
  res->name = (char *)malloc(sizeof(char)*(vec.at(0)));
  memcpy(res->name, prof.substr(prec, vec.at(0)).c_str(), strlen(prof.substr(prec, vec.at(0)).c_str()));
  (res->name)[vec.at(0)]='\0';
  prec = vec.at(0);

  // For each word
  for (unsigned int i = 0;i<vec.size();++i){
    if (i==vec.size()-1){
      next = prof.size();
    } else {
      next = vec.at(i+1);
    }
    size = (next-vec.at(i)); 
    res->param[i] = (char *)malloc(sizeof(char)*(size));
    memcpy(res->param[i], prof.substr(prec+1, size-1).c_str(), strlen(prof.substr(prec+1, size-1).c_str()));
    (res->param[i])[size]='\0';
    if(i < vec.size()-1)
      prec = vec.at(i+1);
  }
  
  return res;

}
