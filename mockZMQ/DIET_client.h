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

/**
 * \brief Overload of DIET internal structure
 */
typedef struct toto {
  char* id;
} toto;

/**
 * \brief Overload of DIET structure
 */
typedef struct diet_arg_t {
  /**
   * \brief Overload of DIET param, position of the argument
   */
  int pos;
  /**
   * \brief Overload of DIET param, profile
   */
  void* prof;
  /**
   * \brief Overload of DIET param, description of the argument
   */
  toto desc;
} diet_arg_t;

/**
 * \brief Overload of DIET structure
 */
typedef struct diet_profile_t {
  /**
   * \brief Overload of DIET param, array of parameters
   */
  char ** param;
  /**
   * \brief Overload of DIET param, last IN param in the array
   */
  int IN;
  /**
   * \brief Overload of DIET param, last INOUT param in the array
   */
  int INOUT;
  /**
   * \brief Overload of DIET param, last OUT param in the array
   */
  int OUT;
  /**
   * \brief Overload of DIET param, name of the service
   */
  char* name;
  /**
   * \brief Overload of DIET param
   */
  diet_arg_t* parameters;
} diet_profile_t;


/**
 * \brief Overload of DIET structure
 */
typedef struct diet_container_t {
  /**
   * \brief Overload of DIET param, size of the container
   */
  int size;
  /**
   * \brief Overload of DIET param, id of elements in the container
   */
  char* elt_ids[2];
} diet_container_t;


/**
 * \brief Overload of DIET constant
 */
#define DIET_VOLATILE 1
/**
 * \brief Overload of DIET constant
 */
#define DIET_PERSISTENT_RETURN 1

/**
 * \brief Overload of DIET function, allocate the profile of a service
 * \param name The name of the service
 * \param IN number of IN parameters
 * \param INOUT number of IN+INOUT parameters
 * \param OUT number of IN+INOUT+OUT parameters
 * \return The allocated profile
 */
diet_profile_t*
diet_profile_alloc(const char* name, int IN, int INOUT, int OUT);

/**
 * \brief Overload of DIET function, set a param value in a profile to a string(char *)
 * \param arg Structure containing the string param to set and its position
 * \param value The value to set
 * \param pers The persistency of the data (DEPRECATED, kept for compatibility)
 * \return 0 on success, an error code otherwise
 */
int
diet_string_set(diet_arg_t* arg, char* value, int pers);

/**
 * \brief Overload of DIET function, call to a DIET service
 * \param prof The profile of the service to call
 * \return 0 on success, an error code otherwise
 */
int
diet_call(diet_profile_t* prof);

/**
 * \brief Generic function created to encapsulate the code
 */
int
diet_call_gen(diet_profile_t* prof, const std::string& uri);

/**
 * \brief Overload of DIET function, get the value of a string(char *) in the profile
 * \param arg Structure containing the argument
 * \param value OUT the value gotten
 * \param ptr DEPRECATED, kept for compatibility
 * \return 0 on success, an error code otherwise
 */
int
diet_string_get(diet_arg_t* arg, char** value, void* ptr);

/**
 * \brief Overload of DIET function, free an allocated profile
 * \param prof The profile to free
 * \return 0 on success, an error code otherwise
 */
int
diet_profile_free(diet_profile_t* prof);

/**
 * \brief Overload of DIET function, to get the arg in position pos in the profile prof
 * \param prof The profile
 * \param pos The position of the desired parameter
 * \return Structure describing the argument
 */
diet_arg_t*
diet_parameter(diet_profile_t* prof, int pos);

/**
 * \brief To deserialize a profile
 * \param prof The serialized profile
 * \return The deserialized profile
 */
boost::shared_ptr<diet_profile_t>
my_deserialize(const std::string& prof);

/**
 * \brief To serialize a profile
 * \param prof The profile
 * \return The serialized profile
 */
std::string
my_serialize(diet_profile_t* prof);

/**
 * \brief Overload of DIET function, to initialize
 * \param cfg DEPRECATED, kept for compatibility
 * \param argc DEPRECATED, kept for compatibility
 * \param argv DEPRECATED, kept for compatibility
 * \return 0 on success, an error code otherwise
 */
int
diet_initialize(const char* cfg, int argc, char** argv);

/**
 * \brief Overload of DIET function, to finalize the call
 * \return 0 on success, an error code otherwise
 */
int
diet_finalize();

/**
 * \brief Overload of DIET function
 * \param arg ?
 * \param flag ?
 * \return 0 on success, an error code otherwise
 */
int
diet_container_set(diet_arg_t* arg, int flag);

#endif // __DIETMOCK__
