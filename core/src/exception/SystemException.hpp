/**
 * \file SystemException.hpp
 * \brief This file defines the super class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __SYSTEMEXCEPTION__
#define __SYSTEMEXCEPTION__

#include <string>
#include "VishnuException.hpp"

// RESERVED CODES FROM 1 TO 9 plus negative values
// TODO describe the error codes
static const int ERRCODE_AUTHENTERR = -1;
static const int ERRCODE_COMMUNICATION = 1;
static const int ERRCODE_DBERR = 2;
static const int ERRCODE_DBCONN = 3;
static const int ERRCODE_SYSTEM = 4;
static const int ERRCODE_INVCOMP = 5;
static const int ERRCODE_INVMAPPER = 6;
static const int ERRCODE_INVEXCEP = 7;
static const int ERRCODE_INVDATA = 8;
static const int ERRCODE_SSH = 9;

/**
 * \brief This class represents the exception that are due to the system
 * \class SystemException
 */
class SystemException : public VishnuException {

public:

  /**
   * \brief Default constructor
   */
  SystemException();
  /**
   * \brief Copy constructor
   */
  SystemException(const SystemException& e);
  /**
   * \brief Constructor
   */
  SystemException(int msg, std::string msgComp = "");
  /**
   * \brief Default destructor
   */
  virtual ~SystemException() throw(){};
  /**
   * \brief To get the generic message
   * \return Returns the message
   */
  std::string
  getMsg() const;
  /**
   * \brief Function to get the exception type in string
   * \return Returns the type of the exception
   */
  std::string
  getTypeS() const;
  /**
   * \brief Function to initialize all the generic messages
   */
  void
  initMsg();
};

#endif // VISHNUEXCEPTION
