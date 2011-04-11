/**
 * \file TMSVishnuException.hh
 * \brief This file defines the TMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __TMSVISHNUEXCEPTION__
#define __TMSVISHNUEXCEPTION__

#include "UserException.hpp"

// Definitions of TMS specific error codes
static const int ERRCODE_UNKNOWN_BATCH_SCHEDULER = 101;
static const int ERRCODE_BATCH_SCHEDULER_ERROR = 102;
static const int ERRCODE_UNKNOWN_JOBID = 103;
static const int ERRCODE_CANT_CANCEL = 104;
static const int ERRCODE_TMS_NOT_ADMIN = 105;




class TMSVishnuException: public UserException{

  public:
  /**
   * \brief Default constructor
   * \fn TMSVishnuException()
   */
  TMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(const TMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(int msg, std::string msgComp= "");
  /**
   * \brief Default constructor
   * \fn ~TMSVishnuException()
   */
  ~TMSVishnuException() throw (){};
  
  /**
   * \brief Function to initialize all UMS generic messages
   * \fn void initMsg()
   */
  void
  initMsg();

  
};

#endif // TMSVishnuException
