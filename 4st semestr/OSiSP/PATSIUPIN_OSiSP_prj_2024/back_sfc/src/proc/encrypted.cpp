#include "main_proc.h"

#include <openssl/evp.h>
#include <boost/filesystem.hpp>

void to_encrypt_file(const std::string& filename, const std::string& password) {

    std::string encrypted_file = filename + ".enc";
    if (boost::filesystem::exists(encrypted_file)) {
        boost::filesystem::remove(encrypted_file);
    }

    std::ifstream inFile(filename, std::ios::binary);
    std::ofstream outFile(filename + ".enc", std::ios::binary);

    const EVP_CIPHER* cipherType = EVP_aes_256_cbc();
    const EVP_MD* digestType = EVP_sha256();
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];

    EVP_BytesToKey(cipherType, digestType, nullptr,
                   reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), 1, key, iv);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, cipherType, nullptr, key, iv);

    char buffer[4096];
    unsigned char bufferOut[4096 + EVP_MAX_BLOCK_LENGTH];
    int numRead, numCrypted;

    while (inFile.read(buffer, sizeof(buffer)), numRead = inFile.gcount()) {
        EVP_EncryptUpdate(ctx, bufferOut, &numCrypted, reinterpret_cast<unsigned char*>(buffer), numRead);
        outFile.write(reinterpret_cast<char*>(bufferOut), numCrypted);
    }

    EVP_EncryptFinal_ex(ctx, bufferOut, &numCrypted);
    outFile.write(reinterpret_cast<char*>(bufferOut), numCrypted);

    EVP_CIPHER_CTX_free(ctx);
}

void to_decrypt_file(const std::string& filename, const std::string& password) {
    std::ifstream inFile(filename, std::ios::binary);
    std::ofstream outFile(filename.substr(0, filename.size() - 4), std::ios::binary);

    const EVP_CIPHER* cipherType = EVP_aes_256_cbc();
    const EVP_MD* digestType = EVP_sha256();
    unsigned char key[EVP_MAX_KEY_LENGTH];
    unsigned char iv[EVP_MAX_IV_LENGTH];

    EVP_BytesToKey(cipherType, digestType, nullptr,
                   reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), 1, key, iv);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, cipherType, nullptr, key, iv);

    char buffer[4096];
    unsigned char bufferOut[4096 + EVP_MAX_BLOCK_LENGTH];
    int numRead, numDecrypted;

    while (inFile.read(buffer, sizeof(buffer)), numRead = inFile.gcount()) {
        EVP_DecryptUpdate(ctx, bufferOut, &numDecrypted, reinterpret_cast<unsigned char*>(buffer), numRead);
        outFile.write(reinterpret_cast<char*>(bufferOut), numDecrypted);
    }

    EVP_DecryptFinal_ex(ctx, bufferOut, &numDecrypted);
    outFile.write(reinterpret_cast<char*>(bufferOut), numDecrypted);

    EVP_CIPHER_CTX_free(ctx);
}