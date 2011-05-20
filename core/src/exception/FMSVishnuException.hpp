/**
 * \file FMSVishnuException.hh
 * \brief This file defines the FMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __FMSVISHNUEXCEPTION__
#define __FMSVISHNUEXCEPTION__

#include "UserException.hpp"

// Definitions of FMS specific error codes
static const int ERRCODE_UNKNOWN_PATH = 201;
static const int ERRCODE_RUNTIME_ERROR = 202;
static const int ERRCODE_UNKNOWN_FILETRANSFERID = 203;



class FMSVishnuException: public UserException{

  public:
  /**
   * \brief Default constructor
   * \fn FMSVishnuException()
   */
  FMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  FMSVishnuException(const FMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  FMSVishnuException(int msg, std::string msgComp= "");
  /**
   * \brief Default constructor
   * \fn ~FMSVishnuException()
   */
  ~FMSVishnuException() throw (){};

  /**
   * \brief Function to initialize all UMS generic messages
   * \fn void initMsg()
   */
  void
  initMsg();


};

#endif // FMSVishnuException
