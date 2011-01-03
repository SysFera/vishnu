/**
 * \file UMSVishnuException.hh
 * \brief This file defines the UMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __UMSVISHNUEXCEPTION__
#define __UMSVISHNUEXCEPTION__

#include "VishnuException.hh"
#include <map>

static const int UNBERR = 5;

class UMSVishnuException: public VishnuException{
private :
  std::map<int, std::string> mp;
protected:
public:
  /**
   * \brief Default constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException(UMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  UMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~UMSVishnuException()
   */
  ~UMSVishnuException()throw (){};
  /**
   * \brief Function to get the generic message in the UMS part depending on the inherited mval integer
   * \fn getMsg
   * \return Returns the corresponding generic message
   */
  std::string getMsg();


  
};

#endif // UMSVishnuException
