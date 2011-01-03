/**
 * \file FMSVishnuException.hh
 * \brief This file defines the FMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __FMSVISHNUEXCEPTION__
#define __FMSVISHNUEXCEPTION__

#include "VishnuException.hh"

static const int FNBERR = 5;

class FMSVishnuException: public VishnuException{
private :
protected:
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
  FMSVishnuException(FMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  FMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~FMSVishnuException()
   */
  ~FMSVishnuException() throw(){};
  /**
   * \brief Function to get the generic message in the FMS part depending on the inherited mval integer
   * \fn getMsg
   * \return Returns the corresponding generic message
   */
  std::string getMsg();


  
};

#endif // FMSVishnuException
