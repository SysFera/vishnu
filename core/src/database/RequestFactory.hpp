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
 * This request needs 2 arguments to be replaced, the first one is
 * the status of the account, the second is the userid corresponding to
 * the account
 */
const int VR_UPDATE_ACCOUNT_WITH_USERS = 1;
/**
 * \brief Request for updating account according to machine key
 * This request needs 2 arguments to be replaced, the first one is
 * the status of the account, the second is the machineid corresponding to
 * the account
 */
const int VR_UPDATE_ACCOUNT_WITH_MACHINE = 2;
/**
 * \brief Request for updating authaccount according to authsystem key
 * This request needs 2 arguments to be replaced, the first one is
 * the status of the authaccount, the second is the authsystemid corresponding to
 * the account
 */
const int VR_UPDATE_AUTHACCOUNT_WITH_AUTHSYSTEM = 3;
/**
 * \brief Condition for a request based on authsystem and status
 * This request needs 2 arguments to be replaced, the first one is
 * the id of the authsystem, the second is the status of the authsystem
 */
const int VR_COND_AUTHSYS_AND_STATUS = 4;
/**
 * \brief Condition for a request based on authsystem and status
 * This request needs 1 argument to be replaced, the
 * id of the authsystem
 */
const int VR_COND_AUTHSYSID = 5;
/**
 * \brief Condition for a request based on authsystem and status
 * This request needs 1 argument to be replaced, the
 * id of the authsystem
 */
const int VR_COND_USERID = 6;
/**
 * \brief Request for updating the login in authaccount according to
 * the authsystem and user keys
 * This request needs 3 arguments to be replaced, the first one is
 * the login for the authaccount, the second is the authsystem key corresponding to the account and the last the user key
 */
const int VR_UPDATE_AUTHACCOUNT_LOGIN_WITH_AUTHSYSTEM_AND_USER = 7;
/**
 * \brief Request for inserting into authaccount
 * This request needs 4 arguments to be replaced, the first one is
 * the authsystem key, the second is the user key, the third is the login and the last the status
 */
const int VR_INSERT_AUTHACCOUNT = 8;
/**
 * \brief Request for updating the status in authaccount according to
 * the authsystem and user keys
 * This request needs 3 arguments to be replaced, the first one is
 * the login for the authaccount, the second is the authsystem key corresponding to the account and the last the user key
 */
const int VR_UPDATE_AUTHACCOUNT_STATUS_WITH_AUTHSYSTEM_AND_USER = 9;
/**
 * \brief Request for selecting from authaccount
 * This request needs 2 arguments to be replaced, the first one is
 * the column to select, the second is the condition
 */
const int VR_SELECT_AUTHACCOUNT = 10;
/**
 * \brief Condition for a request based on authsystem, user and status
 * This request needs 3 arguments to be replaced, the first one is
 * the key of the authsystem, the second is the key of the user,
 * the last is the status of the authaccount
 */
const int VR_COND_AUTHSYS_USER_STATUS = 11;
/**
 * \brief Condition for a request based on authsystem, user and status
 * This request needs 3 arguments to be replaced, the first one is
 * the key of the authsystem, the second is the login of the user,
 * the last is the status of the authaccount
 */
const int VR_COND_AUTHSYS_LOGIN_STATUS = 12;


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
