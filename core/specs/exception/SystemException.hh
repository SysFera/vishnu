/**
 * \file SystemException.hh
 * \brief This file defines the super class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __SYSTEMEXCEPTION__
#define __SYSTEMEXCEPTION__

#include "VishnuException.hh"

class SystemException :
  public VishnuException {
public:

typedef enum ExceptionType{
  UMS  = 0 ,
  TMS  = 1 ,
  FMS  = 2 , 
  IMS  = 3 ,
  NONE = 4   
} ExceptionType;

private :

protected :

public :

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
