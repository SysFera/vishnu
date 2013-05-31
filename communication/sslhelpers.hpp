#ifndef _SSLHELPER_HPP
#define _SSLHELPER_HPP

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string>
#include <string.h>

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

#define SIDE_CLIENT 0
#define SIDE_SERVER 1

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
  EVP_PKEY *serverKeypair;
  EVP_PKEY *clientPubKey;

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
  int genTestClientKey(int keyLen);
  int setKey(std::string& key, int side);
};

#endif
