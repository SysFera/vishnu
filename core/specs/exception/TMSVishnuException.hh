/**
 * \file TMSVishnuException.hh
 * \brief This file defines the TMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __TMSVISHNUEXCEPTION__
#define __TMSVISHNUEXCEPTION__

#include "VishnuException.hh"
#include <map>

static const int TNBERR = 5;

class TMSVishnuException: public VishnuException{
private :
  std::map<int, std::string> mp;
protected:
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
  TMSVishnuException(TMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  TMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~TMSVishnuException()
   */
  ~TMSVishnuException() throw (){};
  /**
   * \brief Function to get the generic message in the TMS part depending on the inherited mval integer
   * \fn getMsg
   * \return Returns the corresponding generic message
   */
  std::string getMsg();


  
};

#endif // TMSVishnuException
