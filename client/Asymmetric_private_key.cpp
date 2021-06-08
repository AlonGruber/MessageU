#include "Asymmetric_private_key.h"

std::string Asymmetric_private_key::get_private_key_in_Base64()
{
	std::string encoded_key;
	CryptoPP::Base64Encoder enc(new CryptoPP::StringSink(encoded_key));
	rsa_private_key.DEREncode(enc);
	enc.MessageEnd();
	return encoded_key;
}

void Asymmetric_private_key::load_private_key_from_Base64(std::string base64_key)
{

	CryptoPP::StringSource ss(base64_key, true, new CryptoPP::Base64Decoder);
	rsa_private_key.BERDecode(ss);
}

Asymmetric_private_key::Asymmetric_private_key()
{
	CryptoPP::AutoSeededRandomPool rnd;
	rsa_private_key.GenerateRandomWithKeySize(rnd, RSA_KEY_BIT_SIZE);
}

std::string Asymmetric_private_key::decrypt_using_private_key(std::string to_decrypt)
{
	CryptoPP::AutoSeededRandomPool rng;
	std::string decrypted;
	CryptoPP::RSAES_OAEP_SHA_Decryptor d(rsa_private_key);
	CryptoPP::StringSource ss2(to_decrypt, true, new CryptoPP::PK_DecryptorFilter(rng, d, new CryptoPP::StringSink(decrypted)));
	return decrypted;
}

std::string Asymmetric_private_key::get_raw_key()
{
	std::string str;
	CryptoPP::StringSink ss(str);
	rsa_private_key.DEREncode(ss);
	return str;
}

