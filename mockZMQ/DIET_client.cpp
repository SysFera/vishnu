//
// DIET_client.c
// Implémentation du mock de la couche DIET par ZMQ dans VISHNU pour UMS
// Le 02/05/2012
// Auteur K. COULOMB
//

#include "DIET_client.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>


diet_profile_t*
diet_profile_alloc(const char* name, int IN, int INOUT, int OUT) {
  diet_profile_t* res = (diet_profile_t*) malloc(sizeof(diet_profile_t)*1);
  res->IN = IN;
  res->INOUT = INOUT;
  res->OUT = OUT;
  res->param = (char **)malloc(sizeof (char *)*(IN+INOUT+OUT));
  memset(res->param,0,(IN+INOUT+OUT));
  res->name = (char *)malloc (sizeof(char) * (strlen(name)+1));
  memcpy(res->name, name, strlen(name));
  res->name[strlen(name)]='\0';
  return res;
}

int
diet_string_set(diet_arg_t* arg, char* value, int pers){
  if (value) {
    arg->prof->param[arg->pos] = (char *)malloc(sizeof(char)*(strlen(value)+1));
    memcpy(arg->prof->param[arg->pos], value, strlen(value));
    (arg->prof->param[arg->pos])[strlen(value)] = '\0';
//    std::cout << " Setting val to : " << arg->prof->param[arg->pos] << " for pos " << arg->pos << std::endl;
//    std::cout << " Setting val orig to : " << value << " for pos " << arg->pos << std::endl;
  } else {
    arg->prof->param[arg->pos] = (char *)malloc(sizeof(char)*(strlen("")+1));
    memcpy(arg->prof->param[arg->pos], "", strlen(""));
    (arg->prof->param[arg->pos])[strlen("")] = '\0';
  }
  return 0;
}

int
diet_call(diet_profile_t* prof){
  zmq::context_t ctx(1);
  zmq::socket_t sock(ctx, ZMQ_REQ);
  sock.connect("tcp://localhost:5555");
//  std::cerr << "send: \"" << my_serialize(prof).c_str() << std::endl;
  std::string s1 = my_serialize(prof);
  zmq::message_t request(s1.length()+1);
  memcpy((void*)request.data(), s1.c_str(), s1.length()+1);
  sock.send(request);

  zmq::message_t reply;
  sock.recv(&reply);
//  std::cout << "Client receive : " << (char*)reply.data() << std::endl;

  boost::shared_ptr<diet_profile_t>
    tmp(my_deserialize(static_cast<char *>(reply.data())));
//  *prof =
  prof->IN = tmp->IN;
  prof->OUT = tmp->OUT;
  prof->INOUT = tmp->INOUT;
  for(int i=0;i<=prof->OUT;++i){
    prof->param[i] = strdup(tmp->param[i]);
  }

  return 0;
}

int
diet_string_get(diet_arg_t* arg, char** value, void* ptr){
//  std::cout << "Getting arg in pos :" << arg->pos << " of value: " << arg->prof->param[arg->pos] << std::endl;
  *value = (char *)malloc((strlen(arg->prof->param[arg->pos])+1)*sizeof (char));
  memcpy(*value, arg->prof->param[arg->pos], strlen(arg->prof->param[arg->pos]));
  (*value)[strlen(arg->prof->param[arg->pos])]='\0';
//  std::cout << "Value set :" << *value << std::endl;
//  std::cout << " Getting val to : " << arg->prof->param[arg->pos] << "for pos " << arg->pos << std::endl;
  return 0;
}

int
diet_profile_free(diet_profile_t* prof){
  return 0;
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
  std::stringstream res;
  res << prof->name <<  "$$$"
      << prof->IN << "$$$"
      << prof->INOUT << "$$$"
      << prof->OUT << "$$$";
  for (int i = 0; i<(prof->OUT); ++i) {
    res << prof->param[i] << "$$$";
  }
  res << prof->param[(prof->OUT)];
  return res.str();
}

boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof){
  boost::shared_ptr<diet_profile_t> res;
  std::vector<int> vec;

  std::vector<std::string> vecString;
  boost::algorithm::split_regex(vecString, prof, boost::regex("\\${3}"));

  if (!vecString.empty()) {
    res.reset(new diet_profile_t);
    std::vector<std::string>::iterator it = vecString.begin();
    res->name = strdup((it++)->c_str());
    res->IN = boost::lexical_cast<int>(*(it++));
    res->INOUT = boost::lexical_cast<int>(*(it++));
    res->OUT = boost::lexical_cast<int>(*(it++));
    res->param = (char**)malloc(sizeof(char*) * vecString.size() - 4);
    for (int i = 0; it != vecString.end(); it++, i++) {
      res->param[i] = strdup(it->c_str());
    }
  }

  return res;
}

int
diet_initialize(const char* cfg, int argc, char** argv){
  return 0;
}

int
diet_finalize(){
  return 0;
}
