/**
 * \file IMSVishnuException.hh
 * \brief This file defines the IMS sub class for the Vishnu exceptions.
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 */
#ifndef __IMSVISHNUEXCEPTION__
#define __IMSVISHNUEXCEPTION__

#include "VishnuException.hh"

static const int INBERR = 5;

class IMSVishnuException: public VishnuException{
private :
  std::map<int, std::string> mp;
protected:
public:
  /**
   * \brief Default constructor
   * \fn IMSVishnuException()
   */
  IMSVishnuException();
  /**
   * \brief Copy constructor
   * \fn UMSVishnuException()
   */
  IMSVishnuException(IMSVishnuException& e);
  /**
   * \brief Constructor
   * \fn UMSVishnuException()
   */
  IMSVishnuException(int msg, std::string msgComp);
  /**
   * \brief Default constructor
   * \fn ~IMSVishnuException()
   */
  ~IMSVishnuException() throw(){};
  /**
   * \brief Function to get the generic message in the IMS part depending on the inherited mval integer
   * \fn getMsg
   * \return Returns the corresponding generic message
   */
  std::string getMsg();


  
};

#endif // IMSVishnuException
