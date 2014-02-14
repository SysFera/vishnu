/**
 * \file core/src/mungeUtils.hpp
 * \brief Utility functions for using munge
 * \author Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 * \date Feb. 2014
 */


#ifndef MUNGEUTILS_HPP
#define MUNGEUTILS_HPP

#include <string>
#include "munge.h"
#include <pwd.h>

class MungeCredential {

public:
  /**
   * @brief Create a class with the given munge credential chain
   * @param chain The credential chain
   */
  MungeCredential(const std::string& chain) : mchain(chain){}

  /**
    * @brief MungeCredential
    */
  MungeCredential(void) {}

  /**
   * @brief Create a munge credential and return it as a string
   * @param payload The payload
   * @return The credential string
   */
  static std::string
  createCredentials(const std::string& payload);

  /**
   * @brief Decode a the credential chain
   * @return 0 or error code
   */
  int
  decode(void);


  /**
   * @brief Decode a given credential chain
   * @param chain The credential chain
   * @return
   */
  int
  decode(const std::string& chain);

  /**
   * @brief Get the user uid
   * @return The uid
   */
  uid_t
  getUid(void);

  /**
   * @brief Get the system username associated to a given uid
   * @param uid The UID
   * @return Empty or the associated username
   */
  std::string
  getSystemUserName(void);

  /**
   * @brief getHome
   * @return
   */
  std::string
  getHome(void);

  /**
   * @brief Return the last error msg
   * @return
   */
  std::string
  lastError(void) {return mlastError;}

private:
  std::string mchain;
  uid_t muid;
  gid_t mgid;
  int64_t mpayloadUid;
  std::string mlastError;
  passwd* msystemInfo;
};

namespace vishnu {
  /**
   * @brief Create a munge credential and return it as a string
   * @param payload The payload
   * @return The credential string
   */
  std::string
  createMungeCredentials(const std::string& payload);

  /**
   * @brief Take a munge credential string and return the uid of the orginating user
   * @param credentialChain The credential string
   * @param credential a structure containing the credential info
   * @return 0 or error code
   */
  int
  getMungeCredential(const std::string& credentialChain, MungeCredential& credential);

  /**
   * @brief Get the user uid
   * @param credential Munge-decoded credential
   * @return The uid
   */
  uid_t
  getUid(const MungeCredential& credential);

  /**
   * @brief Get the system username associated to a given uid
   * @param uid The UID
   * @return Empty or the associated username
   */
  std::string
  getSystemUserName(uid_t uid);

  /**
   * @brief Get the system username associated to a given uid
   * @param uid The UID
   * @return Empty or the associated username
   */
  std::string
  getSystemUserName(const MungeCredential& credential);
}
#endif // MUNGEUTILS_HPP
