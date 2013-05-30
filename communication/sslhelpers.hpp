#ifndef SSLHELPERS_HPP
#define SSLHELPERS_HPP

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <string>


class SslCrypto {
public:
  SslCrypto() {}

  virtual int encrypt(std::string msg, unsigned char **encMsg) = 0;

  virtual int encrypt(const char *msg, size_t msgLen, unsigned char **encMsg) = 0;

  virtual std::string decrypt(unsigned char *encMsg, size_t encMsgLen) = 0;

  virtual int decrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) = 0;

protected:
  virtual int setKey(std::string& key) = 0;
  EVP_PKEY *key;
};

class SslCryptoClient : public SslCrypto {
public:
  SslCryptoClient();

  SslCryptoClient(std::string& pubKey);

  ~SslCryptoClient();


  int encrypt(std::string msg, unsigned char **encMsg);

  int encrypt(const char *msg, size_t msgLen, unsigned char **encMsg);

  std::string decrypt(unsigned char *encMsg, size_t encMsgLen);

  int decrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

protected:
  int setKey(std::string& pubKey);
};

class SslCryptoServer : public SslCrypto {
public:
  SslCryptoServer();

  SslCryptoServer(std::string& privateKey);

  ~SslCryptoServer();

  int encrypt(std::string msg, unsigned char **encMsg);

  int encrypt(const char *msg, size_t msgLen, unsigned char **encMsg);

  std::string decrypt(unsigned char *encMsg, size_t encMsgLen);

  int decrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

protected:
  int setKey(std::string& privKey);
};




#endif // SSLHELPERS_HPP
