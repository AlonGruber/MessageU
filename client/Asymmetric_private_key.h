#pragma once
#include <cryptlib.h>
#include "rsa.h"
#include "osrng.h"
#include "base64.h"
#include "utils.h"


//class for a dealing with the RSA private key

const int RSA_KEY_BIT_SIZE = 1024;

class Asymmetric_private_key
{
private:
	CryptoPP::RSA::PrivateKey rsa_private_key;
public:
	Asymmetric_private_key();

	std::string decrypt_using_private_key(std::string to_decrypt);
	std::string get_raw_key();

	void load_private_key_from_Base64(std::string key);
	std::string get_private_key_in_Base64();
};

