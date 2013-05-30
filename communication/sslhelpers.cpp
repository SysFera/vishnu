#include <iostream>
#include "sslhelpers.hpp"

using namespace std;

SslCryptoClient::SslCryptoClient() : SslCrypto() {
  key = NULL;
}

SslCryptoServer::SslCryptoServer() : SslCrypto() {
  key = NULL;
}

SslCryptoClient::SslCryptoClient(string& pubKey) : SslCrypto() {
  key = NULL;
  setKey(pubKey);
}

SslCryptoServer::SslCryptoServer(string& privkey)  : SslCrypto() {
  key = NULL;
  setKey(privkey);
}

SslCryptoClient::~SslCryptoClient() {
  RSA_free(key);
}

SslCryptoServer::~SslCryptoServer() {
  RSA_free(key);

}

int SslCryptoServer::encrypt(std::string msg, unsigned char **encMsg) {
  return encrypt(msg.c_str(), msg.size(), encMsg);
}

int SslCryptoServer::encrypt(const char *msg, size_t msgLen, unsigned char **encMsg) {

  if(key !=NULL)
  {
    *encMsg = (unsigned char*)malloc(RSA_size(key));
    if(encMsg == NULL) {
      return -1;
    }
  } else {
    std::cout << "private key is NULL \n";
  }

  return  RSA_private_encrypt(msgLen, reinterpret_cast<const unsigned char *>(msg), *encMsg, key, RSA_PKCS1_PADDING);
}

int SslCryptoClient::encrypt(std::string msg, unsigned char **encMsg)
{
  return encrypt(msg.c_str(), msg.size(), encMsg);
}

int SslCryptoClient::encrypt(const char *msg, size_t msgLen, unsigned char **encMsg)
{
  if(key != NULL){
    *encMsg = (unsigned char*)malloc(RSA_size(key));
    if(encMsg == NULL) {
      return -1;
    }
  } else {
    std::cout << "public key is NULL \n";
  }


  return  RSA_public_encrypt(msgLen, reinterpret_cast<const unsigned char *>(msg), *encMsg, key, RSA_PKCS1_PADDING);
}

std::string SslCryptoClient::decrypt(unsigned char *encMsg, size_t encMsgLen)
{
  char **decMsg = NULL;
  decrypt(encMsg, encMsgLen, decMsg);
  return string(*decMsg);
}

int SslCryptoClient::decrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg)
{
  *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
  if(decMsg == NULL) {
    return -1;
  }

  return RSA_public_decrypt(encMsgLen, encMsg, reinterpret_cast<unsigned char *>(*decMsg), key, RSA_PKCS1_PADDING);

}

std::string SslCryptoServer::decrypt(unsigned char *encMsg, size_t encMsgLen) {
  char **decMsg = NULL;
  decrypt(encMsg, encMsgLen, decMsg);
  return string(*decMsg);
}

int SslCryptoServer::decrypt(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {

  *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
  if(decMsg == NULL) {
    return -1;
  }

  return RSA_private_decrypt(encMsgLen, encMsg, reinterpret_cast<unsigned char *>(*decMsg), key, RSA_PKCS1_PADDING);


}


int SslCryptoServer::setKey(std::string &privKey)
{
  FILE *file;
  file = fopen(privKey.c_str(),"r");

  if(file){
    key = PEM_read_RSAPrivateKey(file, &key,NULL,NULL);
    std::cout << "setting private key, File is " << privKey << " and the key is " << key << "\n";
    return 0;
  } else{
    return -1;
  }

}


int SslCryptoClient::setKey(std::string &pubKey)
{
  FILE *file;
  file = fopen(pubKey.c_str(),"r");

  if(file){
    key = (RSA *)PEM_read_RSAPublicKey(file, &key,NULL,NULL);
    std::cout << "setting public key, File is " << pubKey << " and the key is " << key << "\n";
    return 0;
  } else{
    return -1;
  }

}
