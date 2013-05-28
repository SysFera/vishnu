#include "sslhelpers.hpp"

using namespace std;

SslCrypto::SslCrypto() {

    publicKey = NULL;
    privateKey = NULL;

    encryptCtx = NULL;
    decryptCtx = NULL;
}

SslCrypto::SslCrypto(string& pubKey) {
    publicKey = NULL;
    privateKey = NULL;

    encryptCtx = NULL;
    decryptCtx = NULL;
    setPubKey(pubKey);
}

SslCrypto::SslCrypto(string& privkey) {
    publicKey = NULL;
    privateKey = NULL;

    encryptCtx = NULL;
    decryptCtx = NULL;
    setPrivKey(privkey);
}

SslCrypto::~SslCrypto() {
    EVP_PKEY_free(privateKey);
    EVP_PKEY_free(publicKey);

    EVP_CIPHER_CTX_cleanup(encryptCtx);


    EVP_CIPHER_CTX_cleanup(decryptCtx);

    free(encryptCtx);
    free(decryptCtx);

}

int SslCrypto::encryptPriv(std::string msg, unsigned char **encMsg) {
    return encryptPriv(msg.c_str(), msg.size(), encMsg);
}

int SslCrypto::encryptPriv(const char *msg, size_t msgLen, unsigned char **encMsg) {
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

int SslCrypto::encryptPub(std::string msg, unsigned char **encMsg)
{
    return encryptPub(msg.c_str(), msg.size(), encMsg);
}

int SslCrypto::encryptPub(const char *msg, size_t msgLen, unsigned char **encMsg)
{
    size_t encMsgLen = 0;
    size_t blockLen  = 0;
    *encMsg = (unsigned char*)malloc(EVP_PKEY_size(publicKey));
    if(encMsg == NULL) return FAILURE;


    rsaSymKey = (unsigned char*)malloc(rsaKeyLen/8);
    rsaIV = (unsigned char*)malloc(EVP_MAX_IV_LENGTH);

    if(!EVP_SealInit(encryptCtx, EVP_aes_128_cbc(), &rsaSymKey, &rsaSymKeyLen, rsaIV, &publicKey, 1)) {
        return FAILURE;
    }

    if(!EVP_SealUpdate(rsaEncryptCtx, *encMsg + encMsgLen, (int*)&blockLen, (const unsigned char*)msg, (int)msgLen)) {
        return FAILURE;
    }
    encMsgLen += blockLen;

    if(!EVP_SealFinal(encryptCtx, *encMsg + encMsgLen, (int*)&blockLen)) {
        return FAILURE;
    }
    encMsgLen += blockLen;

    EVP_CIPHER_CTX_cleanup(rsaEncryptCtx);

    /*int RSA_public_encrypt(int flen, unsigned char *from,
       unsigned char *to, RSA *rsa, int padding);

    int RSA_private_decrypt(int flen, unsigned char *from,
        unsigned char *to, RSA *rsa, int padding);*/
    return (int)encMsgLen;
}

std::string SslCrypto::decryptPub(unsigned char *encMsg, size_t encMsgLen)
{
}

int SslCrypto::decryptPub(unsigned char *encMsg, size_t encMsgLen, char **decMsg)
{
}

std::string SslCrypto::decryptPriv(unsigned char *encMsg, size_t encMsgLen) {
    char **decMsg = NULL;
    rsaDecrypt(encMsg, encMsgLen, decMsg);
    return string(*decMsg);
}

int SslCrypto::decryptPriv(unsigned char *encMsg, size_t encMsgLen, char **decMsg) {
    size_t decLen   = 0;
    size_t blockLen = 0;
    EVP_PKEY *key;
    *decMsg = (char*)malloc(encMsgLen + EVP_MAX_IV_LENGTH);
    if(decMsg == NULL) return FAILURE;

    #ifdef PSUEDO_CLIENT
        key = clientPubKey;
    #else
        key = serverKeyPair;
    #endif

    if(!EVP_OpenInit(rsaDecryptCtx, EVP_aes_128_cbc(), rsaSymKey, rsaSymKeyLen, rsaIV, key)) {
        return FAILURE;
    }

    if(!EVP_OpenUpdate(rsaDecryptCtx, (unsigned char*)*decMsg + decLen, (int*)&blockLen, encMsg, (int)encMsgLen)) {
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



int SslCrypto::setClientPubKey(unsigned char* pubKey, size_t pubKeyLen) {
    BIO *bio = BIO_new(BIO_f_base64());
    if(!BIO_write(bio, pubKey, pubKeyLen)) {
        return FAILURE;
    }

    RSA *_pubKey = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);
    EVP_PKEY_assign_RSA(clientPubKey, _pubKey);

    BIO_free_all(bio);

    return SUCCESS;
}

unsigned char* SslCrypto::getServerPubKey() {
    BIO *bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bio, serverKeypair);

    int pubKeyLen = BIO_pending(bio);
    unsigned char *pubKey = (unsigned char*)malloc(pubKeyLen);

    BIO_read(bio, pubKey, pubKeyLen);

    BIO_free_all(bio);

    return pubKey;
}

unsigned char* SslCrypto::getServerPriKey() {
    BIO *bio = BIO_new(BIO_s_mem());
    // TODO fix this. it doesn't work.
    PEM_write_bio_PrivateKey(bio, serverKeypair, EVP_aes_128_cbc(), rsaSymKey, rsaSymKeyLen, 0, NULL);

    int priKeyLen = BIO_pending(bio);
    unsigned char *priKey = (unsigned char*)malloc(priKeyLen + 1);

    BIO_read(bio, priKey, priKeyLen);

    BIO_free_all(bio);

    return priKey;
}

unsigned char* SslCrypto::getServerAESKey() {
    return aesKey;
}

/*int SslCrypto::init(size_t rsaKeyLen, size_t aesKeyLen) {
    // Initalize contexts
    rsaEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
    aesEncryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));

    rsaDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));
    aesDecryptCtx = (EVP_CIPHER_CTX*)malloc(sizeof(EVP_CIPHER_CTX));

    if(rsaEncryptCtx == NULL || aesEncryptCtx == NULL || rsaDecryptCtx == NULL || rsaDecryptCtx == NULL) {
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

    if(EVP_PKEY_keygen(ctx, &serverKeypair) <= 0) {
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

    if(!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), (const unsigned char*)SALT, (const unsigned char*)AES_KEY_PASS, strlen(AES_KEY_PASS), AES_ROUNDS, aesKey, aesIV)) {
        return FAILURE;
    }

    return SUCCESS;
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

    return SUCCESS;
}*/


int SslCrypto::setPrivKey(string &privKey)
{

    privKey = PEM_read_PrivateKey(rsa_pkey_file, NULL, NULL, NULL);
}
