/**
 * \file RequestFactory.hpp
 * \brief This file defines the virtual database request factory
 * each constant corresponding to a request will be prefixed by VR for VishnuRequest
 * \author Kevin Coulomb (kevin.coulomb@sysfera.com)
 * \date 04/06/13
 */

#ifndef _REQUESTFACTORY_H_
#define _REQUESTFACTORY_H_

#include <string>
#include <map>

/**
 * \brief Request for updating account according to users key
 * The syntax between mysql and postgresql is not compatible
 * mysql allows "update t1, t2 set ..." and does not accept from keywords
 * whereas postgresql accept whereas and can't stand "update t1, t2"
 */
const int VR_UPDATE_ACCOUNT_WITH_USERS = 1;


/**
 * \class RequestFactory
 * \brief Request for database factory
 */
class RequestFactory{
public:
  /**
   * \brief Constructor
   */
  RequestFactory();
  /**
   * \brief Destructor
   */
  ~RequestFactory();
  /**
   * \brief To get a request from a request file based on a key
   * \param key the key indicating the request to get
   * \return the corresponding sql request
   */
  virtual std::string
  get(const int key) = 0;

protected:
  std::map<int, std::string> mrequest;

};

#endif // REQUESTFACTORY
