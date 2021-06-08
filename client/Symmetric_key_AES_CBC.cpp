#include "Symmetric_key_AES_CBC.h"
#include "utils.h"



Symmetric_key_AES_CBC::Symmetric_key_AES_CBC()
{
	CryptoPP::AutoSeededRandomPool random;
	random.GenerateBlock(Symmetric_key, sizeof(Symmetric_key));
}

Symmetric_key_AES_CBC::Symmetric_key_AES_CBC(std::string key)
{
	CryptoPP::ArraySource ss(key, true, new CryptoPP::ArraySink(Symmetric_key, sizeof(Symmetric_key)));
}

std::string Symmetric_key_AES_CBC::encrypt_using_key(std::string to_encrypt)
{
	std::string encoded;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryptor;
	encryptor.SetKeyWithIV(Symmetric_key, sizeof(Symmetric_key), IV);
	CryptoPP::StringSource ss(to_encrypt, true,new CryptoPP::StreamTransformationFilter(encryptor,new CryptoPP::StringSink(encoded)));
	return encoded;
}

std::string Symmetric_key_AES_CBC::decrypt_using_key(std::string to_decrypt)
{
	std::string decrypted;
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryptor;
	decryptor.SetKeyWithIV(Symmetric_key, sizeof(Symmetric_key), IV);
	CryptoPP::StringSource ss(to_decrypt, true, new CryptoPP::StreamTransformationFilter(decryptor,new CryptoPP::StringSink(decrypted)));
	return decrypted;
}

std::string Symmetric_key_AES_CBC::get_symmetric_key_string()
{
	std::string key_in_string;
	CryptoPP::ArraySource as(Symmetric_key, sizeof(Symmetric_key), true, new CryptoPP::StringSink(key_in_string));
	return key_in_string;
}


Symmetric_key_AES_CBC::Symmetric_key_AES_CBC(const Symmetric_key_AES_CBC& R)
{
	memcpy(Symmetric_key, R.Symmetric_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	memcpy(IV, R.IV, CryptoPP::AES::BLOCKSIZE);
}

Symmetric_key_AES_CBC& Symmetric_key_AES_CBC::operator=(const Symmetric_key_AES_CBC& other)
{
	memcpy(Symmetric_key, other.Symmetric_key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	memcpy(IV, other.IV, CryptoPP::AES::BLOCKSIZE);
	return *this;
}
