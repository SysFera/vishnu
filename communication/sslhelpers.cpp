#include "sslhelpers.hpp"

using namespace std;

SslCryptoClient::SslCryptoClient() {

    publicKey = NULL;

}

SslCryptoServer::SslCryptoServer() {


    privateKey = NULL;

}

SslCryptoClient::SslCryptoClient(string& pubKey) {
    publicKey = NULL;

    setPubKey(pubKey);
}

SslCryptoServer::SslCryptoServer(string& privkey) {

    privateKey = NULL;

    setPrivKey(privkey);
}

SslCryptoClient::~SslCryptoClient() {
    RSA_free(publicKey);
}

SslCryptoServer::~SslCryptoServer() {
    RSA_free(privateKey);

}

int SslCryptoServer::encryptPriv(std::string msg, unsigned char **encMsg) {
    return encryptPriv(msg.c_str(), msg.size(), encMsg);
}

int SslCryptoServer::encryptPriv(const char *msg, size_t msgLen, unsigned char **encMsg) {

    *encMsg = (unsigned char*)malloc(RSA_size(privateKey));
    if(encMsg == NULL) {
        return -1;
    }

    return  RSA_private_encrypt(msgLen, reinterpret_cast<const unsigned char *>(msg), *encMsg, privateKey, RSA_PKCS1_PADDING);
}

int SslCryptoClient::encryptPub(std::string msg, unsigned char **encMsg)
{
    return encryptPub(msg.c_str(), msg.size(), encMsg);
}

int SslCryptoClient::encryptPub(const char *msg, size_t msgLen, unsigned char **encMsg)
{

    *encMsg = (unsigned char*)malloc(RSA_size(publicKey));
    if(encMsg == NULL) {
        return -1;
    }

    return  RSA_public_encrypt(msgLen, reinterpret_cast<const unsigned char *>(msg), *encMsg, publicKey, RSA_PKCS1_PADDING);
}

std::string SslCryptoClient::decryptPub(unsigned char *encMsg, size_t encMsgLen)
{
    char **decMsg = NULL;
    decryptPub(encMsg, encMsgLen, decMsg);
    return string(*decMsg);
}

int SslCryptoClient::decryptPub(unsigned char *encMsg, size_t encMsgLen, char **decMsg)
{
    *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
    if(decMsg == NULL) {
        return -1;
    }

    return RSA_public_decrypt(encMsgLen, encMsg, reinterpret_cast<unsigned char *>(*decMsg), publicKey, RSA_PKCS1_PADDING);

}

std::string SslCryptoServer::decryptPriv(unsigned char *encMsg, size_t encMsgLen) {
    char **decMsg = NULL;
    decryptPriv(encMsg, encMsgLen, decMsg);
    return string(*decMsg);
}

int SslCryptoServer::decryptPriv(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {

    *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
    if(decMsg == NULL) {
        return -1;
    }

    return RSA_private_decrypt(encMsgLen, encMsg, reinterpret_cast<unsigned char *>(*decMsg), privateKey, RSA_PKCS1_PADDING);


}




int SslCryptoServer::setPrivKey(std::string &privKey)
{
    FILE *file;
    file = fopen(privKey.c_str(),"r");

    if(file){
        privateKey = PEM_read_RSAPrivateKey(file, &privateKey,NULL,NULL);
        return 0;
    } else{
        return -1;
    }

}


int SslCryptoClient::setPubKey(std::string &pubKey)
{
    FILE *file;
    file = fopen(pubKey.c_str(),"r");

    if(file){
        publicKey = PEM_read_RSA_PUBKEY(file, &publicKey,NULL,NULL);
        return 0;
    } else{
        return -1;
    }

}
