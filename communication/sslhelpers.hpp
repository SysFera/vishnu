#ifndef SSLHELPERS_HPP
#define SSLHELPERS_HPP

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>




class SslCrypto {
public:
    SslCrypto();

    SslCrypto(string& pubKey);

    SslCrypto(string& privateKey);

    ~SslCrypto();

    int encryptPriv(std::string msg, unsigned char **encMsg);

    int encryptPriv(const char *msg, size_t msgLen, unsigned char **encMsg);

    int encryptPub(std::string msg, unsigned char **encMsg);

    int encryptPub(const char *msg, size_t msgLen, unsigned char **encMsg);



    std::string decryptPub(unsigned char *encMsg, size_t encMsgLen);

    int decryptPub(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

    std::string decryptPriv(unsigned char *encMsg, size_t encMsgLen);

    int decryptPriv(unsigned char *encMsg, size_t encMsgLen, char **decMsg);

    unsigned char* getPubKey();

    unsigned char* getPriKey();

private:

    int setPubKey(string& pubKey);
    int setPrivKey(string& privKey);
    EVP_PKEY *publicKey;
    EVP_PKEY *privateKey;

    EVP_CIPHER_CTX *encryptCtx;
    EVP_CIPHER_CTX *decryptCtx;

};



#endif // SSLHELPERS_HPP
