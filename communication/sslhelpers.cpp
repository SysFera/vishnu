#include "sslhelpers.hpp"
#include <iostream>

using namespace std;

SslCrypto::SslCrypto(std::string& key, int side) : encSide(side) {

  serverKeypair = NULL;
  clientPubKey  = NULL;
  rsaSymKey     = NULL;
  rsaSymKeyLen  = 0;
  rsaIV         = NULL;
  encryptLen    = 0;

  if (encSide == SIDE_CLIENT) {
    genTestClientKey(DEFAULT_RSA_KEYLEN);
  }
  init(DEFAULT_RSA_KEYLEN, DEFAULT_AES_KEYLEN);
  setKey(key, side);
}

SslCrypto::~SslCrypto() {
  EVP_PKEY_free(serverKeypair);
  EVP_PKEY_free(clientPubKey);

  EVP_CIPHER_CTX_cleanup(rsaEncryptCtx);
  EVP_CIPHER_CTX_cleanup(aesEncryptCtx);

  EVP_CIPHER_CTX_cleanup(rsaDecryptCtx);
  EVP_CIPHER_CTX_cleanup(aesDecryptCtx);

  free(rsaEncryptCtx);
  free(aesEncryptCtx);

  free(rsaDecryptCtx);
  free(aesDecryptCtx);

  free(rsaSymKey);
  free(rsaIV);

  free(aesKey);
  free(aesIV);
}

int SslCrypto::rsaEncrypt(std::string msg, unsigned char **encMsg) {
  return rsaEncrypt(msg.c_str(), msg.size(), encMsg);
}

int SslCrypto::rsaEncrypt(const char *msg, size_t msgLen, unsigned char **encMsg) {
  size_t encMsgLen = 0;
  size_t blockLen  = 0;
  *encMsg = (unsigned char*)malloc(EVP_PKEY_size(clientPubKey));
  if(encMsg == NULL) return FAILURE;
  if(!EVP_SealInit(rsaEncryptCtx, EVP_aes_128_cbc(), &rsaSymKey, &rsaSymKeyLen, rsaIV, &clientPubKey, 1)) {
    return FAILURE;
  }

  if(!EVP_SealUpdate(rsaEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen, (const unsigned char*)msg, (int)msgLen)) {
    return FAILURE;
  }
  encMsgLen += blockLen;

  if(!EVP_SealFinal(rsaEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen)) {
    return FAILURE;
  }
  encMsgLen += blockLen;

  EVP_CIPHER_CTX_cleanup(rsaEncryptCtx);

  return (int)encMsgLen;
}

int SslCrypto::aesEncrypt(std::string msg, unsigned char **encMsg) {
  return aesEncrypt(msg.c_str(), msg.size(), encMsg);
}

int SslCrypto::aesEncrypt(const char *msg, size_t msgLen, unsigned char **encMsg) {
  size_t blockLen  = 0;
  size_t encMsgLen = 0;
  *encMsg = (unsigned char*)malloc(msgLen + AES_BLOCK_SIZE);
  if(encMsg == NULL) return FAILURE;

  if(!EVP_EncryptInit_ex(aesEncryptCtx,
                         EVP_aes_256_cbc(),
                         NULL,
                         aesKey,
                         aesIV)) {
    return FAILURE;
  }

  if(!EVP_EncryptUpdate(aesEncryptCtx,
                        *encMsg,
                        (int*)&blockLen,
                        (unsigned char*)msg,
                        msgLen)) {
    return FAILURE;
  }
  encMsgLen += blockLen;

  if(!EVP_EncryptFinal_ex(aesEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen)) {
    return FAILURE;
  }

  EVP_CIPHER_CTX_cleanup(aesEncryptCtx);
  return encMsgLen + blockLen;
}

std::string SslCrypto::rsaDecrypt(unsigned char *encMsg, size_t encMsgLen) {
  char **decMsg = NULL;
  rsaDecrypt(encMsg, encMsgLen, decMsg);
  return string(*decMsg);
}

int SslCrypto::rsaDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {
  size_t decLen   = 0;
  size_t blockLen = 0;
  *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
  if(decMsg == NULL) return FAILURE;

  int success;
  if (encSide == SIDE_SERVER) {
    success = EVP_OpenInit(rsaDecryptCtx,
                           EVP_aes_128_cbc(),
                           rsaSymKey,
                           rsaSymKeyLen,
                           rsaIV,
                           serverKeypair);
  } else {
    success = EVP_OpenInit(rsaDecryptCtx,
                           EVP_aes_128_cbc(),
                           rsaSymKey,
                           rsaSymKeyLen,
                           rsaIV,
                           clientPubKey);
  }

  if(!success) {
    return FAILURE;
  }

  if(!EVP_OpenUpdate(rsaDecryptCtx,
                     (unsigned char*)*decMsg + decLen,
                     (int*)&blockLen, encMsg,
                     (int)encMsgLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  if(!EVP_OpenFinal(rsaDecryptCtx, (unsigned char*)*decMsg + decLen, (int*)&blockLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  (*decMsg)[decLen] = '\0';

  EVP_CIPHER_CTX_cleanup(rsaDecryptCtx);

  return (int)decLen;
}

std::string SslCrypto::aesDecrypt(unsigned char *encMsg, size_t encMsgLen) {
  char **decMsg = NULL;
  aesDecrypt(encMsg, encMsgLen, decMsg);
  return string(*decMsg);
}

int SslCrypto::aesDecrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {
  size_t decLen   = 0;
  size_t blockLen = 0;

  *decMsg = (char*)malloc(encMsgLen);
  if(*decMsg == NULL) return FAILURE;

  if(!EVP_DecryptInit_ex(aesDecryptCtx,
                         EVP_aes_256_cbc(),
                         NULL,
                         aesKey,
                         aesIV)) {
    return FAILURE;
  }

  if(!EVP_DecryptUpdate(aesDecryptCtx,
                        (unsigned char*)*decMsg,
                        (int*)&blockLen, encMsg,
                        (int)encMsgLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  if(!EVP_DecryptFinal_ex(aesDecryptCtx,
                          (unsigned char*)*decMsg + decLen,
                          (int*)&blockLen)) {
    return FAILURE;
  }
  decLen += blockLen;

  (*decMsg)[decLen] = '\0';

  EVP_CIPHER_CTX_cleanup(aesDecryptCtx);

  return encMsgLen;
}


int SslCrypto::init(size_t rsaKeyLen, size_t aesKeyLen) {
  // Initalize contexts
  rsaEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
  aesEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));

  rsaDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
  aesDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));

  if(rsaEncryptCtx == NULL || aesEncryptCtx == NULL ||
     rsaDecryptCtx == NULL || rsaDecryptCtx == NULL) {
    return FAILURE;
  }

  // Init these here to make valgrind happy
  EVP_CIPHER_CTX_init(rsaEncryptCtx);
  EVP_CIPHER_CTX_init(aesEncryptCtx);

  EVP_CIPHER_CTX_init(rsaDecryptCtx);
  EVP_CIPHER_CTX_init(aesDecryptCtx);

  // Init RSA
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if(EVP_PKEY_keygen_init(ctx) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, (int)rsaKeyLen) <= 0) {
    return FAILURE;
  }

  if(!(serverKeypair = EVP_PKEY_new())) {
    return FAILURE;
  }

  EVP_PKEY_CTX_free(ctx);

  rsaSymKey = (unsigned char*)malloc(rsaKeyLen/8);
  rsaIV = (unsigned char*)malloc(EVP_MAX_IV_LENGTH);

  // Init AES
  aesKey = (unsigned char*)malloc(aesKeyLen/8 + EVP_MAX_IV_LENGTH);
  aesIV = (unsigned char*)malloc(aesKeyLen/8);

  if(rsaSymKey == NULL || rsaIV == NULL || aesKey == NULL || aesIV == NULL) {
    return FAILURE;
  }

  if(!EVP_BytesToKey(EVP_aes_256_cbc(),
                     EVP_sha1(),
                     (const unsigned char*)SALT,
                     (const unsigned char*)AES_KEY_PASS,
                     strlen(AES_KEY_PASS),
                     AES_ROUNDS,
                     aesKey,
                     aesIV)) {
    return FAILURE;
  }

  return SUCCESSFUL;
}

int SslCrypto::genTestClientKey(int keyLen) {
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

  if(EVP_PKEY_keygen_init(ctx) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, keyLen) <= 0) {
    return FAILURE;
  }

  if(EVP_PKEY_keygen(ctx, &clientPubKey) <= 0) {
    return FAILURE;
  }

  EVP_PKEY_CTX_free(ctx);

  return SUCCESSFUL;
}

int SslCrypto::setKey(std::string& keyPath, int side)
{
  FILE *file = fopen(keyPath.c_str(),"r");
  if (file) {
    if(side == SIDE_SERVER) {
      RSA* privKey = PEM_read_RSAPrivateKey(file, &privKey, NULL, NULL);
      EVP_PKEY_assign_RSA(serverKeypair, privKey);
    } else {
      RSA* pubKey = PEM_read_RSA_PUBKEY(file, &pubKey, NULL, NULL);
      EVP_PKEY_assign_RSA(clientPubKey, pubKey);
    }
    return SUCCESSFUL;
  }
  return FAILURE;
}
