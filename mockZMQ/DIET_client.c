//
// DIET_client.c
// Implémentation du mock de la couche DIET par ZMQ dans VISHNU pour UMS
// Le 02/05/2012
// Auteur K. COULOMB 
//

#include "DIET_client.h"

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
  zmsg * request = new zmsg(my_serialize(prof));
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
  for (int i = 0; i<(prof->OUT); ++i) {
    res += std::string(prof->param[i]);
    res += "#";
  }
  return res;
}

diet_profile_t*
my_deserialize(std::string prof){
  diet_profile_t* res = (diet_profile_t *) malloc(sizeof(diet_profile_t)*1);
  std::vector<int> vec;

  int size;

  // Getting all the separators
  size_t cpt = 0;
  do{
    cpt=s.find_first_of("#", cpt);
    if (cpt ==string::npos){
      break;
    }
    vec.push_back(cpt);
    cpt++;
  }
  while ((true));

  // All param
  res->param = (char **)malloc(sizeof(char *)*vec.size);
  prec = 0;
  
  // Setting first
  res->param[0] = (char *)malloc(sizeof(char)*(vec.at(0)));
  memcpy(res->param[0], prof.substr(prec, vec.at(0)));
  (res->param[0])[vec.at(0)]='\0';
  prec = vec.at(0);

  // For each word
  for (int i = 0;i<vec.size();++i){
    if (i==vec.size()-1){
      next = prof.size();
    } else {
      next = vec.at(i+1);
    }
    size = (next-vec.at(i)); 
    res->param[i+1] = (char *)malloc(sizeof(char)*(size));
    memcpy(res->param[i+1], prof.substr(prec+1, size-1));
    (res->param[i+1])[size]='\0';
    prec = vec.at(i+1);
  }
  
  return res;

}
