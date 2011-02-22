/**
 * \file SystemException.hpp
 * \brief This file defines the super class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __SYSTEMEXCEPTION__
#define __SYSTEMEXCEPTION__

#include "VishnuException.hpp"

static const int ERRCODE_DIET = 1;
static const int ERRCODE_DBERR = 2;
static const int ERRCODE_DBCONN = 3;
static const int ERRCODE_SYSTEM = 4;
static const int ERRCODE_INVCOMP = 5;
static const int ERRCODE_INVMAPPER = 6;

class SystemException : public VishnuException {
  
public:

  /**
   * \brief Default constructor
   * \fn SystemException()
   */
  SystemException();
  /**
   * \brief Copy constructor
   * \fn SystemException(SystemException e)
   */
  SystemException(const SystemException& e);
  /**
   * \brief Constructor
   * \fn SystemException(int msg, string msgComp = "")
   */
  SystemException(int msg, std::string msgComp = "");
  /**
   * \brief Default destructor
   * \fn ~SystemException()
   */
  virtual ~SystemException() throw(){};
  /**
   * \brief To get the generic message
   * \fn std::string getMsg()
   * \return Returns the message
   */
  std::string
  getMsg() const;
  /**
   * \brief Function to get the exception type in string
   * \fn virtual std::string getTypeS()
   * \return Returns the type of the exception
   */
  std::string
  getTypeS() const;
  /**
   * \brief Function to initialize all the generic messages
   * \fn void initMsg()
   */
  void
  initMsg();
};

#endif // VISHNUEXCEPTION
