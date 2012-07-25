//
// DIET_client.h
// Mock de la couche DIET par ZMQ dans VISHNU pour UMS
// Le 02/05/2012
// Auteur K. COULOMB
//

#ifndef __DIETMOCK__
#define __DIETMOCK__

#include "mdcliapi.hpp"
#include <string>
#include <boost/shared_ptr.hpp>

typedef struct toto {
  char* id;
} toto;

typedef struct diet_arg_t {
  int pos;
  void* prof;
  toto desc;
} diet_arg_t;

typedef struct diet_profile_t {
  char ** param;
  int IN;
  int INOUT;
  int OUT;
  char* name;
  diet_arg_t* parameters;
} diet_profile_t;


typedef struct diet_container_t {
  int size;
  char* elt_ids[2];
} diet_container_t;


#define DIET_VOLATILE 1
#define DIET_PERSISTENT_RETURN 1

diet_profile_t*
diet_profile_alloc(const char* name, int IN, int INOUT, int OUT);

int
diet_string_set(diet_arg_t* arg, char* value, int pers);

int
diet_call(diet_profile_t* prof);

int
diet_call_gen(diet_profile_t* prof, int port);

int
diet_string_get(diet_arg_t* arg, char** value, void* ptr);

int
diet_profile_free(diet_profile_t* prof);

diet_arg_t*
diet_parameter(diet_profile_t* prof, int pos);

bool
isUMS(std::string test);
bool
isTMS(std::string test);
bool
isIMS(std::string test);

boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof);

std::string
my_serialize(diet_profile_t* prof);

int
diet_initialize(const char* cfg, int argc, char** argv);

int
diet_finalize();

void
dagda_init_container(diet_arg_t* arg);

void
dagda_put_file(char * path, int val, char** id);

void
dagda_get_file(char* val, char** err);

void
dagda_add_container_element(char* arg, char* id, int val);

void
dagda_get_container(char* IDContainer);

void
dagda_get_container_elements(char* IDContainer, diet_container_t* content);

int
diet_container_set(diet_arg_t* arg, int flag);

void
dagda_delete_data(char* id);

#endif // __DIETMOCK__
