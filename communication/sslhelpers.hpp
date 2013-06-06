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

#define DEBUG

#ifdef DEBUG
#define DEFAULT_RSA_KEYLEN 1024
#define DEFAULT_AES_KEYLEN 128
#define AES_ROUNDS 3
#else
#define DEFAULT_RSA_KEYLEN 2048
#define DEFAULT_AES_KEYLEN 256
#define AES_ROUNDS 6
#endif

#define PSUEDO_CLIENT

#define SALT         "alsa_channel_control"
#define AES_KEY_PASS "alsa_channel_control"

#define SUCCESSFUL 0
#define FAILURE -1

#define KEY_SERVER_PRI 0
#define KEY_SERVER_PUB 1
#define KEY_CLIENT_PUB 2

const int SIDE_CLIENT = 0;
const int SIDE_SERVER = 1;

const int MSG_CHUNK_SIZE = 2048;
const int DEFAULT_SSL_PORT = 5580;

class SslCrypto {
public:
  SslCrypto(std::string& key, int side);

  ~SslCrypto();

  int rsaEncrypt(std::string msg, unsigned char **encMsg);

  int rsaEncrypt(const char *msg, size_t msgLen, unsigned char **encMsg);

  int aesEncrypt(std::string msg, unsigned char **encMsg);

  int aesEncrypt(const char *msg, size_t msgLen, unsigned char **encMsg);

  std::string rsaDecrypt(unsigned char *encMsg, size_t encMsgLen);

  int rsaDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

  std::string aesDecrypt(unsigned char *encMsg, size_t encMsgLen);

  int aesDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

private:
  int encSide; /* server vs client */
  EVP_PKEY *keypair;
  EVP_PKEY *publicKey;

  EVP_CIPHER_CTX* rsaEncryptCtx;
  EVP_CIPHER_CTX* aesEncryptCtx;

  EVP_CIPHER_CTX* rsaDecryptCtx;
  EVP_CIPHER_CTX* aesDecryptCtx;

  unsigned char* rsaSymKey;
  int rsaSymKeyLen;
  unsigned char* rsaIV;

  unsigned char* aesKey;
  unsigned char* aesIV;

  size_t encryptLen;

  int init(size_t rsaKeyLen, size_t aesKeyLen);
  int setKey(std::string& key, int side);
};


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

  ~TlsServer() {
    BIO_flush(sslBio);
    BIO_free_all(sslBio);
  }

  void run(void);

  void run2(void);

  std::string getErrorMsg(void) const { return errorMsg; }

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
  BIO* sslBio;

  /**
   * @brief recvMsg
   */
  void recvMsg(void);
};

class TlsClient {

public:

  TlsClient(const std::string& host,
            int port = DEFAULT_SSL_PORT,
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
  std::string getData(void) const { return data; }

  std::string getErrorMsg(void) const { return errorMsg; }

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
  void recvMsg(void);
};

#endif
