/**
 * \file FMSVishnuException.hpp
 * \brief This file defines the FMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __FMSVISHNUEXCEPTION__
#define __FMSVISHNUEXCEPTION__

#include "UserException.hpp"
#ifdef __WIN32__
#define VISHNU_API_LIB __declspec(dllexport)
#else
#define VISHNU_API_LIB
#endif
// Definitions of FMS specific error codes
static const int ERRCODE_INVALID_PATH = 201;
static const int ERRCODE_RUNTIME_ERROR = 202;
static const int ERRCODE_UNKNOWN_FILETRANSFERID = 203;

/**
 * \brief A FMS error handling class
 */

/**
 * \class FMSVishnuException
 * \brief Specialisation of the user exception for the FMS module
 */
class VISHNU_API_LIB FMSVishnuException: public UserException{

  public:
  /**
   * \brief Default constructor
   */
  FMSVishnuException();
  /**
   * \brief Copy constructor
   * \param e another FMS excpetion object
   */
  FMSVishnuException(const FMSVishnuException& e);
  /**
   * \brief Constructor with paramters
   * \param  msg the error code
   * \param msgComp The complementary message to add to generic error message
   */
  FMSVishnuException(int msg, std::string msgComp= "");
  /**
   * \brief Default destructor
   */
  ~FMSVishnuException() throw (){};

  /**
   * \brief Function to initialize all FMS generic messages
   */
  void
  initMsg();


};

#endif // FMSVishnuException
