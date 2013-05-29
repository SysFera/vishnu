#ifndef SSLHELPERS_HPP
#define SSLHELPERS_HPP

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <string>




class SslCryptoClient {
public:
    SslCryptoClient();

    SslCryptoClient(std::string& pubKey);

    ~SslCryptoClient();


    int encryptPub(std::string msg, unsigned char **encMsg);

    int encryptPub(const char *msg, size_t msgLen, unsigned char **encMsg);



    std::string decryptPub(unsigned char *encMsg, size_t encMsgLen);

    int decryptPub(unsigned char *encMsg, size_t encMsgLen, char **decMsg);


    unsigned char* getPubKey();


private:

    int setPubKey(std::string& pubKey);

    RSA *publicKey;



};

class SslCryptoServer {
public:
    SslCryptoServer();

    SslCryptoServer(std::string& privateKey);

    ~SslCryptoServer();

    int encryptPriv(std::string msg, unsigned char **encMsg);

    int encryptPriv(const char *msg, size_t msgLen, unsigned char **encMsg);


    std::string decryptPriv(unsigned char *encMsg, size_t encMsgLen);

    int decryptPriv(unsigned char *encMsg, size_t encMsgLen, char **decMsg);


    unsigned char* getPriKey();

private:

    int setPrivKey(std::string& privKey);
    RSA *privateKey;


};




#endif // SSLHELPERS_HPP
