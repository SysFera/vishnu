/**
 * \file VishnuException.hh
 * \brief This file defines the super class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __VISHNUEXCEPTION__
#define __VISHNUEXCEPTION__

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <exception>

class VishnuException :
  public std::exception {
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

  
  /**
   *  \brief A complementatry message to the exception
   */
  std::string   mmsgc;
  /**
   *  \brief The type of the exception (UMS, TMS, FMS, IMS, NONE)
   */
  ExceptionType mtype;
  /**
   *  \brief An exception code refereing to a generic message. -1 means no generic error message.
   */
  int           mval;
public :

  /**
   * \brief Default constructor
   * \fn VishnuException()
   */
  VishnuException();
  /**
   * \brief Copy constructor
   * \fn VishnuException(VishnuException e)
   */
  VishnuException(VishnuException& e);
  /**
   * \brief Constructor
   * \fn VishnuException(int msg, string msgComp = "")
   */
  VishnuException(int msg, std::string msgComp = "");
  /**
   * \brief Default destructor
   * \fn ~VishnuException()
   */
  ~VishnuException() throw(){};
  /**
   * \brief To get the type of the exception in integer
   * \fn getTypeI()
   * \return Returns the type of the exception
   */
  ExceptionType
  getTypeI(){return mtype;}
  /**
   * \brief Function to get the exception type in string
   * \fn getTypeS()
   * \return Returns the type of the exception
   */
  std::string
  getTypeS();
  /**
   * \brief To get a complement to the message
   * \fn getMsgDetail()
   * \return Returns a complement to the generic error message
   */
  std::string
  getMsgComp(){return mmsgc;}
  /**
   * \brief To get the generic message
   * \fn getMsg()
   * \return Returns the message
   */
  virtual std::string
  getMsg(){return "";}
  /**
   * \brief Function to set the type of the exception
   * \fn setType(ExceptionType t)
   * \param t The type of the exception
   */
  void
  setType(ExceptionType t){mtype = t;}
  /**
   * \brief Function to set the complement of a message
   * \fn setMsgComp(std::string m)
   * \param m The complement of the message
   */
  void
  setMsgComp(std::string m){mmsgc = m;}
  /**
   * \brief Function to set the generic message
   * \fn setMsg(int val)
   * \param val The val of the msg
   */
  void
  setMsg(int val){mval = val;}
  /**
   * \brief Function to append a complement to the detailled message
   * \fn appendMsgComp(string s)
   * \param s The message to append
   */
  void
  appendMsgComp(std::string s);
  /**
   * \brief To get the mval as an int
   * \fn getMsgI()
   */
  int
  getMsgI();
};

#endif // VISHNUEXCEPTION
