#pragma once
#include <cryptlib.h>
#include "Asymmetric_private_key.h"
#include "rsa.h"
#include "osrng.h"

//class for a dealing with the RSA public key

class Asymmetric_public_key
{
private:
	CryptoPP::RSA::PublicKey  rsa_public_key;
public:
	Asymmetric_public_key(std::string key);
	Asymmetric_public_key(Asymmetric_private_key * rsa_private_key);
	std::string encrypt_using_public_key(std::string to_encrypt);
	std::string get_raw_key_in_string();

};

