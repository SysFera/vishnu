/**
 * \file core/src/mungeUtils.cpp
 * \brief Utility functions for using munge
 * \author Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
 * \date Feb. 2014
 */

#include "utilVishnu.hpp"
#include "SystemException.hpp"
#include "mungeUtils.hpp"
#include <cstdlib>
#include <boost/format.hpp>

/**
 * @brief Create a munge credential and return it as a string
 * @param payload The payload
 * @return The credential string
 */
std::string
MungeCredential::createCredentials(const std::string& payload) {

  char* mungeResult;
  munge_err_t  err = munge_encode(&mungeResult,
                                  NULL,
                                  payload.c_str(),
                                  payload.size());
  if (err != EMUNGE_SUCCESS) {
    throw SystemException(ERRCODE_SYSTEM, munge_strerror(err));
  }

  std::string credential = std::string(mungeResult);
  free(mungeResult);
  return credential;
}

/**
 * @brief Decode a the credential chain
 * @return 0 or error code
 */
int
MungeCredential::decode(void) {

  char* payload;
  int payloadLen;

  munge_err_t err = munge_decode(mchain.c_str(),
                                 NULL,
                                 (void**)(&payload),
                                 &payloadLen,
                                 &muid,
                                 &mgid);
  if (err == EMUNGE_SUCCESS) {
    mpayloadUid = -1;
    if (payloadLen > 0 && muid == 0) {
      mpayloadUid = vishnu::convertToInt(payload);
    }
    free(payload);
    msystemInfo = getpwuid(muid);
  } else {
    mlastError = std::string(munge_strerror(err));
  }
  return err;
}


/**
 * @brief Decode a given credential chain
 * @param chain The credential chain
 * @return
 */
int
MungeCredential::decode(const std::string& chain) {
  mchain = chain;
  return decode();
}


/**
 * @brief Get the user uid
 * @return The uid
 */
uid_t
MungeCredential::getUid(void) {
  if (mpayloadUid > 0) {
    return mpayloadUid;
  }
  return muid;
}

/**
 * @brief Get the system username associated to a given uid
 * @return Empty or the associated username
 */
std::string
MungeCredential::getSystemUserName(void) {
  if (msystemInfo) {
    return std::string(msystemInfo->pw_name);
  }
  return std::string("");
}

/**
 * @brief getHome
 * @return
 */
std::string
MungeCredential::getHome(void) {
  if (msystemInfo) {
    return std::string(msystemInfo->pw_dir);
  }
  return std::string("");
}


