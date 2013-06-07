/**
 * \file sslhelpers.hpp
 * \brief This file contains the definition of the base classes (client/server) for the TLS
 * \author Rodrigue Chakode (rodrigue.chakode@sysfera.com)
 * \date May 2013
 */

#ifndef _SSLHELPER_HPP
#define _SSLHELPER_HPP

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <boost/shared_ptr.hpp>

#include "zhelpers.hpp"
#include "DIET_client.h"

const int MSG_CHUNK_SIZE = 2048;

/**
 * @brief The TlsServer class
 */
class TlsServer {

public:
  TlsServer(const std::string& privKey,
            const std::string cert,
            int port,
            const std::string& internalSrvUri)
    : listeningPort(port),
      privateKey(privKey),
      certificate(cert),
      internalServiceUri(internalSrvUri)
  { }

  ~TlsServer() {}

  /**
   * @brief run
   */
  void
  run(void);

  /**
   * @brief getErrorMsg
   * @return
   */
  std::string
  getErrorMsg(void) const { return errorMsg; }

private:
  /**
   * \brief listening port
  */
  int listeningPort;

  /**
   * \brief  path to server private key
  */
  std::string privateKey;

  /**
   * \brief  path to the certificate
  */
  std::string certificate;

  /**
   * \brief Secret key used for communication
  */
  std::string secretKey;

  /**
   * @brief The internal uri to connect to the service
   */
  std::string internalServiceUri;

  /**
   * @brief error
   */
  std::string errorMsg;

  /**
   * @brief data
   */
  std::string data;

  /**
   * @brief sslBio
   */
  BIO* clientBioHandler;

  /**
   * @brief recvMsg
   */
  void recvMsg(void);
};

class TlsClient {

public:

  TlsClient(const std::string& host,
            int port,
            const std::string& ca = "")
    : serverAddr(host),
      serverPort(port),
      cafile(ca)
  {
  }

  ~TlsClient() {
    BIO_free_all(sslBio);
  }

  /**
   * @brief send
   */
  int
  send(const std::string& data);

  /**
   * @brief recv
   * @return
   */
  std::string
  recv(void);

  std::string
  getErrorMsg(void) const { return errorMsg; }

private:
  /**
   * \brief Server address
  */
  const std::string serverAddr;

  /**
   * \brief The listening port on the server
  */
  int serverPort;

  /**
   * \brief  path to the CA file
  */
  std::string cafile;

  /**
   * @brief sslBio
   */
  BIO* sslBio;

  /**
   * \brief Message received from server
  */
  std::string data;

  /**
   * @brief error
   */
  std::string errorMsg;

  /**
   * @brief recvMsg
   */
  void
  recvMsg(void);
};

#endif
