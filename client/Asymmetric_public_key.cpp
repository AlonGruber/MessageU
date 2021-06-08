#include "Asymmetric_public_key.h"

Asymmetric_public_key::Asymmetric_public_key(std::string key)
{
	CryptoPP::ByteQueue queue;
	CryptoPP::StringSource ss(key,true);
	ss.TransferTo(queue);
	rsa_public_key.Load(queue);
	queue.MessageEnd();
}



Asymmetric_public_key::Asymmetric_public_key(Asymmetric_private_key *  rsa_private_key)
{
	CryptoPP::RSA::PrivateKey private_key;
	CryptoPP::StringSource ss(rsa_private_key->get_raw_key(), true);
	private_key.BERDecode(ss);
	CryptoPP::RSA::PublicKey key(private_key);
	rsa_public_key = key;
}

std::string Asymmetric_public_key::encrypt_using_public_key(std::string to_encrypt)
{
	CryptoPP::AutoSeededRandomPool rng;
	std::string encrypted;
	CryptoPP::RSAES_OAEP_SHA_Encryptor e(rsa_public_key);
	CryptoPP::StringSource ss(to_encrypt, true, new CryptoPP::PK_EncryptorFilter(rng, e, new CryptoPP::StringSink(encrypted)));
	return encrypted;
}

std::string Asymmetric_public_key::get_raw_key_in_string()
{
	CryptoPP::ByteQueue queue;
	rsa_public_key.Save(queue);
	std::string str;
	CryptoPP::StringSink ss(str);
	queue.CopyTo(ss);
	ss.MessageEnd();
	return str;
}
