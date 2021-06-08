#pragma once
#include <cryptlib.h>
#include "aes.h"
#include "osrng.h"
#include "ccm.h"
#include "hex.h"

//class for the symmetric key 

const int SYMMETRIC_KEY_BYTE_SIZE = 16;

class Symmetric_key_AES_CBC
{
private:
	CryptoPP::byte Symmetric_key[CryptoPP::AES::DEFAULT_KEYLENGTH];
	CryptoPP::byte IV[CryptoPP::AES::BLOCKSIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
public:
	Symmetric_key_AES_CBC();
	Symmetric_key_AES_CBC(std::string key);

	std::string encrypt_using_key(std::string to_encrypt);
	std::string decrypt_using_key(std::string to_decrypt);

	std::string get_symmetric_key_string();
	
	~Symmetric_key_AES_CBC() = default;
	Symmetric_key_AES_CBC(const Symmetric_key_AES_CBC& R);
	Symmetric_key_AES_CBC& operator=(const Symmetric_key_AES_CBC& other);
};

