/**
 * \file FMSVishnuException.hpp
 * \brief This file defines the FMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __FMSVISHNUEXCEPTION__
#define __FMSVISHNUEXCEPTION__

#include "UserException.hpp"

// Definitions of FMS specific error codes
static const int ERRCODE_INVALID_PATH = 201;
static const int ERRCODE_RUNTIME_ERROR = 202;
static const int ERRCODE_UNKNOWN_FILETRANSFERID = 203;



/**
 * \class FMSVishnuException
 * \brief Specialisation of the user exception for the FMS module
 */
class FMSVishnuException: public UserException{

  public:
  /**
   * \brief Default constructor
   */
  FMSVishnuException();
  /**
   * \brief Copy constructor
   * \param e: The exception to copy
   */
  FMSVishnuException(const FMSVishnuException& e);
  /**
   * \brief Constructor
   * \param msg: The message to set to the exception
   * \param msgComp: The complementary message to set
   */
  FMSVishnuException(int msg, std::string msgComp= "");
  /**
   * \brief Default constructor
   */
  ~FMSVishnuException() throw (){};

  /**
   * \brief Function to initialize all UMS generic messages
   */
  void
  initMsg();


};

#endif // FMSVishnuException
